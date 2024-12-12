use ::{Variable, VariableType};
use ::Expression;

#[derive(Clone)]
pub struct ProgStack {
	values: Vec<Expression>,
	frames: Vec<usize>,
}

impl ProgStack {
	pub fn new() -> Self {
		ProgStack {
			values: Vec::new(),
			frames: Vec::new(),
		}
	}

	pub fn pop(&mut self, var_type: VariableType) -> Expression {
		let value = self.values.pop().unwrap();
		let actual_type = value.get_type();

		if actual_type != var_type && var_type != VariableType::Unknown {
			if actual_type == VariableType::Unknown {
				//debug!("Filling in type: {} is {}", value, var_type);
				value
			} else {
				error!("{} - Expected {:?}, got {:?}", value, var_type, actual_type);
				Expression::Error
			}
		} else {
			value
		}
	}

	fn peek_type(&self) -> Option<VariableType> {
		self.values.last().map(|value| value.get_type())
	}

	pub fn push(&mut self, value: Expression) {
		self.values.push(value);
	}

	pub fn open_frame(&mut self) {
		self.frames.push(self.values.len());
	}

	pub fn dup_frame(&mut self) {
		let frame_begin: usize = *self.frames.last().unwrap();
		self.frames.push(self.values.len());

		let mut dup = (&self.values[frame_begin..]).to_vec();
		self.values.append(&mut dup);
	}

	// TODO: better error handling - possible return Result<Option<Expression>> or even Result<Expression>
	pub fn handle_frame(&mut self) -> Option<Expression> {
		let depth = self.frames.pop().unwrap();
		let frame = self.values.split_off(depth);

		let mut iter = frame.into_iter();
		let mut value = if let Some(Expression::RawInt(value)) =  iter.next() {
			let (value_type, index) = (value >> 24, (value & 0x00FFFFFF) as usize);
			match value_type {
				0x00 => {
					// TODO: figure out a better place to handle this
					if index == 0x53 {
						if let Some(Expression::RawInt(store)) = iter.next() {
							let (store_type, store_index) = (store >> 24, (store & 0x00FFFFFF) as usize);
							match store_type {
								0x00 => Variable {
									name: format!("local_data_{}", store_index),
									var_type: VariableType::Unknown,
								}.to_expression(),
								0x7D => Expression::LocalVarRef(store_index),
								_ => panic!("Unexpected store for 0x53: {}", store)
							}
						} else {
							Expression::System(0x53)
						}
					} else {
						Expression::System(index as i32)
					}
				},
				0x7E => Expression::FunctionRef(index),
				0x7F => Expression::GlobalVarRef(index),
				_ => Expression::System(value as i32)
			}
		} else {
			return None;
		};

		while let Some(expr) = iter.next() {
			if let Expression::RawInt(-1) = expr {
				let rhs = iter.next().unwrap_or_else(|| {
					error!("No index for {}", value);
					Expression::Error
				});
				value = Expression::index(value, rhs);
			} else {
				value = Expression::member(value, expr);
			}
		}

		Some(value)
	}

	pub fn get_value(&mut self, var_type: VariableType) -> Expression {
		match var_type {
			VariableType::Int | VariableType::Str => self.pop(var_type),
			VariableType::IntRef | VariableType::StrRef | VariableType::IntListRef | VariableType::StrListRef =>
				if self.peek_type() == Some(var_type) {
					self.pop(var_type)
				} else {
					self.handle_frame().unwrap()
				},
			VariableType::IntList(_) | VariableType::StrList(_) => self.pop(var_type),

			VariableType::Void => panic!("Attempting to get a void!"),
			VariableType::Error => Expression::Error,
			VariableType::Unknown => self.pop(var_type),


			VariableType::Obj => {
				let value = self.pop(VariableType::Unknown);
				if value.get_type() == VariableType::Obj {
					value
				} else {
					// Assume it is integer and index of obj list
					if let Expression::RawInt(-1) = self.pop(VariableType::Int) {} else {
						panic!();
					}
					let list = self.get_value(VariableType::ObjList(0));
					Expression::index(list, value)
				}
			}
			VariableType::ObjList(_) => {
				let value = self.pop(VariableType::Unknown);
				if let VariableType::ObjList(_) = value.get_type() {
					value
				} else {
					// Assume it is integer and member of stage element
					let stage_elem = self.get_value(VariableType::StageElem);
					Expression::member(stage_elem, value)
				}
			},
			// TODO: handle stage_elem_list
			VariableType::StageElem => {
				if self.peek_type() == Some(VariableType::StageElem) {
					self.pop(VariableType::StageElem)
				} else {
					self.handle_frame().unwrap()
				}
			},
		}
	}
}