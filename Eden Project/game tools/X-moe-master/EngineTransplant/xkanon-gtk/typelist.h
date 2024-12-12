#ifndef __TYPELIST__H__
#define __TYPELIST__H__
#include<vector>

class NullType {};
struct EmptyType {};
template<int v>
struct Int2Type {
	enum {value = v};
};

struct selno_list {
	std::vector<int> data;
	void p(int n) { data.push_back(n);} // alias
	selno_list() {
	}
	selno_list(int a0) {
		p(a0); 
	}
	selno_list(int a0,int a1) {
		p(a0); p(a1); 
	}
	selno_list(int a0,int a1,int a2) {
		p(a0); p(a1); p(a2); 
	}
	selno_list(int a0,int a1,int a2,int a3) {
		p(a0); p(a1); p(a2); p(a3); 
	}
	selno_list(int a0,int a1,int a2,int a3,int a4) {
		p(a0); p(a1); p(a2); p(a3); p(a4); 
	}
	selno_list(int a0,int a1,int a2,int a3,int a4,int a5) {
		p(a0); p(a1); p(a2); p(a3); p(a4); p(a5); 
	}
	selno_list(int a0,int a1,int a2,int a3,int a4,int a5,int a6) {
		p(a0); p(a1); p(a2); p(a3); p(a4); p(a5); p(a6); 
	}
	selno_list(int a0,int a1,int a2,int a3,int a4,int a5,int a6,int a7) {
		p(a0); p(a1); p(a2); p(a3); p(a4); p(a5); p(a6); p(a7); 
	}
	selno_list(int a0,int a1,int a2,int a3,int a4,int a5,int a6,int a7,int a8) {
		p(a0); p(a1); p(a2); p(a3); p(a4); p(a5); p(a6); p(a7); p(a8); 
	}
	selno_list(int a0,int a1,int a2,int a3,int a4,int a5,int a6,int a7,int a8,int a9) {
		p(a0); p(a1); p(a2); p(a3); p(a4); p(a5); p(a6); p(a7); p(a8); p(a9);
	}
};
#define TLI1(a0) selno_list(a0)
#define TLI2(a0,a1) selno_list(a0,a1)
#define TLI3(a0,a1,a2) selno_list(a0,a1,a2)
#define TLI4(a0,a1,a2,a3) selno_list(a0,a1,a2,a3)
#define TLI5(a0,a1,a2,a3,a4) selno_list(a0,a1,a2,a3,a4)
#define TLI6(a0,a1,a2,a3,a4,a5) selno_list(a0,a1,a2,a3,a4,a5)
#define TLI7(a0,a1,a2,a3,a4,a5,a6) selno_list(a0,a1,a2,a3,a4,a5,a6)
#define TLI8(a0,a1,a2,a3,a4,a5,a6,a7) selno_list(a0,a1,a2,a3,a4,a5,a6,a7)
#define TLI9(a0,a1,a2,a3,a4,a5,a6,a7,a8) selno_list(a0,a1,a2,a3,a4,a5,a6,a7,a8)
#define TLI10(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9) selno_list(a0,a1,a2,a3,a4,a5,a6,a7,a8,a9)

#endif
