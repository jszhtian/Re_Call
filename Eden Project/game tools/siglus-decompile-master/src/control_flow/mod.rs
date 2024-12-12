// TODO: clean up the loop detection and structuring code
// TODO: find all the TODOs and fix
// TODO: remove the panics
// TODO: make the gotos more robust - have an actual labelling system
//	 TODO: this includes the stupid ass current line system with statements right now
//	 TODO: seriously, even a solution involving assigning a unique id to each statement that begins a block
//			linking gotos to that unique id
//			and checking if it is linked to and adding the label when writing would be a better solution
//	 TODO: ^ do that, at least its better than this crap
// TODO: do some tests

use std;
use std::io::Write;

use std::collections::{HashSet, HashMap};

use petgraph::Direction;
use petgraph::graph::{Graph, NodeIndex};
use petgraph::visit::{IntoNodeReferences, DfsPostOrder, Walker};
use petgraph::algo::dominators::{simple_fast as find_dominators};
use petgraph::dot::{Dot, Config as DotCfg};

use ::Result;
use ::Expression;

use ::Variable;
use ::Function;

mod instruction;
mod statement;
mod loops;

pub use self::instruction::Instruction;
pub use self::statement::Statement;


use self::statement::StatementConstructor;
use self::statement::simplify_statements;
use self::loops::{Loop, LoopType};

// Note: petgraph NodeIndices must not be used after a node is removed
pub struct ControlFlowGraph {
	graph: Graph<Block, GraphEdge>,
	entry: NodeIndex,
	is_function: bool,

	if_follow: HashMap<NodeIndex, NodeIndex>,
	loop_head: HashMap<NodeIndex, NodeIndex>,
	loop_latch: HashMap<NodeIndex, NodeIndex>,

	loops: Vec<Loop>,

	post_order: Vec<NodeIndex>
}

#[derive(Debug, Copy, Clone, PartialEq)]
enum GraphEdge {
	Entry(usize),
	TwoWay(bool),
	OneWay,
}

impl ControlFlowGraph {
	pub fn main(entrypoints: &[(usize, usize)]) -> Self {
		let mut graph = Graph::new();
		let head = graph.add_node(Block::new(std::usize::MAX));	// TODO: temp, remove
		for &(z_level, entry_address) in entrypoints {
			let succ = graph.add_node(Block::new(entry_address));
			graph.update_edge(head, succ, GraphEdge::Entry(z_level));
		}
		ControlFlowGraph {
			graph,
			entry: head,
			is_function: false,

			if_follow: HashMap::new(),
			loop_head: HashMap::new(),
			loop_latch: HashMap::new(),

			loops: Vec::new(),

			post_order: Vec::new(),
		}
		
	}
	pub fn function(address: usize) -> Self {
		let mut graph = Graph::new();
		let head = graph.add_node(Block::new(address));
		ControlFlowGraph {
			graph,
			entry: head,
			is_function: true,

			if_follow: HashMap::new(),
			loop_head: HashMap::new(),
			loop_latch: HashMap::new(),

			loops: Vec::new(),

			post_order: Vec::new(),
		}
	}

	pub fn get_block_id_at(&mut self, address: usize) -> NodeIndex {
		if let Some((index, ref _block)) = self.graph
			.node_references()
			.find(|&(_index, ref block)| {
				block.address == address
			}
		) { // find_map
			return index;
		}
		self.graph.add_node(Block::new(address))
	}

	pub fn traversed(&self, index: NodeIndex) -> bool {
		self.graph[index].traversed
	}

	pub fn set_traversed(&mut self, index: NodeIndex) {
		self.graph[index].traversed = true;
	}

	pub fn add_inst(&mut self, block_id: NodeIndex, inst: Instruction) {
		self.graph[block_id].add_inst(inst);
	}

	pub fn add_successor(&mut self, pred: NodeIndex, succ: NodeIndex) {
		self.graph.update_edge(pred, succ, GraphEdge::OneWay);
	}

	pub fn add_branch(&mut self, block_id: NodeIndex, condition: Expression, then_block: NodeIndex, else_block: NodeIndex) {
		self.graph[block_id].condition = Some(condition);

		self.graph.update_edge(block_id, then_block, GraphEdge::TwoWay(true));
		self.graph.update_edge(block_id, else_block, GraphEdge::TwoWay(false));
	}

	fn get_then_else(&self, block_id: NodeIndex) -> Result<(NodeIndex, NodeIndex)> {
		let mut succ = self.graph.neighbors(block_id).detach();

		// Edge to true block is labelled with Some(true)
		let (edge_index, node_index) = succ.next(&self.graph).ok_or("")?;
		let edge_weight = self.graph.edge_weight(edge_index).ok_or("")?;
		if edge_weight == &GraphEdge::TwoWay(true) {
			// node_index is the true block, and the next one is false block
			Ok((node_index, succ.next(&self.graph).ok_or("")?.1))
		} else if edge_weight == &GraphEdge::TwoWay(false) {
			Ok((succ.next(&self.graph).ok_or("")?.1, node_index))
		} else {
			Err("Not a valid 2-way".into())
		}
	}
}

// Structuring
impl ControlFlowGraph {
	pub fn structure_statements(&mut self) -> Vec<Statement> {
		trace!("Starting graph structuring...");

		self.structure_loops();

		self.structure_ifs();

		// Construct the actual statements
		let mut constructor = StatementConstructor::new();
		let mut statements = constructor.construct_statements(self, self.entry);
		simplify_statements(&mut statements, &constructor);

		statements
	}

	fn structure_loops(&mut self) {
		trace!("Structuring loops...");
		
		let loops = loops::find_loops(&self.graph, self.entry);

		for loop_ref in loops.iter() {
			for &node in loop_ref.nodes.iter() {
				self.loop_head.entry(node).or_insert(loop_ref.head);
				self.loop_latch.entry(node).or_insert(loop_ref.latch);
			}
		}

		self.loops = loops;
	}


	// Determine follow nodes for 2-way conditional nodes
	// This step must be done after loops are found or weirdness will ensue
	fn structure_ifs(&mut self) {
		trace!("Structuring 2-ways...");
		let dominators = find_dominators(&self.graph, self.entry);

		let mut unresolved: HashSet<NodeIndex> = HashSet::new();

		// For each 2-way node that is not part of the header or latching node of a loop, 
		// the follow node is calculated as the node that takes it as an immediate dominator 
		// and has two or more in-edges (since it must be reached by at least two different paths from the header). 

		// If there is more than one such node, the one that encloses the maximum number of nodes is selected 
		// (i.e. the one with the largest number). 
		
		// If such a node is not found, the 2-way header node is placed on the unresolved set. 
		// Whenever a follow node is found, all nodes that belong to the set of unresolved nodes 
		// are set to have the same follow node as the one just found 
		// (i.e. they are nested conditionals or unstructured conditionals that reach this node).
		for node in self.post_order() {
			// Check if it is a 2-way that is not a head or latching node
			// ALSO structure n-ways at the same time
			if self.graph.edges(node).count() >= 2 && !self.is_head_latch(node) {
				// Find the maximum node (in RPO) such that it
				//	* has more than two predecessors
				//	* is immediately dominated by `node`
				// if it exists.
				// This is equivalent to finding the first node in a post-ordering.
				//	i think

				let mut post_order = self.post_order().into_iter();

				// TODO: only need to check the nodes up to node I think, since 
				//  any nodes after that won't be dominated since they come before
				if let Some(follow) = post_order.find(|&index|
					self.graph.edges_directed(index, Direction::Incoming).count() >= 2
						&&
					dominators.immediate_dominator(index) == Some(node)
						&& // don't want a loop header that only lies on one of the branches to be the follow node
					self.loop_head.get(&index) != Some(&index)

				) {
					// Set the follow node of all unresolved nodes that lie between the head and the follow
					self.if_follow.insert(node, follow);
					let mut resolved = Vec::new();
					while let Some(n) = post_order.next() {
						if n == node {
							break;
						}
						if unresolved.remove(&n) {
							self.if_follow.insert(n, follow);
							resolved.push(n.index());
						}
					}
					trace!("If follow node found for {}: {}. Inserting {:?}", 
						node.index(), follow.index(), resolved);
				} else {
					unresolved.insert(node);
				}
			}
		}
		if !unresolved.is_empty() {
			warn!("Unresolved 2-way nodes: {:?}", set2vec(&unresolved));
		}
	}

	// If a node is a header or latching node,
	//  it will have itself as the corresponding node
	fn is_head_latch(&self, node: NodeIndex) -> bool {
		self.loop_head.get(&node) == Some(&node) ||
		self.loop_latch.get(&node) == Some(&node)
	}


	// TODO: check if needs rebuilding
	fn post_order(&mut self) -> Vec<NodeIndex> {
		if self.post_order.is_empty() {
			trace!("Computing post order traversal...");
			self.post_order = DfsPostOrder::new(&self.graph, self.entry).iter(&self.graph).collect();
		}

		self.post_order.clone()
	}
}

impl ControlFlowGraph {
	pub fn write_graph<F: Write>(&self, out: &mut F) -> Result<()> {
		let dot = Dot::with_config(&self.graph, &[DotCfg::NodeIndexLabel, DotCfg::EdgeNoLabel]);
		write!(out, "{:?}", dot)?;
		Ok(())
	}
}

// Replace ref
impl ControlFlowGraph {
	pub fn replace_ref(&mut self, global_vars: &[Variable], functions: &[Function], local_vars: &[Variable]) {
		trace!("Replacing references..");
		for block in self.graph.node_weights_mut() {
			for inst in block.instructions.iter_mut() {
				inst.replace_ref(global_vars, functions, local_vars);
			}
			block.condition.as_mut().map(|expr| expr.replace_ref(global_vars, functions, local_vars));
		}
	}
}

// Basic Blocks
struct Block {
	address: usize,

	traversed: bool,

	instructions: Vec<Instruction>,
	condition: Option<Expression>,
}

impl Block {
	fn new(address: usize) -> Self {
		Block {
			address,

			traversed: false,

			instructions: Vec::new(),
			condition: None,
		}
	}

	fn add_inst(&mut self, inst: Instruction) {
		self.instructions.push(inst);
	}
}

impl std::fmt::Debug for Block {
	fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
		write!(f, "{:0x}", self.address)
	}
}


//======================================================================

// Util
fn set2vec(set: &HashSet<NodeIndex>) -> Vec<usize> {
	set.iter().cloned().map(NodeIndex::index).collect::<Vec<usize>>()
}