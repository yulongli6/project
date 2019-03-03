#pragma once
#include<string>

typedef unsigned short USH;
typedef unsigned char UCH;
typedef unsigned long long ULL;


static size_t WSIZE = 32 * 1024;
static size_t MIN_MATCH = 3;
static size_t MAX_MATCH = 255 + 3;



void GetLine(FILE* pIn, std::string& strAceve)
{
	while (!feof(pIn))
	{
		char ch = fgetc(pIn);
		if ('\n' == ch)
			return;
		strAceve += ch;
	}
}
