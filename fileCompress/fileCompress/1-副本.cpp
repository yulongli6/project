#include"FileCompressHaffman.h"
#include"CreateHaffmanTree.hpp"
#include<errno.h>
#include<assert.h>


void FileCompressHaffman::GetHaffmanCode(HaffmanTreeNode<CharInfo>* root, std::vector<CharInfo>& info)
{
	if (nullptr == root)
		return;

	GetHaffmanCode(root->pLeft, info);
	GetHaffmanCode(root->pRight, info);

	if (root->pLeft == nullptr && root->pRight == nullptr)
	{
		HaffmanTreeNode<CharInfo>* cur = root;
		while (cur->pParent != nullptr)
		{
			if (cur == cur->pParent->pLeft)
				info[root->_val._ch].CharCode += '0';
			else
				info[root->_val._ch].CharCode += '1';
			cur = cur->pParent;
		}
		reverse(info[root->_val._ch].CharCode.begin(), info[root->_val._ch].CharCode.end());
	}
}



void FileCompressHaffman::CompressFile(FILE* pIn, FILE* pOut, std::vector<CharInfo>& info)
{
	UCH pRead[1024];
	fseek(pIn, 0, SEEK_SET);
	char ch = 0;
	char bitCount = 0;
	while (true)
	{
		
		size_t rSize = fread(pRead, 1, 1024, pIn);
		if (rSize == 0)
			break;
		for (size_t i = 0; i < rSize; ++i)
		{
			std::string strCode = info[pRead[i]].CharCode;
			for (size_t j = 0; j < strCode.size(); ++j)
			{
				ch <<= 1;
				if ('1' == strCode[j])
					ch |= 1;
				bitCount++;
				if (8 == bitCount)
				{
					fputc(ch, pOut);
					bitCount = 0;
				}
				
			}
		}

	}
	
	if (bitCount < 8 && bitCount > 0)
	{
		ch <<= (8 - bitCount);
		fputc(ch, pOut);
	}

	

}


void FileCompressHaffman::CompressHaffman(const std::string& Fileofpath)
{
	//1. 打开被压缩文件，获取文件中每个字符串出现的总次数。

	FILE * pIn = fopen(Fileofpath.c_str(), "r");
	if (pIn == nullptr)
	{
		perror("open file for read");
		return;
	}
	UCH pRead[1024];
	std::vector<CharInfo> info(256);
	for (size_t i = 0; i < info.size(); ++i)
	{
		info[i]._ch = i;
	}

	while (true)
	{
		size_t rSize = fread(pRead, 1, 1024, pIn);
		if (0 == rSize)
		{
			break;
		}

		for (size_t i = 0; i < rSize; ++i)
		{
			info[pRead[i]]._count++;
		}
	}
	
	//2. 以每个字符出现的总次数为权值构建huffman树。
	CreateHaffmanTree<CharInfo> hf;
	hf.GetHaffmanTree(info);
	//3. 通过huffman树获取每个字符的huffman编码。
	GetHaffmanCode(hf.GetRoot(), info);
	//4. 读取源文件，对源文件中的每个字符使用获取的huffman编码进行改写，
	//将改写结果写到压缩文件中，直到文件结束。
	
	string FileName = Fileofpath.substr(0,Fileofpath.find('.'));
	FileName += ".hzp";

	FILE* pOut = fopen(FileName.c_str(), "w");
	if (nullptr == pOut)
	{
		perror("open file for write");
		return;
	}

	//----------------------压缩文件头部----------------------------

	//写入源文件的后缀信息(.txt)和换行
	std::string fileName = Fileofpath.substr(Fileofpath.rfind('.'));
	fwrite(fileName.c_str(), 1, fileName.size(), pOut);
	fputc('\n', pOut);

	//统计源文件字符的种类
	int count = 0;
	for (size_t i = 0; i < info.size(); ++i)
	{
		if (info[i]._count != 0)
			count++;
	}


	//把统计的种类换算为字符串写入压缩文件
	char buff[33];
	_itoa(count, buff, 10);
	fwrite(buff, 1, strlen(buff), pOut);
	fputc('\n', pOut);


	//把字符和出现的次数写入压缩文件，每个字符占一行
	//A,1
	//B,3
	//C,5
	//D,7
	for (size_t i = 0; i < info.size(); ++i)
	{
		if (info[i]._count != 0)
		{
			char buff1[33];
			_itoa(info[i]._count, buff1, 10);
			fputc(info[i]._ch, pOut);
			fputc(',', pOut);
			fwrite(buff1, 1, strlen(buff1), pOut);
			fputc('\n', pOut);
		} 
	}

	//把正文的压缩写入文件
	FileCompressHaffman fch;
	fch.CompressFile(pIn, pOut, info);

	fclose(pIn);
	fclose(pOut);

}


void FileCompressHaffman::UNCompressHaffman(const std::string& Fileofpath)
{
	//检测文件的后缀
	std::string strPosFix = Fileofpath.substr(Fileofpath.rfind('.'));
	if (".hzp" != strPosFix)
	{
		cout << "压缩文件格式有问题" << endl;
		return;
	}

	//获取解压缩的信息
	FILE* pIn = fopen(Fileofpath.c_str(), "r");
	if (nullptr == pIn)
	{
		cout << "打开文件失败" << endl;
		return;
	}


	//获取源文件后缀
	strPosFix = "";
	GetLine(pIn, strPosFix);

	string strLine;
	GetLine(pIn, strLine);
	size_t lineCount = atoi(strLine.c_str());

	//字符信息
	std::vector<CharInfo> info(256);
	for (size_t i = 0; i < info.size(); ++i)
	{
		info[i]._ch = i;
	}

	for (size_t i = 0; i < lineCount; ++i)
	{
		strLine = "";
		GetLine(pIn, strLine);
		if (strLine.empty())
		{
			strLine += '\n';
			GetLine(pIn, strLine);
		}
		info[(UCH)strLine[0]]._count = atoi(strLine.c_str() + 2);
	}

	//创建哈夫曼树
	CreateHaffmanTree<CharInfo> hf;
	hf.GetHaffmanTree(info);

	//解压缩
	string strUNComFile = "2";
	strUNComFile += strPosFix;
	FILE* pOut = fopen(strUNComFile.c_str(), "w");
	assert(pOut);

	char rdBuff[1024];
	HaffmanTreeNode<CharInfo>* cur = hf.GetRoot();
	char pos = 7;
	size_t FileSize = hf.GetRoot()->_val._count;
	while (true)
	{
		size_t rdSize = fread(rdBuff, 1, 1024, pIn);
		if (rdSize == 0)
			break;

		for (size_t i = 0; i < rdSize; ++i)
		{
			pos = 7;
			for (int j = 0; j < 8; j++)
			{
				if (rdBuff[i] & (1 << pos))
					cur = cur->pRight;
				else
					cur = cur->pLeft;

				if (nullptr == cur->pLeft && nullptr == cur->pRight)
				{
					fputc(cur->_val._ch, pOut);
					cur = hf.GetRoot();

					FileSize--;
					if (0 == FileSize)
						break;
				}
				pos--;
			}
		}
	}

	fclose(pIn);
	fclose(pOut);
}

void FileCompressHaffman::GetLine(FILE* pIn, std::string& strAceve)
{
	while (!feof(pIn))
	{
		char ch = fgetc(pIn);
		if ('\n' == ch)
			return;
		strAceve += ch;
	}
}

int main()
{
	FileCompressHaffman hf;
	//hf.CompressHaffman("1.txt");
	hf.UNCompressHaffman("1.hzp");
}