//---------------------------------------------------------------------------
/*
	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
// Safe Object List
//---------------------------------------------------------------------------
#ifndef ObjectListH
#define ObjectListH

#include "tjs.h"
using namespace TJS;

//---------------------------------------------------------------------------
/*
	Simple implementation of auto array for objects.
	This manages neither object's construction nor destruction.
*/
//---------------------------------------------------------------------------
#define TOBJECTLIST_INC_COUNT 64
class tVoidObjectList
{
private:
	void * * Objects; // array of the object
	void * * BackupedObjects; // backuped objects
	tjs_int BackupedCount; // item count in BackupedObjects
	tjs_int Capacity; // capacity of the array
	tjs_int Count; // actual count
	tjs_int SafeLockCount; // safe lock reference count
	bool Backuped; // whether "Object" is backuped to "BackupedObject"

	void InternalAssign(const tVoidObjectList & ref)
	{
		// this internal function does not free the array previously allocated.
		// only "Objects" ( not "BackupedObjects" ) is copied.
		if(ref.Count)
		{
			Objects = (void **)TJS_malloc(ref.Count * sizeof(void *) );
			memcpy(Objects, ref.Objects, ref.Count * sizeof(void *) );
		}
		else
		{
			Objects = NULL;
		}
		Capacity = Count = ref.Count;
	}

	void Backup()
	{
		// backup current "Objects" to "BackupedObjects"
		if(Count)
		{
			BackupedObjects = (void**)TJS_malloc(Count * sizeof(void *) );
			memcpy(BackupedObjects, Objects, Count * sizeof(void *) );
		}
		else
		{
			BackupedObjects = NULL;
		}
		BackupedCount = Count;
		Backuped = true;
	}

	void Commit()
	{
		// commit the current array
		// this simply free BackupedObjects ( and its related things )
		if(BackupedObjects) TJS_free(BackupedObjects);
		BackupedObjects = NULL;
		BackupedCount = 0;
		Backuped = false;
	}

public:
	tVoidObjectList()
	{
		Objects = NULL;
		BackupedObjects = NULL;
		Capacity = 0;
		Count = 0;
		Backuped = false;
		SafeLockCount = 0;
	}

	tVoidObjectList(const tVoidObjectList & ref)
	{
		SafeLockCount = 0;
		BackupedObjects = NULL;
		Backuped = false;
		InternalAssign(ref);
	}

	~tVoidObjectList()
	{
		if(Objects) TJS_free(Objects);
		if(BackupedObjects) TJS_free(BackupedObjects);
	}

	void operator =(const tVoidObjectList & ref)
	{
		Assign(ref);
	}

	void Assign(const tVoidObjectList & ref)
	{
		// note that this function does not change any safe locking
		// effects ( locking count is not changed )
		if(Objects) TJS_free(Objects);
		InternalAssign(ref);
	}

	void SafeLock()
	{
		// lock array safely
		// safe locking is managed by reference counter;
		if(SafeLockCount == 0)
			Compact(); // this is needed
		SafeLockCount ++;
	}

	void SafeUnlock()
	{
		SafeLockCount --;
		if(SafeLockCount == 0)
			Commit();
	}

	tjs_int GetSafeLockedObjectCount() const
	{
		// this function only valid in safe locking
		if(Backuped) return BackupedCount;
		return Count;
	}

	void * GetSafeLockedObjectAt(tjs_int index) const
	{
		// this migight contain null pointer
		if(Backuped)
		{
			return BackupedObjects[index];
		}
		return Objects[index];
	}

	tjs_int GetCount() const
	{
		// note that if you want to ensure the result is
		// actual object count in the array,
		// call Compact() before GetCount().
		return Count;
	}

	tjs_int GetActualCount()
	{
		// this does Compact() before returning current Count,
		// ensuring returned count is actual object count in the array.
		Compact();
		return Count;
	}

	void SetCount(tjs_int count)
	{
		if(SafeLockCount && !Backuped) Backup();
		Reserve(count);
		Count = count;
	}

	void Reserve(tjs_int count)
	{
		if(SafeLockCount && !Backuped) Backup();
		if(Capacity < count)
		{
			Capacity = count + TOBJECTLIST_INC_COUNT;
			if(Capacity)
				Objects = (void**)TJS_realloc(Objects, Capacity * sizeof(void *));
			else
				if(Objects) TJS_free(Objects), Objects = NULL;
		}
	}

	void Compact()
	{
		// this eliminates NULL pointer from the array
		if(SafeLockCount && !Backuped) Backup();
		void **s, **d, **slim;
		slim = Objects + Count;
		s = d = Objects;
		while(s < slim)
		{
			if(*s == NULL)
			{
				s++;
				continue;
			}
			if(s != d) *d = *s;
			s++, d++;
		}
		Count = d - Objects;
		Capacity = Count;
		if(Count)
			Objects = (void**)TJS_realloc(Objects, Count * sizeof(void *));
		else
			if(Objects) TJS_free(Objects), Objects = NULL;
	}

	void * & operator [] (tjs_int index)
	{
		// this might return a null pointer
		if(SafeLockCount && !Backuped) Backup();
		return Objects[index];
	}

	void * & const operator [] (tjs_int index) const
	{
		// this might return a null pointer
		return Objects[index];
	}

	tjs_int Find(void * object) const
	{
		// find "object" from array
		// return -1 if "object" does not exist
		if(!object) return -1; // null cannot be finded
		void * const *  s = Objects;
		void * const *  slim = Objects + Count;
		while(s < slim)
		{
			if(*s == object) return s - Objects;
			s++;
		}
		return -1;
	}

	bool Add(void * object)
	{
		// add "object" to array
		// this does not allow duplicates
		if(SafeLockCount && !Backuped) Backup();
		if(!object) return false; // null cannot be added
		if(Find(object) == -1)
		{
			tjs_int orgcount = Count;
			SetCount(Count+1);
			Objects[orgcount] = object;
			return true;
		}
		return false;
	}

	bool Remove(void * object)
	{
		// remove object from array
		// (this only set the pointer to null)
		if(SafeLockCount && !Backuped) Backup();
		tjs_int index = Find(object);
		bool ret;
		if(index != -1)
		{
			Objects[index] = NULL;
			ret = true;
		}
		else
		{
			ret = false;
		}
		if(Backuped && object)
		{
			// remove also from BackupedObjects
			void * * s = BackupedObjects,
				* * slim = BackupedObjects + BackupedCount;
			while(s < slim)
			{
				if(*s == object)
				{
					*s = NULL;
					break;
				}
				s++;
			}
		}
		return ret;
	}

	void Remove(tjs_int index)
	{
		if(SafeLockCount && !Backuped) Backup();
		void * object = Objects[index];
		Objects[index] = NULL;
		if(Backuped && object)
		{
			// remove also from BackupedObjects
			void ** s = BackupedObjects,
				** slim = BackupedObjects + BackupedCount;
			while(s < slim)
			{
				if(*s == object)
				{
					*s = NULL;
					break;
				}
				s++;
			}
		}
	}
};
//---------------------------------------------------------------------------
// template wrappter class for tVoidObjectList
template <typename ObjT>
class tObjectList
{
private:
	tVoidObjectList List;

public:
	tObjectList() : List()
	{
	}

	tObjectList(const tObjectList<ObjT> & ref) : List(ref.List)
	{
	}

	~tObjectList()
	{
	}

	void operator =(const tObjectList<ObjT> & ref)
	{
		List.operator =(ref.List);
	}

	void Assign(const tObjectList<ObjT> & ref)
	{
		List.Assign(ref.List);
	}

	void SafeLock()
	{
		List.SafeLock();
	}

	void SafeUnlock()
	{
		List.SafeUnlock();
	}

	tjs_int GetSafeLockedObjectCount() const
	{
		return List.GetSafeLockedObjectCount();
	}

	ObjT * GetSafeLockedObjectAt(tjs_int index) const
	{
		return static_cast<ObjT*>(List.GetSafeLockedObjectAt(index));
	}

	tjs_int GetCount() const
	{
		return List.GetCount();
	}

	tjs_int GetActualCount()
	{
		return List.GetActualCount();
	}

	void SetCount(tjs_int count)
	{
		List.SetCount(count);
	}

	void Reserve(tjs_int count)
	{
		List.Reserve(count);
	}

	void Compact()
	{
		List.Compact();
	}

	ObjT * & operator [] (tjs_int index)
	{
		return static_cast<ObjT*>(List.operator[](index));
	}

	void * & const operator [] (tjs_int index) const
	{
		return static_cast<ObjT*>(List.operator [] const (index));
	}

	tjs_int Find(ObjT * object) const
	{
		return List.Find(static_cast<void*>(object));
	}

	bool Add(ObjT * object)
	{
		return List.Add(static_cast<void*>(object));
	}

	bool Remove(ObjT * object)
	{
		return List.Remove(static_cast<void*>(object));
	}

	void Remove(tjs_int index)
	{
		List.Remove(index);
	}
};
//---------------------------------------------------------------------------
class tVoidObjectListSafeLockHolder
{
private:
	tVoidObjectList & List;
public:
	tVoidObjectListSafeLockHolder(tVoidObjectList &list) : List(list)
	{
		List.SafeLock();
	}
	~tVoidObjectListSafeLockHolder()
	{
		List.SafeUnlock();
	}
};
//---------------------------------------------------------------------------
template <typename ObjT>
class tObjectListSafeLockHolder
{
private:
	tObjectList<ObjT> & List;
public:
	tObjectListSafeLockHolder(tObjectList<ObjT> &list) : List(list)
	{
		List.SafeLock();
	}
	~tObjectListSafeLockHolder()
	{
		List.SafeUnlock();
	}
};
//---------------------------------------------------------------------------

#endif
