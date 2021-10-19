#pragma once


class CompManager
{
private:
	static int _size;
	static int genNewTypeID() { return _size++; }
public:
	static int size() { return _size; }
	template<typename T> 
	static int getTypeID()
	{
		static int typeID = genNewTypeID();
		return typeID;
	}
};
typedef CompManager CmpMngr;

struct Component
{
private:
public:
};