#include "stdafx.h"
#include "MalieCHS.h"


CMalieCHS::CMalieCHS(vector<wstring> & filelist)
{
	for each (auto && file in filelist)
	{
		cout << "ReadFile:" << wtocGBK(file.c_str()) << endl;
		CMojiCHS moji(file.c_str());
		auto && x = moji.GetDB();
		db.insert(x.begin(), x.end());
		auto&& size = db.size();
		cout << "GetSize:" << size << "\n" << endl;
	}
}


CMalieCHS::~CMalieCHS()
{
}
