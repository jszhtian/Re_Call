
struct T1
{
	tTJSString name;
	std::unordered_map<K2,T2> files; // ?
}

struct T // domain
{
	tTJSString domain; //?
	std::unordered_map<K1,T1> paths;
}

class CompoundStorageMediaFS
{
	std::unordered_map<K,T> FileTable;
}


