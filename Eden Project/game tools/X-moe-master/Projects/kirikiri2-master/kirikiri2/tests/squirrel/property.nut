// �O���[�o���ϐ�
gname <- "global";

class PropertyTest extends Object {

	_name = "PropertyTest";
	
	constructor() {
		::Object.constructor();
	}

	function getName() {
		return _name;
	}

	function setName(value) {
		_name = value;
	}

	function test() {

		// �v���p�e�B���Q��
		name  = "name";
		print("name:" + name);
		
		// �O���[�o���ϐ����Q��
		gname = "gname";
		print("gname:" + gname);

		// ���݂��Ȃ��ϐ����Q��
		try {
			xname = "aaaa";
		} catch(e) {
			print(e);
		}
		try {
			print("xname:" + xname);
		} catch(e) {
			print(e);
		}
	}
};

local ptest = PropertyTest();
ptest.test();
