use std::mem;
use std::collections::HashSet;
use std::io::Write;

// TODO: remove
use petgraph::graph::NodeIndex;

use Instruction;
use Expression;
use Result;
use ControlFlowGraph;
use control_flow::{GraphEdge, LoopType};

pub struct StatementConstructor {
	block_done: HashSet<NodeIndex>,
	if_follow: Vec<NodeIndex>,
	latch_node: Vec<NodeIndex>,
	gotos: HashSet<NodeIndex>,
}

impl StatementConstructor {
	pub fn new() -> StatementConstructor {
		StatementConstructor {
			block_done: HashSet::new(),
			if_follow: Vec::new(),
			latch_node: Vec::new(),
			gotos: HashSet::new(),
		}
	}

	pub fn construct_statements(&mut self, cfg: &ControlFlowGraph, entry: NodeIndex) -> Vec<Statement> {
		let mut result = Vec::new();

		let mut block_id = Some(entry);

		let mut line_num = None;

		// Construct entry differently
		if !cfg.is_function && entry == cfg.entry {
			let follow = cfg.if_follow.get(&entry).cloned();

			let mut entry_statement = EntryStatement::new();

			if let Some(follow) = follow {
				self.if_follow.push(follow);
			}
			let mut succ = cfg.graph.neighbors(entry).detach();
			while let Some((edge_index, node_index)) = succ.next(&cfg.graph) {
				let edge = cfg.graph[edge_index];
				if let GraphEdge::Entry(z_level) = edge {
					let block = self.construct_statements(cfg, node_index);
					entry_statement.add_entry(z_level, block);
				} else {
					error!("Entry has edge {:?}", edge);
				}
			}
			self.if_follow.pop();

			result.push(Statement {
				s_type: StatementType::Entry(entry_statement),
				line_num: None
			});

			block_id = follow;
		}

		while let Some(index) = block_id {
			let block = &cfg.graph[index];

			// Break if reached the follow node *BEFORE* visiting
			if self.if_follow.last() == Some(&index) {
				break;
			} else if self.block_done.contains(&index) {
				result.push(Statement::goto(index));
				self.gotos.insert(index);
				break;
			}

			result.push(Statement::label(index));
			self.block_done.insert(index);

			// Handle header nodes of loops separately
			if let Some(ref loop_ref) = cfg.loops.iter().find(|&ref loop_ref| loop_ref.head == index) {
				if loop_ref.head == loop_ref.latch {
					panic!("TODO: Loops where the header is the latch node.");
				}
				let loop_statement = match loop_ref.loop_type {
					// Pre-tested:
					// A<--.
					// |\  |
					// | \ |
					// |  \|
					// C   B
					// Before: A
					// Condition: A if else block is C, else negate(A)
					// Body: B + A
					LoopType::PreTested => {
						let mut loop_before = Vec::new();
						for inst in block.instructions.iter() {
							if let &Instruction::Line(line) = inst {
								line_num = Some(line as usize);
							} else {
								loop_before.push(Statement::new(inst.clone(), line_num.take()));
							}
						}
						result.extend(loop_before.clone());
						
						let condition = cfg.graph[loop_ref.head].condition.clone().unwrap();

						let (then_index, else_index) = cfg.get_then_else(loop_ref.head).unwrap();
						let (succ, condition) = if loop_ref.loop_follow == Some(else_index) {
							(then_index, condition)
						} else {
							(else_index, condition.negate())
						};

						self.latch_node.push(loop_ref.latch);
						let mut loop_contents = self.construct_statements(cfg, succ);
						self.latch_node.pop();


						loop_contents.extend(loop_before);
						Statement {
							s_type: StatementType::Loop(LoopStatement::While {
								condition,
								body: loop_contents
							}),
							line_num: None
						}
					},
					// Post-tested:
					// A<--.
					//  \  |
					//   \ |
					//    \|
					//     B ---> C
					// Before: Nothing
					// Condition: B if else block is C, else negate(B)
					// Body: A + B
					LoopType::PostTested => {
						let mut loop_contents = Vec::new();
						for inst in block.instructions.iter() {
							if let &Instruction::Line(line) = inst {
								line_num = Some(line as usize);
							} else {
								loop_contents.push(Statement::new(inst.clone(), line_num.take()));
							}
						}

						self.latch_node.push(loop_ref.latch);
						for succ in cfg.graph.neighbors(index) {
							loop_contents.extend(self.construct_statements(cfg, succ));
						}
						self.latch_node.pop();


						// Negate the condition if the loop breaks when it is true
						//	i.e. the `then` block is outside the loop
						let (_, else_index) = cfg.get_then_else(loop_ref.latch).unwrap();
						let condition = cfg.graph[loop_ref.latch].condition.clone().unwrap();
						let condition = if loop_ref.loop_follow == Some(else_index) {
							condition
						} else {
							condition.negate()
						};

						Statement {
							s_type: StatementType::Loop(LoopStatement::DoWhile {
								condition,
								body: loop_contents
							}),
							line_num: None
						}
					},
					LoopType::Endless => {
						panic!("Endless loops not supported yet!");
					},
				};

				// Continue with follow
				result.push(loop_statement);
				block_id = loop_ref.loop_follow;
			} else {
				for inst in block.instructions.iter() {
					if let &Instruction::Line(line) = inst {
						line_num = Some(line as usize);
					} else {
						result.push(Statement::new(inst.clone(), line_num.take()));
					}
				}

				// Break if reached the latch node *AFTER* visiting
				if self.latch_node.last() == Some(&index) {
					break;
				}


				let mut succ = cfg.graph.neighbors(index);
				match succ.clone().count() {
					0 | 1 => block_id = succ.next(),
					2 => {
						let if_statement = {
							let (then_index, else_index) = cfg.get_then_else(index).unwrap();

							// TODO: clean this
							// Check if it is of the form
							// if (cond) {
							//	 <then node>
							// }
							// <follow node>
							let (then_block, else_block, condition) = {
								let mut condition = block.condition.clone().unwrap_or(Expression::Error);
								if let Some(&block_follow) = cfg.if_follow.get(&index) { 
									let mut empty_then = false;

									let mut then_block;
									// TODO: find a proper way to do this
									if self.block_done.contains(&then_index) {
										then_block = vec![Statement::goto(then_index)];
										self.gotos.insert(then_index);
									} else if then_index != block_follow {
										self.if_follow.push(block_follow);
										then_block = self.construct_statements(cfg, then_index);
										self.if_follow.pop();
									} else {
										// then clause is the follow node
										// if (condition) {
										// } else {
										//   do_something();
										// }
										// do_follow();
										empty_then = true;
										condition = condition.negate();
										self.if_follow.push(block_follow);
										then_block = self.construct_statements(cfg, else_index);
										self.if_follow.pop();
									};

									let mut else_block;
									if self.block_done.contains(&else_index) {
										if !empty_then {	// didn't just write it
											else_block = Some(vec![Statement::goto(else_index)]);
											self.gotos.insert(else_index);
										} else {
											else_block = None;
										}
									} else if else_index != block_follow {
										self.if_follow.push(block_follow);
										else_block = Some(self.construct_statements(cfg, else_index));
										self.if_follow.pop();
									} else {
										else_block = None;
									};

									(then_block, else_block, condition)
								} else { // No follow, emit if..then..else
									let then_block = self.construct_statements(cfg, then_index);
									let else_block = Some(self.construct_statements(cfg, else_index));
									(then_block, else_block, condition)
								}
							};

							IfStatement::new(condition, then_block, else_block)
						};

						let if_statement = Statement {
							s_type: StatementType::If(if_statement),
							line_num: line_num.take()
						};
						result.push(if_statement);
						block_id = cfg.if_follow.get(&index).cloned();
					},
					_ => panic!("No non-entry switches!")
				}
			}

		}

		if let Some(num) = line_num {
			warn!("Unused line number {}", num);
		}

		result
	}
}




// These aren't really statements, I picked a bad name
#[derive(Clone)]
pub struct Statement {
	s_type: StatementType,
	line_num: Option<usize>,
}

#[derive(Clone)]
enum StatementType {
	Inst(Instruction),
	If(IfStatement),
	Loop(LoopStatement),
	Goto(NodeIndex),
	Label(NodeIndex),
	Entry(EntryStatement)
}

#[derive(Clone)]
struct IfStatement {
	condition: Expression,
	then_block: Vec<Statement>,
	else_block: Option<Vec<Statement>>
}

impl IfStatement {
	fn new(condition: Expression, then_block: Vec<Statement>, else_block: Option<Vec<Statement>>) -> Self {
		if let Some(ref v) = else_block {
			if v.is_empty() {
				return IfStatement {
					condition,
					then_block,
					else_block: None
				}
			}
		}

		IfStatement {
			condition,
			then_block,
			else_block
		}
	}
	fn apply_func_to_blocks<F>(&mut self, mut func: F)
	where 
		F: FnMut(&mut Vec<Statement>) -> (),
	{
		func(&mut self.then_block);
		if let Some(ref mut block) = self.else_block {
			func(block);
		}
	}
}

#[derive(Clone)]
enum LoopStatement {
	While {
		condition: Expression,
		body: Vec<Statement>
	},
	DoWhile {
		condition: Expression,
		body: Vec<Statement>
	},
	For {
		// Header
		init: Option<Instruction>,
		condition: Expression,
		after: Option<Instruction>,

		body: Vec<Statement>,
	}
}

impl LoopStatement {
	fn write<F: Write>(&self, out: &mut F, indent: usize, comment: String) -> Result<()> {
		match *self {
			LoopStatement::While { ref condition, ref body } => {
				write!(out, "{}while ({}) {{ {}\n", "\t".repeat(indent), condition, comment)?;
				for s in body.iter() {
					s.write(out, indent + 1)?;
				}
				write!(out, "{}}} \n", "\t".repeat(indent))?;
			},
			LoopStatement::DoWhile { ref condition, ref body } => {
				write!(out, "{}do {{ {}\n", "\t".repeat(indent), comment)?;
				for s in body.iter() {
					s.write(out, indent + 1)?;
				}
				write!(out, "{}}} while ({}) \n", "\t".repeat(indent), condition)?;
			},
			LoopStatement::For { ref init, ref condition, ref after, ref body } => {
				let init_str = if let Some(inst) = init { format!("{}", inst) } else { String::new() };
				let next_str = if let Some(inst) = after { format!("{}", inst) } else { String::new() };
				
				write!(out, "{}for({};{};{}) {{ {}\n", "\t".repeat(indent), init_str, condition, next_str, comment)?;
				for s in body.iter() {
					s.write(out, indent + 1)?;
				}
				write!(out, "{}}} \n", "\t".repeat(indent))?;
			},
		}
		Ok(())
	}

	fn apply_func_to_blocks<F>(&mut self, mut func: F)
	where 
		F: FnMut(&mut Vec<Statement>) -> (),
	{
		match self {
			LoopStatement::While { ref mut body, .. } => func(body),
			LoopStatement::DoWhile { ref mut body, .. } => func(body),
			LoopStatement::For { ref mut body, .. } => func(body),

		}
	}
}

#[derive(Clone)]
struct EntryStatement {
	entries: Vec<(usize, Vec<Statement>)>
}

impl EntryStatement {
	fn new() -> Self {
		EntryStatement {
			entries: Vec::new()
		}
	}

	fn add_entry(&mut self, z_level: usize, block: Vec<Statement>) {
		self.entries.push((z_level, block));
	}

	fn apply_func_to_blocks<F>(&mut self, mut func: F)
	where 
		F: FnMut(&mut Vec<Statement>) -> (),
	{
		for &mut (_, ref mut block) in self.entries.iter_mut() {
			func(block);
		}
	}
}


impl Statement {
	fn new(inst: Instruction, line_num: Option<usize>) -> Statement {
		Statement {
			s_type: StatementType::Inst(inst),
			line_num
		}
	}

	fn goto(block: NodeIndex) -> Statement {
		warn!("Creating a goto for block {}", block.index());
		Statement {
			s_type: StatementType::Goto(block),
			line_num: None
		}
	}

	fn label(block: NodeIndex) -> Statement {
		Statement {
			s_type: StatementType::Label(block),
			line_num: None
		}
	}

	pub fn write<F: Write>(&self, out: &mut F, indent: usize) -> Result<()> {
		let comment = if let Some(line_num) = self.line_num {
			format!("// line {}", line_num)
		} else {
			"".to_string()
		};
		match self.s_type {
			StatementType::Inst(ref inst) => write!(out, "{}{} {}\n", "\t".repeat(indent), inst, comment)?,
			StatementType::If(ref statement) => {
				write!(out, "{}if ({}) {{ {}\n", "\t".repeat(indent), statement.condition, comment)?;
				for s in statement.then_block.iter() {
					s.write(out, indent + 1)?;
				}

				if let Some(ref block) = statement.else_block {
					write!(out, "{}}} else {{\n", "\t".repeat(indent))?;
					for s in block.iter() {
						s.write(out, indent + 1)?;
					}
				}
				write!(out, "{}}}\n", "\t".repeat(indent))?;
			},
			StatementType::Entry(ref statement) => {
				write!(out, "{}Entry {{ \n", "\t".repeat(indent))?;
				for &(z_level, ref block) in statement.entries.iter().rev() {
					write!(out, "{}z {}: \n", "\t".repeat(indent), z_level)?;
					for s in block.iter() {
						s.write(out, indent + 1)?;
					}
				}
				write!(out, "{}}} \n", "\t".repeat(indent))?;
			},
			StatementType::Loop(ref statement) => statement.write(out, indent, comment)?,
			StatementType::Goto(block) => write!(out, "{}goto label_{} \n", "\t".repeat(indent), block.index())?,
			StatementType::Label(block) => write!(out, "{}label_{}: \n", "\t".repeat(indent), block.index())?,
		}
		Ok(())
	}
}


// Simplification
pub fn simplify_statements(block: &mut Vec<Statement>, ctor: &StatementConstructor) {
	remove_labels(block, &ctor.gotos);

	construct_for_loops(block);

	//fold_ifs(block);
}

fn remove_labels(statements: &mut Vec<Statement>, gotos: &HashSet<NodeIndex>) {
	// poor man's retain_mut/drain_filter
	let mut i = 0;
	while i != statements.len() {
		let to_remove = {
			let s = &mut statements[i];
			match s.s_type {
				StatementType::Label(ref label) => !gotos.contains(label),
				StatementType::If(ref mut statement) => {statement.apply_func_to_blocks(|block| remove_labels(block, gotos)); false},
				StatementType::Loop(ref mut statement) => {statement.apply_func_to_blocks(|block| remove_labels(block, gotos)); false},
				StatementType::Entry(ref mut statement) => {statement.apply_func_to_blocks(|block| remove_labels(block, gotos)); false},
				StatementType::Inst(_) => false,
				StatementType::Goto(_) => false,
			}
		};
		if to_remove {
			statements.remove(i);
		} else {
			i += 1;
		}
	}
}

fn construct_for_loops(block: &mut Vec<Statement>) {
	// Contruct for loops from the following
	//	if (some_condition) {
	//		do {
	//			do_something();
	//		} while (some_condition);
	//	}
	//	=>
	//	for (;some_condition;) {
	//		do_something();
	//	}
	// TODO: clean up this mess
	for s in block.iter_mut() {
		let s_type = &mut s.s_type;
		let for_loop = match *s_type {
			StatementType::If(ref mut statement) => 
				convert_if_to_for(statement)
					.or_else(|| {statement.apply_func_to_blocks(construct_for_loops); None}),
			StatementType::Loop(ref mut statement) => {statement.apply_func_to_blocks(construct_for_loops); None},
			_ => None,
		};

		if let Some(s) = for_loop {
			mem::replace(s_type, StatementType::Loop(s));
		}
	}

	// Leaves the if statement in a broken state if Some(Loop) is returned
	fn convert_if_to_for(if_statement: &mut IfStatement) -> Option<LoopStatement> {
		if if_statement.then_block.len() != 1 || !if_statement.else_block.is_none() {
			return None;
		}
		let if_cond = &if_statement.condition;
		if let Statement { s_type: StatementType::Loop(ref mut loop_statement), ..} = if_statement.then_block[0] {
			if let LoopStatement::DoWhile { condition, body } = loop_statement {
				if condition == if_cond {
					construct_for_loops(body);

					return Some(LoopStatement::For {
						init: None,
						condition: mem::replace(condition, Expression::Error),
						after: None,
						body: mem::replace(body, Vec::new())
					});
				}
			}
		}
		None
	}
}

// fn fold_ifs(block: &mut Vec<Statement>) {
// 	for s in block.iter_mut() {
// 		match s.s_type {
// 			StatementType::If(ref mut statement) => {

// 			},
// 			StatementType::Loop(ref mut statement) => statement.apply_func_to_blocks(fold_ifs),
// 			_ => {},
// 		};
// 	}
// }