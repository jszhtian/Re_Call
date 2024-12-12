#pragma once

#include "Anz.h"
#include "VarInfo.h"
#include <map>

class GlobalVarManager : private std::map<AnzUInt32, VarInfo*>
{
public:
	VarInfo* operator [] (const AnzUInt32& Index)
	{
		auto it = find(Index);
		if (it == end())
		{
			auto v = new VarInfo(0);
			insert(std::make_pair(Index, v));
			return v;
		}
		else
		{
			return it->second;
		}
	}

	VarInfo* Get(const AnzUInt32& Index)
	{
		auto it = find(Index);
		if (it == end())
		{
			auto v = new VarInfo(0);
			insert(std::make_pair(Index, v));
			return v;
		}
		else
		{
			return it->second;
		}
	}

	void Set(const AnzUInt32& Index, const VarInfo& var)
	{
		auto it = find(Index);
		if (it == end())
		{
			insert(std::make_pair(Index, new VarInfo(var)));
		}
		else
		{
			delete it->second;
			it->second = new VarInfo(var);
		}
	}

	~GlobalVarManager()
	{
		for (auto it = begin(); it != end(); it++)
			it->second->Relase();
	}
};

