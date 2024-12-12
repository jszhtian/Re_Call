use std::fmt;

use Expression;
use Variable;
use Function;

use format_list;

// Instructions
#[derive(Clone)]
pub enum Instruction {
	Line(u32),
	Binding { lhs: Expression, rhs: Expression },
	AddText(Expression, u32),
	SetName(Expression),
	Return(Vec<Expression>),
	Expression(Expression),
	Nop(u8)
}

impl Instruction {
	pub fn replace_ref(&mut self, global_vars: &[Variable], functions: &[Function], local_vars: &[Variable]) {
		match *self {
			Instruction::Binding { ref mut lhs, ref mut rhs } => {
				lhs.replace_ref(global_vars, functions, local_vars);
				rhs.replace_ref(global_vars, functions, local_vars);
			},
			Instruction::AddText(ref mut expr, _) => expr.replace_ref(global_vars, functions, local_vars),
			Instruction::SetName(ref mut expr) => expr.replace_ref(global_vars, functions, local_vars),
			Instruction::Return(ref mut vec) => for expr in vec.iter_mut() {
				expr.replace_ref(global_vars, functions, local_vars);
			},
			Instruction::Expression(ref mut expr) => expr.replace_ref(global_vars, functions, local_vars),
			Instruction::Line(_) => (),
			Instruction::Nop(_) => (),
		}
	}
}

impl fmt::Display for Instruction {
	fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
		match *self {
			Instruction::Line(num) => write!(f, "line {}", num),
			Instruction::Binding { ref lhs, ref rhs } => write!(f, "{} = {}", lhs, rhs),
			Instruction::AddText(ref text, id) => write!(f, "addtext {}, {}", text, id),
			Instruction::SetName(ref name) => write!(f, "setname {}", name),
			Instruction::Return(ref args) =>
				if args.is_empty() {
					write!(f, "return")
				} else {
					write!(f, "return({})", format_list(args.iter()))
				},
			Instruction::Expression(ref expr) => {
				// If the expression is a variable, treat it as a declaration
				if let Expression::Variable(Variable { ref name, ref var_type }) = expr {
					write!(f, "{} {}", var_type, name)
				} else {
					write!(f, "{}", expr)
				}
			}
			Instruction::Nop(op) => write!(f, "nop {:#2x}", op)
		}
	}
}
