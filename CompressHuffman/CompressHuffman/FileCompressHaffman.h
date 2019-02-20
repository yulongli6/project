#pragma once
#include<string>
#include<iostream>
#include"CreateHaffmanTree.hpp"

struct CharInfo
{
	CharInfo(int count = 0)
	:_count(count)
	{}

	char _ch;
	long long _count;
	std::string CharCode;
};

class FileCompressHaffman
{
public:
	void CompressHaffman(const std::string& Fileofpath);
	void UnCompressHaffman();
private:
	void GetHaffmanCode(HaffmanTreeNode<CharInfo>* root, std::vector<CharInfo>& info);
};
