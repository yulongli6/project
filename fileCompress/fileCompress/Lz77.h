#pragma once
#include"HashTable.h"
#include<string>

class Lz77
{
public:
	Lz77();
	~Lz77();

	std::string CompressFile(const std::string& fileNamePath);
	void UNCompressFile(const std::string& fileNamePath);

private:
	UCH maxLongMatch(USH matchHead, USH& curMatchDist);
	void WriteFlag(FILE* pOutF, UCH& chFlag, UCH& bitCount, bool IsChar);
	void FillWindow(FILE* pIn);
private:
	UCH* _pWin;
	HashTable _ht;
	size_t _start;
	size_t _lookAhead;
};