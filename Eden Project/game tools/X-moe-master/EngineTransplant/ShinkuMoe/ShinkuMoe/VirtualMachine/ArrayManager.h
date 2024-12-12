#pragma once

#include <map>
#include "Anz.h"
#include <vector>
#include "VarInfo.h"


#ifndef MAX
#define MAX(a, b) a>b?a:b
#endif

//should be USHORT or UCHAR
class ArrayManager : private std::map<AnzUInt, std::vector<VarInfo*>*>
{ 
public:
	VarInfo* Get(const AnzUInt& ArrayId, const AnzUInt& Index)
	{
		auto it = find(ArrayId);
		
		if (it != end())
		{
			//auto allocate
			if (it->second->size() - 1 < Index)
			{
				for (AnzUInt i = it->second->size(); i < Index + 1; i++)
				{
					it->second->push_back(new VarInfo(0));
				}
				return it->second->at(Index);
			}
			else
			{
				return it->second->at(Index);
			}
		}
		else
		{
			auto v = new std::vector<VarInfo*>;
			//auto insert ?
			insert(std::make_pair(ArrayId, v));

			//20 is big enough for FVP?
			for (AnzUInt i = 0; i < MAX(Index + 1, 20); i++)
			{
				v->push_back(new VarInfo(0));
			}
			return v->at[Index];
		}
	}

	void Set(const AnzUInt& ArrayId, const AnzUInt& Index, VarInfo& v)
	{
		auto it = find(ArrayId);

		if (it != end())
		{
			if (it->second->size() < Index + 1)
			{
				for (AnzUInt i = 0; i < Index + 1; i++)
				{
					it->second->push_back(new VarInfo(0));
				}
				
				(*(*(it->second))[Index]) = v;
			}
			else
			{
				(*(*(it->second))[Index]) = v;
			}
		}
		else
		{
			auto vec = new std::vector<VarInfo*>;
			//auto insert ?
			insert(std::make_pair(ArrayId, vec));

			//20 is big enough for FVP?
			for (AnzUInt i = 0; i < MAX(Index + 1, 20); i++)
			{
				vec->push_back(new VarInfo(0));
			}
			delete (*vec)[Index];
			(*vec)[Index] = new VarInfo(v);
		}
	}
};
