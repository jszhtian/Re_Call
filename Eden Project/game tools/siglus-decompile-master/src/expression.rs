use std;

use ::Variable;
use ::VariableType;
use ::Function;
use Instruction;
use format_list;

////////////////////////////////////////////////////////////////////////////////
#[derive(Clone, Debug, PartialEq)]
pub enum Expression {
	RawInt(i32),
	RawString(String),
	Variable(Variable),

	UnaryExpr {
		value: Box<Expression>,
		op: UnaryOp
	},

	BinaryExpr {
		lhs: Box<Expression>,
		rhs: Box<Expression>,
		op: BinaryOp
	},
	Function(FunctionType),
	FunctionCall { 
		function: Box<Expression>, 
		option: u32, 
		args: Vec<Expression>, 
		extra_params: Vec<u32>, 
		return_type: VariableType, 
		extra: Option<u32>
	},
	System(i32),
	List(Vec<Expression>),
	Error,


	LocalVarRef(usize),
	FunctionRef(usize),
	GlobalVarRef(usize),
}

#[derive(Clone, Debug, PartialEq)]
pub enum FunctionType {
	//System(i32),
	Procedure(usize),
	Named(String),
}

// Constructors
impl Expression {
	pub fn index(lhs: Expression, rhs: Expression) -> Expression {
		Expression::BinaryExpr {
			lhs: Box::new(lhs), 
			rhs: Box::new(rhs),
			op: BinaryOp::Index
		}
	}

	pub fn member(lhs: Expression, rhs: Expression) -> Expression {
		Expression::BinaryExpr {
			lhs: Box::new(lhs), 
			rhs: Box::new(rhs),
			op: BinaryOp::Member
		}
	}

	pub fn procedure_call(label: usize, args: Vec<Expression>) -> Expression {
		let function = FunctionType::Procedure(label);
		Expression::FunctionCall {
			function: Box::new(Expression::Function(function)), 
			option: 0, 
			args, 
			extra_params: Vec::new(), 
			return_type:
			VariableType::Unknown, 
			extra: None
		}
	}
}

impl Expression {
	pub fn has_side_effect(&self) -> bool {
		match *self {
			Expression::RawInt(_) => false,
			Expression::RawString(_) => false,
			Expression::Variable { .. } => false,
			Expression::UnaryExpr { ref value, .. } => value.has_side_effect(),
			Expression::BinaryExpr { ref lhs, ref rhs, .. } => lhs.has_side_effect() || rhs.has_side_effect(),
			Expression::Function(_) => panic!("Function being checked! Check it out."),
			Expression::FunctionCall { .. } => true,
			Expression::System(_) => false,
			Expression::List(_) => true,	// lists always have side effects for simplicity
			Expression::Error => true,

			Expression::LocalVarRef(_) |
			Expression::FunctionRef(_) |
			Expression::GlobalVarRef(_) => false,
		}
	}

	pub fn to_inst(self) -> Instruction {
		Instruction::Expression(self)
	}

	pub fn get_type(&self) -> VariableType {
		match *self {
			Expression::RawInt(_) => VariableType::Int,
			Expression::RawString(_) => VariableType::Str,
			Expression::Variable(Variable { var_type, .. }) => var_type,
			Expression::UnaryExpr { ref value, .. } => value.get_type(),	// temp
			Expression::BinaryExpr { ref lhs, ref rhs, op, .. } => {
				match op {
					// TODO: check
					BinaryOp::Index => match lhs.get_type() {
						VariableType::IntList(_) => VariableType::Int,
						VariableType::StrList(_) => VariableType::Str,
						VariableType::ObjList(_) => VariableType::Obj,
						_ => {
							//warn!("Indexing a list: {}", self);
							VariableType::Unknown
						}
					}
					BinaryOp::Eq | BinaryOp::Neq | BinaryOp::And | BinaryOp::Or => VariableType::Int,
					BinaryOp::Member => rhs.get_type(),
					_ => lhs.get_type()
				}
			},
			Expression::Function(_) => panic!(),
			Expression::FunctionCall { return_type, .. } => return_type,
			Expression::System(_) => VariableType::Unknown,
			Expression::List(_) => VariableType::Unknown,	// TODO: check
			Expression::Error => VariableType::Error,

			Expression::LocalVarRef(_) |
			Expression::FunctionRef(_) |
			Expression::GlobalVarRef(_) => VariableType::Unknown,			
		}
	}

	pub fn calc1(value: Expression, op: u8) -> Expression {
		let op = match op {
			0x01 => return value,
			0x02 => UnaryOp::Negate,
			0x30 => UnaryOp::BitwiseNot,
			_    => {
				error!("Unknown unary operation: {:#02x}", op);
				return Expression::Error
			}
		};

		Expression::UnaryExpr {
			value: Box::new(value), op
		}
	}

	pub fn calc2_int(lhs: Expression, rhs: Expression, op: u8) -> Expression {
		let op = match op {
			0x01 => BinaryOp::Add,
			0x02 => BinaryOp::Sub,
			0x03 => BinaryOp::Mul,
			0x04 => BinaryOp::Div,
			0x05 => BinaryOp::Rem,
			0x10 => BinaryOp::Eq,
			0x11 => BinaryOp::Neq,
			0x12 => BinaryOp::Gt,
			0x13 => BinaryOp::Geq,
			0x14 => BinaryOp::Lt,
			0x15 => BinaryOp::Leq,
			0x20 => BinaryOp::And,
			0x21 => BinaryOp::Or,
			0x31 => BinaryOp::BitwiseAnd,
			0x32 => BinaryOp::BitwiseOr,
			0x33 => BinaryOp::BitwiseXor,
			0x34 => BinaryOp::LeftShift,
			0x35 => BinaryOp::RightShift,
			0x36 => BinaryOp::UnsignedRightShift,
			_    => {
				error!("Unknown integer binary operation: {:#02x}", op);
				return Expression::Error
			}
		};

		Expression::BinaryExpr {
			lhs: Box::new(lhs), 
			rhs: Box::new(rhs),
			op
		}
	}

	pub fn calc2_str(lhs: Expression, rhs: Expression, op: u8) -> Expression {
		let op = match op {
			0x01 => BinaryOp::Add,
			0x10 => BinaryOp::Eq,
			0x11 => BinaryOp::Neq,
			_    => {
				error!("Unknown string binary operation: {:#02x}", op);
				return Expression::Error
			}
		};

		Expression::BinaryExpr {
			lhs: Box::new(lhs), 
			rhs: Box::new(rhs),
			op
		}
	}

	// TODO: check if it is already a negate
	// TODO: also delegate if it is a boolean and/or
	pub fn negate(self) -> Expression {
		match self {
			Expression::RawInt(num) => Expression::RawInt(if num == 0 { 1 } else { 0 }),
			Expression::RawString(string) => panic!("Tried to negate {}", string),
			Expression::UnaryExpr{value, op: UnaryOp::BooleanNot} => *value,
			Expression::BinaryExpr{lhs, rhs, op} => {
				match op {
					BinaryOp::Eq => Expression::BinaryExpr{lhs, rhs, op: BinaryOp::Neq},
					BinaryOp::Neq => Expression::BinaryExpr{lhs, rhs, op: BinaryOp::Eq},
					BinaryOp::Gt => Expression::BinaryExpr{lhs, rhs, op: BinaryOp::Leq},
					BinaryOp::Geq => Expression::BinaryExpr{lhs, rhs, op: BinaryOp::Lt},
					BinaryOp::Lt => Expression::BinaryExpr{lhs, rhs, op: BinaryOp::Geq},
					BinaryOp::Leq => Expression::BinaryExpr{lhs, rhs, op: BinaryOp::Gt},
					_ => {
						Expression::UnaryExpr {
						value: Box::new(Expression::BinaryExpr{lhs, rhs, op}),
						op: UnaryOp::BooleanNot
						}
					}
				}
			}
			_ => {
				Expression::UnaryExpr {
					value: Box::new(self),
					op: UnaryOp::BooleanNot
				}
			}	
		}
	}

	fn precedence(&self) -> u8 {
		match *self {
			Expression::UnaryExpr{..} => 11,
			Expression::BinaryExpr{op, ..} => {
				match op {
					BinaryOp::Add => 9,
					BinaryOp::Sub => 9,
					BinaryOp::Eq => 6,
					BinaryOp::Neq => 6,
					BinaryOp::Index => 11,
					BinaryOp::Member => 11,
					BinaryOp::Mul => 10,
					BinaryOp::Div => 10,
					BinaryOp::Rem => 10,
					BinaryOp::Gt => 7,
					BinaryOp::Geq => 7,
					BinaryOp::Lt => 7,
					BinaryOp::Leq => 7,
					BinaryOp::And => 2,
					BinaryOp::Or => 1,
					BinaryOp::BitwiseOr => 3,
					BinaryOp::BitwiseXor => 4,
					BinaryOp::BitwiseAnd => 5,
					BinaryOp::LeftShift => 8,
					BinaryOp::RightShift => 8,
					BinaryOp::UnsignedRightShift => 8,
				}
			},
			_ => 0xFF
		}
	}
}

impl Expression {
	pub fn replace_ref(&mut self, global_vars: &[Variable], functions: &[Function], local_vars: &[Variable]) {
		match *self {
			Expression::RawInt(_) => (),
			Expression::RawString(_) => (),
			Expression::Variable { .. } => (),
			Expression::UnaryExpr { ref mut value, .. } => value.replace_ref(global_vars, functions, local_vars),
			Expression::BinaryExpr { ref mut lhs, ref mut rhs, .. } => {
				lhs.replace_ref(global_vars, functions, local_vars);
				rhs.replace_ref(global_vars, functions, local_vars);
			},
			Expression::Function(_) => (),
			Expression::FunctionCall { ref mut function, ref mut args, .. } => {
				function.replace_ref(global_vars, functions, local_vars);
				for expr in args.iter_mut() {
					expr.replace_ref(global_vars, functions, local_vars);
				}
			},
			Expression::System(_) => (),
			Expression::List(ref mut vec) => for expr in vec.iter_mut() {
				expr.replace_ref(global_vars, functions, local_vars);
			},
			Expression::Error => (),

			Expression::LocalVarRef(index) => {
				if index < local_vars.len() {
					*self = local_vars[index].clone().to_expression();
				} else {
					warn!("Local var reference {} out of range.", index);
				}
			},
			Expression::FunctionRef(index) => {
				if index < functions.len() {
					*self = functions[index].to_expression();
				} else {
					warn!("Function reference {:#x} out of range.", index);
				}
			},
			Expression::GlobalVarRef(index) => {
				if index < global_vars.len() {
					*self = global_vars[index].clone().to_expression();
				} else {
					warn!("Global var reference {} out of range.", index);
				}
			},
		}
	}
}

// TODO: member actually has higher precedence than index, but similar to subtraction
//	Precedence table
//    [-] [~] [!] (Unary) [] (Index) -> (Member)
//    * / %
//    + -
//    << >> >>>
//    < <= > >=
//    == !=
//    &
//    ^
//    |
//    &&
//    ||


impl std::fmt::Display for Expression {
	fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
		match *self {
			Expression::RawInt(num) => write!(f, "{}", num),
			Expression::RawString(ref string) => write!(f, "\"{}\"", string),	// TODO: might need to escape
			Expression::Variable(Variable {ref name, .. }) => write!(f, "{}", name),
			Expression::UnaryExpr{ref value, op} => {
				if self.precedence() > value.precedence() {
					write!(f, "{}({})", op, value)
				} else {
					write!(f, "{}{}", op, value)
				}
			},
			Expression::BinaryExpr{ref lhs, ref rhs, op} => {
				if op == BinaryOp::Member {
					return write!(f, "{:#x}{}{:#x}", lhs.as_ref(), op, rhs.as_ref());
				}

				let precedence = self.precedence();

				let left_expr = if precedence > lhs.precedence() {
					format!("({})", lhs)
				} else {
					format!("{}", lhs)
				};

				if op == BinaryOp::Index {
					write!(f, "{}[{}]", left_expr, rhs)
				} else {
					let right_precedence = rhs.precedence();
					let right_expr = if precedence > right_precedence {
						format!("({})", rhs)
					} else if precedence < right_precedence {
						format!("{}", rhs)
					} else {
						// Non associative
						if op == BinaryOp::Sub || op == BinaryOp::Div || op == BinaryOp::Member {
							format!("({})", rhs)
						} else {
							format!("{}", rhs)
						}
					};

					write!(f, "{}{}{}", left_expr, op, right_expr)
				}
			},
			Expression::Function(ref func_type) => write!(f, "{}", func_type),
			Expression::FunctionCall {
				ref function, option, ref args, ref extra_params, extra, ..
			} => {
				let option_str = if option == 0 { String::new() } else { format!("{{{}}}", option) };
				let extra_param_str = if extra_params.is_empty() { String::new() } else { format!("<{}>", format_list(extra_params.iter())) };
				let extra_str = if let Some(extra) = extra { format!(", {}", extra) } else { String::new() };
				if let &Expression::Variable(_) = function.as_ref() {
					warn!("Function {} is a variable", function);
				}
				write!(f, "{}{}({}){}{}", function, option_str, format_list(args.iter()), extra_param_str, extra_str)
			},
			Expression::System(num) => write!(f, "sys_{:#x}", num),
			Expression::List(ref vec) => write!(f, "{{{}}}", format_list(vec.iter())),
			Expression::Error => write!(f, "ERROR"),

			Expression::LocalVarRef(idx) => write!(f, "local_var[{}]", idx),
			Expression::FunctionRef(idx) => write!(f, "function_{:#x}", idx),
			Expression::GlobalVarRef(idx) => write!(f, "global_var[{:#x}]", idx),
		}
	}
}

impl std::fmt::LowerHex for Expression {
	fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
		match *self {
			Expression::RawInt(num) => write!(f, "{:#x}", num),
			Expression::RawString(ref string) => write!(f, "\"{}\"", string),
			Expression::Variable(Variable {ref name, .. }) => write!(f, "{}", name),
			Expression::UnaryExpr{ref value, op} => {
				if self.precedence() > value.precedence() {
					write!(f, "{}({:#x})", op, value.as_ref())
				} else {
					write!(f, "{}{:#x}", op, value.as_ref())
				}
			},
			Expression::BinaryExpr{ref lhs, ref rhs, op} => {
				let precedence = self.precedence();

				let left_expr = if precedence > lhs.precedence() {
					format!("({:#x})", lhs.as_ref())
				} else {
					format!("{:#x}", lhs.as_ref())
				};

				if op == BinaryOp::Index {
					write!(f, "{}[{:#x}]", left_expr, rhs.as_ref())
				} else {
					let right_precedence = rhs.precedence();
					let right_expr = if precedence > right_precedence {
						format!("({:#x})", rhs.as_ref())
					} else if precedence < right_precedence {
						format!("{:#x}", rhs.as_ref())
					} else {
						// Non associative
						if op == BinaryOp::Sub || op == BinaryOp::Div || op == BinaryOp::Member {
							format!("({:#x})", rhs.as_ref())
						} else {
							format!("{:#x}", rhs.as_ref())
						}
					};

					write!(f, "{}{}{}", left_expr, op, right_expr)
				}
			},
			Expression::Function(ref func_type) => write!(f, "{}", func_type),
			Expression::FunctionCall {
				ref function, option, ref args, ref extra_params, extra, ..
			} => {
				let option_str = if option == 0 { String::new() } else { format!("{{{}}}", option) };
				let extra_param_str = if extra_params.is_empty() { String::new() } else { format!("<{}>", format_list(extra_params.iter())) };
				let extra_str = if let Some(extra) = extra { format!(", {}", extra) } else { String::new() };
				write!(f, "{:#x}{}({}){}{}", function.as_ref(), option_str, format_list(args.iter()), extra_param_str, extra_str)
			},
			Expression::System(num) => write!(f, "sys_{:#x}", num),
			Expression::List(ref vec) => write!(f, "{{{}}}", format_list(vec.iter())),
			Expression::Error => write!(f, "ERROR"),

			Expression::LocalVarRef(idx) => write!(f, "local_var[{}]", idx),
			Expression::FunctionRef(idx) => write!(f, "function_{:#x}", idx),
			Expression::GlobalVarRef(idx) => write!(f, "global_var[{:#x}]", idx),
		}
	}
}

#[derive(Copy, Clone, Debug, PartialEq)]
pub enum BinaryOp {
	Add,
	Sub,
	Mul,
	Div,
	Rem,
	Eq,
	Neq,
	Gt,
	Geq,
	Lt,
	Leq,

	And,
	Or,

	BitwiseAnd,
	BitwiseOr,
	BitwiseXor,
	LeftShift,
	RightShift,
	UnsignedRightShift,

	Index,
	Member,
}


impl std::fmt::Display for BinaryOp {
	fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
		match *self {
			BinaryOp::Add => write!(f, " + "),
			BinaryOp::Sub => write!(f, " - "),
			BinaryOp::Mul => write!(f, " * "),
			BinaryOp::Div => write!(f, " / "),
			BinaryOp::Rem => write!(f, " % "),
			BinaryOp::Eq => write!(f, " == "),
			BinaryOp::Neq => write!(f, " != "),
			BinaryOp::Gt => write!(f, " > "),
			BinaryOp::Geq => write!(f, " >= "),
			BinaryOp::Lt => write!(f, " < "),
			BinaryOp::Leq => write!(f, " <= "),
			BinaryOp::And => write!(f, " && "),
			BinaryOp::Or => write!(f, " || "),
			BinaryOp::Index => write!(f, "[]"),
			BinaryOp::Member => write!(f, "."),
			BinaryOp::BitwiseAnd => write!(f, " & "),
			BinaryOp::BitwiseOr => write!(f, " | "),
			BinaryOp::BitwiseXor => write!(f, " ^ "),
			BinaryOp::LeftShift => write!(f, " << "),
			BinaryOp::RightShift => write!(f, " >> "),
			BinaryOp::UnsignedRightShift => write!(f, " >>> "),
		}
	}
}


#[derive(Copy, Clone, Debug, PartialEq)]
pub enum UnaryOp {
	// Identity,
	Negate,
	BitwiseNot,
	BooleanNot,
}

impl std::fmt::Display for UnaryOp {
	fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
		match *self {
			UnaryOp::Negate => write!(f, "-"),
			UnaryOp::BitwiseNot => write!(f, "~"),
			UnaryOp::BooleanNot => write!(f, "!"),
		}
	}
}

impl std::fmt::Display for FunctionType {
	fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
		match *self {
			FunctionType::Procedure(num) => write!(f, "PROC_{:#x}", num),
			FunctionType::Named(ref name) => write!(f, "{}", name),
		}
	}
}