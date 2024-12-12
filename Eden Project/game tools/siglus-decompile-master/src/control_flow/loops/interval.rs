use std::collections::{HashSet, HashMap};
use std::fmt;

use petgraph::Direction;
use petgraph::graph::{Graph, NodeIndex};
use petgraph::visit::{IntoNodeReferences, DfsPostOrder, Walker};
use petgraph::algo::is_isomorphic;

use super::{Loop, get_loop_type, get_loop_follow};

use format_list;

struct Interval {
	header: NodeRef,
	root_start: NodeIndex,
	nodes: HashSet<NodeRef>,
	root_end: Option<NodeIndex>,
}

impl fmt::Display for Interval {
	fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
		write!(f, "Header: {:?} ", self.header)?;
		write!(f, "Head:{} End: {:?} ", self.root_start.index(), self.root_end.map(NodeIndex::index))?;
		write!(f, "Nodes: {{{}}} \n", format_list(self.nodes.iter()))?;
		Ok(())
	}
}


#[derive(PartialEq, Eq, Hash, Copy, Clone, Debug)]
struct IntervalRef {
	// Index inside IntervalData's list
	idx: usize,
	// Nesting layer
	//	0 means the children are root nodes,
	//	1 means this contains intervals that contain root nodes, etc
	level: usize,
}

pub struct IntervalData<'a, N: 'a, E: 'a> {
	// List of all intervals at all levels and their corresponding node indices in the graph
	intervals: Vec<(Interval, NodeIndex)>,
	// `deriv_seq[i]` is a list of interval references at level `i`
	deriv_seq: Vec<Vec<IntervalRef>>,
	// `interval_graphs[i]` has node weights of interval references at level `i`
	// NOTE: this is not the original definition we used
	interval_graphs: Vec<Graph<IntervalRef, ()>>,

	// Reference to a graph with the original nodes
	graph: &'a Graph<N, E>,
	// Original definition
	// G1 == graph,           I1 == deriv_seq[0]
	// G2 == itvl_graphs[0],  I2 == deriv_seq[1]

	// List of root nodes in post ordering
	post_order: Vec<NodeIndex>,
	rpo: HashMap<NodeIndex, usize>,
}

#[derive(PartialEq, Eq, Hash, Copy, Clone, Debug)]
enum NodeRef {
	Interval(IntervalRef),
	RootNode(NodeIndex),
}

impl fmt::Display for NodeRef {
	fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
		match *self {
			NodeRef::RootNode(i) => write!(f, "Root({})", i.index()),
			NodeRef::Interval(i) => write!(f, "IntRef({}, {})", i.idx, i.level),
		}
	}
}

impl From<IntervalRef> for NodeRef {
	fn from(interval: IntervalRef) -> Self {
		NodeRef::Interval(interval)
	}
}

impl From<NodeIndex> for NodeRef {
	fn from(node: NodeIndex) -> Self {
		NodeRef::RootNode(node)
	}
}

impl<'a, N, E> IntervalData<'a, N, E> {
	pub fn new(graph: &'a Graph<N, E>, entry: NodeIndex) -> Self {
		let post_order: Vec<NodeIndex> = DfsPostOrder::new(graph, entry).iter(graph).collect();
		let rpo: HashMap<NodeIndex, usize> = 
			post_order
				.iter()
				.rev()
				.enumerate()
				.map(|(i, &node)| (node, i))
				.collect();

		let mut data = IntervalData {
			intervals: Vec::new(),
			deriv_seq: Vec::new(),
			interval_graphs: Vec::new(),

			graph,
			post_order,
			rpo
		};
		data.construct(entry);

		data
	}

	fn construct(&mut self, entry: NodeIndex) {
		trace!("Generating sequence of interval derived graphs...");

		// Maps nodes to the node index of the interval containing them
		let mut node_to_interval: HashMap<NodeRef, NodeIndex> = HashMap::new();

		let mut n = 0;
		// Start off with a mapped version of the root graph
		// Mapping gives a concrete type to use for interval `successors`
		// and also allows easier graph isomorphism checking
		let mut last_graph = self.graph.map(|idx, _| idx.into(), |_, _| ());
		let mut entry: NodeRef = entry.into();
		'construct: loop {
			// Construct the next graph
			let graph: Graph<NodeRef, ()> = {
				let mut graph: Graph<NodeRef, ()> = Graph::new();
				let mut level_intervals = Vec::new();

				// Add nodes
				let new_intervals = self.compute_intervals(&last_graph, entry.into());
				let num_intervals = self.intervals.len();
				self.intervals.extend(new_intervals.into_iter().enumerate().map(|(i, interval)| {
					let itvl_ref = IntervalRef {
						idx: num_intervals + i,
						level: n
					};
					level_intervals.push(itvl_ref);
					let node_index = graph.add_node(itvl_ref.into());
					for &node in interval.nodes.iter() {
						node_to_interval.insert(node, node_index);
					}
					(interval, node_index)
				}));

				// Add edges
				let mut edges: Vec<(NodeIndex, NodeIndex)> = Vec::new();
				for (node, node_ref) in graph.node_references() {
					let itvl_ref = if let NodeRef::Interval(r) = node_ref { *r } else { panic!("error") };
					let interval = &self.intervals[itvl_ref.idx];
					// Use the last graph to get the successors of nodes inside this interval
					for succ_head in self.successors(&interval.0.nodes, &last_graph) {
						edges.push((node, node_to_interval[&succ_head]));
					}
				}
				for edge in edges {
					graph.update_edge(edge.0, edge.1, ());
				}

				// Update entry to the node index of the interval containing entry
				// node_to_interval: node_ref -> node_index
				// graph[node_index]: node_index -> interval_ref
				// into: interval_ref -> node_ref
				entry = graph[node_to_interval[&entry]].into();

				// Append list of references to intervals that were added
				self.deriv_seq.push(level_intervals);

				graph
			};

			// If needed, use `is_isomorphic_matching`
			if is_isomorphic(&graph, &last_graph) {
				break 'construct;
			}

			// TODO: this is an ugly workaround
			self.interval_graphs.push(graph.map(
				|_, n| if let NodeRef::Interval(r) = n { *r } else { panic!("Something went horribly wrong") },
				|_, &e| e 
			));

			last_graph = graph;
			trace!("\tConstructed graph G{}", n + 1);
			trace!("\tIntervals: {}", format_list(self.deriv_seq.last().unwrap().iter().map(|i_ref| &self.intervals[i_ref.idx].0)));
			n += 1;
		}

		// Presumably, if no interval graphs were created the original graph was a single node
		let reducible = self.interval_graphs.last().map(Graph::node_count).unwrap_or(1) == 1;
		trace!("Generated sequence of {} graphs. The final graph was {}.", self.interval_graphs.len() + 1, if reducible { "reducible" } else { "irreducible"});
	}

	fn compute_intervals(&self, graph: &Graph<NodeRef, ()>, entry: NodeRef) -> Vec<Interval> {
		let mut work_list = vec![entry];

		let mut intervals = Vec::new();
		let mut heads = HashSet::new();

		// Compute list of intervals
		while let Some(head) = work_list.pop() {
			if heads.contains(&head) {
				continue;
			}
			
			heads.insert(head);
			let mut nodes = HashSet::new();
			nodes.insert(head);

			// Add all nodes that are dominated by this header
			//  We find all successors of nodes in the interval that are outside the interval
			//  and add them if all their predecessors are inside the interval
			//  Repeat until there is none to be added
			while self.add_all_dominated(&mut nodes, graph) {}

			// Add any nodes that are not in interval but have a predecessor in it to worklist
			for next_head in self.successors(&nodes, graph) {
				work_list.push(next_head);
			}

			// Compute the end node of the interval
			let end = self.compute_end(&nodes, graph).and_then(|end| self.get_end(end));

			intervals.push(Interval {
				header: head,
				nodes,

				root_start: self.get_head(head).unwrap(),
				root_end: end,
			});
		}

		intervals
	}

	// Returns true if anything was added
	fn add_all_dominated(&self, nodes: &mut HashSet<NodeRef>, graph: &Graph<NodeRef, ()>) -> bool {
		let nodes_indices: HashSet<NodeIndex> = nodes.iter().map(|&node_ref| self.get_node_index(node_ref)).collect();

		let succ: HashSet<NodeIndex> = nodes_indices.iter().flat_map(|&index| {
			graph.neighbors_directed(index, Direction::Outgoing)
		}).collect();

		let num_changes: i32 = succ.difference(&nodes_indices).cloned().map(|node| {
			// Check if all predecessors are in the interval
			let mut pred = graph.neighbors_directed(node, Direction::Incoming);
			let to_add = pred.all(|p| nodes.contains(&graph[p]));
			if to_add {
				nodes.insert(graph[node]);
			}
			to_add as i32
		}).sum();

		num_changes != 0
	}

	// TODO - might be flawed
	// Finds the end node of a set of nodes
	fn compute_end(&self, nodes: &HashSet<NodeRef>, graph: &Graph<NodeRef, ()>) -> Option<NodeRef> {
		let mut end = None;
		for &node in nodes.iter() {
			let mut succ = graph.neighbors_directed(self.get_node_index(node), Direction::Outgoing);
			if succ.any(|idx| !nodes.contains(&graph[idx])) {
				end = Some(node);
			}
		}
		end
	}

	// Returns a vector v = {n | there exists `e in nodes` with `n in e.succ` and `n not in nodes`}
	// nodes: Set of nodes to get successors for
	// graph: Graph corresponding to nodes
	fn successors(&self, nodes: &HashSet<NodeRef>, graph: &Graph<NodeRef, ()>) -> Vec<NodeRef> {
		let succ: HashSet<NodeRef> = nodes.iter().flat_map(|&index| {
			graph.neighbors_directed(self.get_node_index(index), Direction::Outgoing).map(NodeIndex::into)
		}).collect();

		succ.difference(nodes).cloned().collect()
	}

	fn post_order(&'a self) -> impl DoubleEndedIterator<Item = NodeIndex> + Clone + 'a {
		self.post_order.iter().cloned()
	}

	// Returns a vector of loops from the innermost first
	pub fn find_loops_in_order(&self) -> Vec<Loop> {
		let mut loops = Vec::new();
		for intervals in self.deriv_seq.iter() {
			for &interval in intervals.iter() {
				let header = self.intervals[interval.idx].0.header;
				// Get the predecessors of the interval's header
				let mut pred = self.get_pred(header).into_iter();
				// Check if any of them lie inside the interval
				if let Some(latch) = pred.find(|p| self.contains(interval, p)) {
					let head = match self.get_head(interval.into()) {
						Some(head) => head,
						None => panic!("Interval header has no head: {:?}", interval)
					};
					let latch = match self.get_end(latch.into()) {
						Some(latch) => latch,
						None => panic!("Latch node has no end: {:?}", latch)
					};
					loops.push(self.create_loop(head, latch, interval));
				}
			}
		}
		loops

		// TODO: set loop_head and loop_latch in the messy implementation
	}

	fn create_loop(&self, head: NodeIndex, latch: NodeIndex, interval: IntervalRef) -> Loop {
		let mut nodes_in_loop = HashSet::new();
			nodes_in_loop.insert(head);

		// EDIT: this is flawed
		// there are graphs where a valid post ordering has nodes outside the loop
		// between the latch node and the header
		// Iterate through head + 1 to latch
		// let mut rpo = self.post_order().rev();
		// rpo.find(|&node| node == head);	// `find` consumes the one found and continues on the next one
		// while let Some(node) = rpo.next() {
		// 	if self.contains_root(interval, node) {
		// 		nodes_in_loop.insert(node);
		// 	}
		// 	if node == latch {
		// 		break;
		// 	}
		// }

		// March backwards through the root graph
		// starting from the latch node
		// adding all nodes that are predecessors
		// and lie in the correct range
		let start = self.get_rpo(&head);
		let end = self.get_rpo(&latch);
		let mut to_visit = HashSet::new();
			to_visit.insert(latch);
		// While there are more nodes
		while !to_visit.is_subset(&nodes_in_loop) {
			let mut temp = HashSet::new();
			for node in to_visit.drain() {
				nodes_in_loop.insert(node);
				for pred in self.graph.neighbors_directed(node, Direction::Incoming) {
					if self.contains_root(interval, pred) {
						let rpo = self.get_rpo(&pred);
						if start < rpo && rpo <= end {
							temp.insert(pred);
						}
					}
				}
			}
			to_visit = temp;
		}


		// Find loop type
		let loop_type = get_loop_type(head, latch, &nodes_in_loop, &self.graph);

		// Find loop follow - currently this is wrong and might pick a node that is earlier
		let loop_follow = get_loop_follow(loop_type, head, latch, &nodes_in_loop, &self.graph);

		let l = Loop {
			head,
			latch,
			nodes: nodes_in_loop,
			loop_type,
			loop_follow,
		};

		trace!("Found Loop: {}", l);

		l
	}	
}

impl<'a, N, E> IntervalData<'a, N, E> {

	fn get_head(&self, node: NodeRef) -> Option<NodeIndex> {
		match node {
			NodeRef::RootNode(index) => Some(index),
			NodeRef::Interval(interval) => self.intervals.get(interval.idx).map(|itvl| itvl.0.root_start)
		}
	}

	fn get_end(&self, node: NodeRef) -> Option<NodeIndex> {
		match node {
			NodeRef::RootNode(index) => Some(index),
			NodeRef::Interval(interval) => self.intervals.get(interval.idx).and_then(|itvl| itvl.0.root_end)
		}
	}

	// Checks if `root_node` is a descendant of `interval`
	fn contains_root(&self, interval: IntervalRef, root_node: NodeIndex) -> bool {
		let interval = self.intervals.get(interval.idx);
		interval.map(|itvl| itvl.0.nodes.iter().any(|&node| {
			match node {
				NodeRef::Interval(itvl_ref) => self.contains_root(itvl_ref, root_node),
				NodeRef::RootNode(node_ref) => node_ref == root_node,
			}
		})).unwrap_or(false)
	}

	// Checks if node is a direct child of interval
	fn contains(&self, interval: IntervalRef, node: &NodeRef) -> bool {
		let interval = &self.intervals[interval.idx];
		interval.0.nodes.iter().any(|child| child == node)
	}

	// Gets the predecessors of node
	fn get_pred(&self, node: NodeRef) -> Vec<NodeRef> {
		match node {
			NodeRef::Interval(itvl) => {
				let graph = &self.interval_graphs[itvl.level];
				let node_index = self.intervals[itvl.idx].1;
				let mut pred = graph.neighbors_directed(node_index, Direction::Incoming);
				pred.map(|node| NodeRef::Interval(graph[node])).collect()
			},
			NodeRef::RootNode(node) => {
				let mut pred = self.graph.neighbors_directed(node, Direction::Incoming);
				pred.map(|node| NodeRef::RootNode(node)).collect()
			}
		}
	}

	fn get_node_index(&self, node_ref: NodeRef) -> NodeIndex {
		match node_ref {
			NodeRef::RootNode(idx) => idx,
			NodeRef::Interval(r) => self.intervals[r.idx].1
		}
	}

	// Gets the reverse post ordering of the root node
	fn get_rpo(&self, node: &NodeIndex) -> usize {
		self.rpo[node]
	}
}