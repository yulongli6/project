#pragma once
#include<string>
#include<vector>
#include<iostream>
#include"CreateHaffmanTree.hpp"
using namespace std;

typedef unsigned char UCH;


//权值的信息：字符、编码、统计个数
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

	UCH _ch;
	long long _count;
	string CharCode;
};

class FileCompressHaffman
{
public:
	void CompressHaffman(const string& Fileofpath);
	string UNCompressHaffman(const string& Fileofpath);
	void CompressFile(FILE* pIn, FILE* pOut, std::vector<CharInfo>& info);
private:
	void GetHaffmanCode(HaffmanTreeNode<CharInfo>* root, vector<CharInfo>& info);
};
