#include"HashTable.h"
#include<string>

const USH HASH_BITS = 15;
//把1左移15位:1000 0000 0000 0000  2^15 = 32k
const USH HASH_SIZE = (1 << HASH_BITS);
//高位清成0，防止越界 0111 1111 1111 1111
const USH HASH_MASK = HASH_SIZE - 1;

HashTable::HashTable(size_t size)
    : _prev(new USH[size * 2])
    , _head(_prev + size)
    , _hashSize(size * 2)
{
	memset(_head, 0, sizeof(USH)* size);
}


HashTable::~HashTable()
{
	if (_prev)
	{
		delete[] _prev;
		_prev = _head = nullptr;
	}
}



void HashTable::HashFunc(USH& hashAddr, UCH ch)
{
	hashAddr = (((hashAddr) << H_SHIFT()) ^ (ch)) & HASH_MASK;
}


USH HashTable::H_SHIFT() 
{ 
	return (HASH_BITS + MIN_MATCH - 1) / MIN_MATCH;
}


void HashTable::Insert(USH& hashAddr, UCH ch, USH pos, USH& matchHead)
{
	HashFunc(hashAddr, ch);

	_prev[pos & HASH_MASK] = _head[hashAddr];
	matchHead = _head[hashAddr];
	_head[hashAddr] = pos;
}


USH HashTable::GetNextList(USH& matchHead)
{
	return _prev[matchHead];
}


void HashTable::Update()
{
	//将右窗口的匹配位置更新到左窗口
	//更新head
	for (size_t i = 0; i < HASH_SIZE; i++)
	{
		if (_head[i] >= WSIZE)
			_head[i] -= WSIZE;
		else
			_head[i] = 0;
	}

	//更新prev
	for (size_t i = 0; i < WSIZE; ++i)
	{
		if (_prev[i] >= WSIZE)
			_prev[i] -= WSIZE;
		else
			_prev[i] = 0;
	}
}













