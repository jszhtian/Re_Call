#include<fstream>
#include<iostream>
#include<string>
#include<iomanip>
#include<vector>
#include<io.h>
#include<istream>

using namespace std;

void getFiles(string path, vector<string>& files, vector<string> &ownname) {  //files存储文件的路径及名称，ownname只存储文件的名称
	long hFile = 0;  //文件句柄  
	struct _finddata_t fileinfo;  //文件信息
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
		do {  //如果是目录,迭代之，如果不是,加入列表  	
			if ((fileinfo.attrib &  _A_SUBDIR)) {
				//if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)getFiles( p.assign(path).append("\\").append(fileinfo.name), files, ownname );
			}
			else {
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				ownname.push_back(fileinfo.name);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void Deal_Txt(string name) {  //\r 0x000D，\n 0x000A，语句左括号，0x300C，将原始文件过滤成翻译文件和额外文件
	ifstream in;
	ofstream out1, out2;
	in.open(".\\test" + name, ios::binary);  //设置用于读取的原始脚本文件目录
	out1.open(".\\test" + name + "trans.txt", ios::binary);  //设置输出用于翻译文件的目录
	out2.open(".\\test" + name + "extra.txt", ios::binary);  //设置输出复原用额外文件的目录
	in.seekg(0, ios::end);
	int length = in.tellg();
	in.seekg(2, ios::beg);
	wchar_t mid1;
	bool hav = false;
	int ci = 0, bysi = 2;
	out1.write("\xFF\xFE", 2);
	out2.write("\xFF\xFE", 2);
	vector<wchar_t> vwc;
	in.read((char*)(&mid1), 2);
	bysi += 2;
	bool fir = true;
	if ((mid1 >= 0x0061 && mid1 <= 0x007A) || (mid1 >= 0x0041 && mid1 <= 0x005A) || mid1 == 0x000D || mid1 == 0x005F) {  //过滤脚本回车
		wchar_t mmidd = 0x0030 + ci;
		out2.write((char*)(&mmidd), 2);
		out2.write("\x20\x00", 2);
		while (mid1 != 0x000D) {
			out2.write((char*)(&mid1), 2);
			in.read((char*)(&mid1), 2);
			bysi += 2;
		}
		out2.write("\x0D\x00", 2);
		out2.write("\x0A\x00", 2);
		hav = true;
	}
	else {
		while (mid1 != 0x000D) {
			out1.write((char*)(&mid1), 2);
			//vwc.push_back(mid1);
			in.read((char*)(&mid1), 2);
			bysi += 2;
		}
		fir = false;
	}
	in.read((char*)(&mid1), 2);
	ci++;
	bysi += 2;
	while (bysi<length) {
		hav = false;
		in.read((char*)(&mid1), 2);
		bysi += 2;
		if ((mid1 >= 0x0061 && mid1 <= 0x007A) || (mid1 >= 0x0041 && mid1 <= 0x005A) || mid1 == 0x000D || mid1 == 0x005F) {  //过滤脚本回车
			wchar_t mmidd = 0x0030 + ci;
			out2.write((char*)(&mmidd), 2);
			out2.write("\x20\x00", 2);
			while (mid1 != 0x000D) {
				out2.write((char*)(&mid1), 2);
				in.read((char*)(&mid1), 2);
				bysi += 2;
			}
			out2.write("\x0D\x00", 2);
			out2.write("\x0A\x00", 2);
			hav = true;
		}
		else {
			while (mid1 != 0x000D) {
				//out1.write((char*)(&mid1), 2);
				vwc.push_back(mid1);
				if (mid1 == 0x300C) {
					hav = true;
				}
				in.read((char*)(&mid1), 2);
				bysi += 2;
			}
		}
		in.read((char*)(&mid1), 2);
		bysi += 2;
		if (!hav && !fir) {
			out1.write("\x0D\x00", 2);
			out1.write("\x0A\x00", 2);
		}
		for (int i = 0; i < vwc.size(); ++i) {
			out1.write((char*)(&vwc[i]), 2);
		}
		if (fir) {
			fir = false;
		}
		vwc.clear();
		ci++;
	}
	out1.write("\x0D\x00", 2);
	out1.write("\x0A\x00", 2);
	in.close();
	out1.close();
	out2.close();
	cout << "Ttx deal finish" << endl;
}

void Restore_Txt(string name) {  //将翻译文件和额外文件合并复原成脚本文件
	ifstream in1, in2;
	in1.open("D:\\OutTrans\\" + name + "trans.txt", ios::binary);  //设置输出用于翻译文件的目录
	in2.open("D:\\OutExtra\\" + name + "extra.txt", ios::binary);  //设置输出复原用额外文件的目录
	ofstream out;
	out.open("D:\\Script\\" + name, ios::binary);  //设置用于读取的原始脚本文件目录
	in1.seekg(0, ios::end);
	in2.seekg(0, ios::end);
	int length1 = in1.tellg();
	int length2 = in2.tellg();
	in1.seekg(2, ios::beg);
	in2.seekg(2, ios::beg);
	wchar_t mid1, mid2;
	bool hav = false;
	int ci = 0, bysi1 = 2, bysi2 = 2;
	out.write("\xFF\xFE", 2);
	in2.read((char*)(&mid2), 2);
	bysi2 += 2;
	while (bysi1 < length1 || bysi2 < length2) {
		while (bysi2 < length2&&mid2 - 0x0030 == ci) {
			in2.read((char*)(&mid2), 2);
			in2.read((char*)(&mid2), 2);
			bysi2 += 4;
			while (mid2 != 0x000A) {
				out.write((char*)(&mid2), 2);
				in2.read((char*)(&mid2), 2);
				bysi2 += 2;
				if (bysi2 >= length2) {
					break;
				}
			}
			out.write((char*)(&mid2), 2);
			ci++;
			if (bysi2 < length2) {
				in2.read((char*)(&mid2), 2);
				bysi2 += 2;
			}
		}
		if (bysi1<length1) {
			in1.read((char*)(&mid1), 2);
			bysi1 += 2;
			while (mid1 != 0x000A) {
				if (mid1 == 0x300C) {
					out.write("\x0D\x00", 2);
					out.write("\x0A\x00", 2);
					ci++;
					while (bysi2 < length2&&mid2 - 0x0030 == ci) {
						in2.read((char*)(&mid2), 2);
						in2.read((char*)(&mid2), 2);
						bysi2 += 4;
						while (mid2 != 0x000A) {
							out.write((char*)(&mid2), 2);
							in2.read((char*)(&mid2), 2);
							bysi2 += 2;
							if (bysi2 >= length2) {
								break;
							}
						}
						out.write((char*)(&mid2), 2);
						ci++;
						if (bysi2 < length2) {
							in2.read((char*)(&mid2), 2);
							bysi2 += 2;
						}
					}
				}
				out.write((char*)(&mid1), 2);
				in1.read((char*)(&mid1), 2);
				bysi1 += 2;
				if (bysi1 >= length1) {
					break;
				}
			}
			out.write((char*)(&mid1), 2);
			ci++;
		}
	}
	in1.close();
	in2.close();
	out.close();
	cout << "Txt restore finish" << endl;
}

void Change_Name() {  //批量更改文件名称，主要是更改后缀
	vector<string> files;
	vector<string> ownname;
	string temp = "H:\\\\Script\\";  //需要批量更改名称的目录
	getFiles("H:\\Script", files, ownname);
	string mid, new_mid;
	int size = files.size();
	cout << size << endl;
	for (int i = 0; i < size; ++i) {
		mid = ownname.at(i);
		new_mid = mid.substr(0, mid.size() - 4);  //设置文件后缀长度,减几就是将文件名长度减少几位
		new_mid = temp + new_mid;
		mid = temp + mid;
		cout << new_mid << endl;
		cout << rename(mid.c_str(), new_mid.c_str()) << endl;
	}
	system("pause");
}

void Deal_Do() {  //执行将原始文件过滤成翻译文件和额外文件操作
	vector<string> files;
	vector<string> ownname;
	getFiles(".\\test", files, ownname);  //原始脚本文件位置
	string mid;
	int size = files.size();
	for (int i = 0; i < size; ++i) {
		mid = ownname.at(i);
		Deal_Txt(mid);
	}
}

void Restore_Do() {  //执行将翻译文件和额外文件合并复原成脚本文件操作
	vector<string> files;
	vector<string> ownname;
	getFiles(".\\test", files, ownname);  //原始脚本文件位置
	string mid;
	int size = files.size();
	for (int i = 0; i < size; ++i) {
		mid = ownname.at(i);
		Restore_Txt(mid);
	}
}

void main() {  //函数调用
			   Deal_Do();
			   //Restore_Do();
			   //Change_Name();
}