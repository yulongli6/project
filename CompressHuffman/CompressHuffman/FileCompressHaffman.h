#pragma once
#include<string>
#include<vector>
#include<iostream>
#include"CreateHaffmanTree.hpp"
using namespace std;

struct CharInfo
{
	CharInfo(int count = 0)
	:_count(count)
	{}

	CharInfo operator+(const CharInfo& info)
	{
		CharInfo temp;
		temp._count = this->_count + info._count;
		return temp;
	}

	char _ch;
	long long _count;
	string CharCode;
};

class FileCompressHaffman
{
public:
	void CompressHaffman(const string& Fileofpath);
	void UnCompressHaffman(const string& Fileofpath);
private:
	void GetHaffmanCode(HaffmanTreeNode<CharInfo>* root, vector<CharInfo>& info);
	void GetLine(FILE* pIn, string& strAceve);
};
