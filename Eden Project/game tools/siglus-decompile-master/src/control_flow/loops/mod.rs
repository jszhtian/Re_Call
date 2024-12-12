use std::fmt;
use std::collections::HashSet;

use petgraph::graph::{Graph, NodeIndex};


mod interval;
use self::interval::IntervalData;


use format_list;

pub struct Loop {
	pub head: NodeIndex,
	pub latch: NodeIndex,
	pub nodes: HashSet<NodeIndex>,
	pub loop_type: LoopType,
	pub loop_follow: Option<NodeIndex>,
}

#[derive(Debug, Copy, Clone, PartialEq)]
pub enum LoopType {
	PreTested,
	PostTested,
	Endless
}

pub fn find_loops<N, E>(graph: &Graph<N, E>, entry: NodeIndex) -> Vec<Loop> {
	let interval_data = IntervalData::new(graph, entry);

	interval_data.find_loops_in_order()
}


// TODO: Patch up a bit nicer
fn get_loop_type<N, E>(head: NodeIndex, latch: NodeIndex, nodes: &HashSet<NodeIndex>, graph: &Graph<N, E>) -> LoopType {
	let num_succ_latch = graph.neighbors(latch).count();

	if num_succ_latch >= 2 {
		// Latch node is 2-way - check if head leads outwards
		if graph.neighbors(head).all(|idx| nodes.contains(&idx)) {
			LoopType::PostTested
		} else {
			LoopType::PreTested
		}
	} else {
		// Latch node is 1-way - check if head leads outwards
		if  graph.neighbors(head).all(|idx| nodes.contains(&idx)) {
			LoopType::Endless
		} else {
			LoopType::PreTested
		}
	}
}

// TODO: do correctly
// Currently we do a filter-last to find the last one
fn get_loop_follow<N, E>(loop_type: LoopType, head: NodeIndex, latch: NodeIndex, nodes: &HashSet<NodeIndex>, graph: &Graph<N, E>) -> Option<NodeIndex> {
	match loop_type {
		// Find a successor that isn't in the loop from the head
		LoopType::PreTested => graph.neighbors(head).filter(|idx| !nodes.contains(&idx)).last(),
		// Find a successor that isn't in the loop from the latch
		LoopType::PostTested => graph.neighbors(latch).filter(|idx| !nodes.contains(&idx)).last(),
		// Endless loops don't have a follow node - unless it's the graph's root end node
		LoopType::Endless => None
	}
}

impl fmt::Display for Loop {
	fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
		write!(f, "{:?} ", self.loop_type)?;
		write!(f, "Head: {} Latch: {} Follow: {:?}\n", self.head.index(), self.latch.index(), self.loop_follow.map(NodeIndex::index))?;
		write!(f, "Nodes: {{{}}} \n", format_list(self.nodes.iter().cloned().map(NodeIndex::index)))?;
		Ok(())
	}
}
