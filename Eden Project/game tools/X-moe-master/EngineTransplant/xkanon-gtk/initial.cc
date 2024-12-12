/*  initial.cc  : Initial class の実装
 */

/*
 *
 *  Copyright (C) 2000-   Kazunori Ueno(JAGARL) <jagarl@creator.club.ne.jp>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
*/
#include<algorithm>
#include"initial.h"

inline bool less_iiptr(InitInstanceBase* const& p1, InitInstanceBase* const& p2) {
	return (p1->priority_) < (p2->priority_);
}
Initialize* Initialize::instance = 0;
Initialize& Initialize::Instance(void) {
	if (! instance) {
		instance = new Initialize;
	}
	return *instance;
}
void Initialize::Add(InitInstanceBase* datum) {
	Instance().array.push_back(datum);
}

void Initialize::Exec(void) {
	ArrayType& funcs = Instance().array;
	sort(funcs.begin(), funcs.end(), less_iiptr);
	ArrayType::iterator it;
	for (it = funcs.begin(); it != funcs.end(); it++) {
		(*it)->exec();
	}
}

#if 0 /* テスト用コード */

#include<functional>
#include<stdio.h>
void a(void) {
	printf("a\n");
}
void b(void) {
	printf("b\n");
}
int c(void) {
	printf("c\n");
}
int d(int n) {
	printf("d%d\n",n);
}
struct A {
	inline void operator () (void){
		printf("A\n");
	}
};
struct B : unary_function<int,void> {
	inline void operator () (int a) const{
		printf("B %d\n",a);
	}
};

Init init0(20,a);
Init init1(10,b);
Init init2(30,c);
Init init3(55,bind0th(ptr_fun(d),10));
Init init4(30,c);
Init init5(60,bind0th(B(),20));
int main(void) {
	Initialize::Exec();
}
#endif /* テスト用コード終わり */
