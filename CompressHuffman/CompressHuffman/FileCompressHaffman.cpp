#include"FileCompressHaffman.h"
#include"CreateHaffmanTree.hpp"
#include<errno.h>


void GetHaffmanCode(HaffmanTreeNode<CharInfo>* root, std::vector<CharInfo>& info)
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
			if (cur = cur->pParent->pLeft)
				info[root->_val._ch].CharCode += '0';
			else
				info[root->_val._ch].CharCode += '1';
			cur = cur->pParent;
		}
		reverse(info[root->_val._ch].CharCode.begin(), info[root->_val._ch].CharCode.end());
	}
}



void Compress(FILE* pIn, FILE* pOut, std::vector<CharInfo>& info)
{
	char pRead[1024];
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

	fclose(pIn);
	fclose(pOut);

}


void FileCompressHaffman::CompressHaffman(const std::string& Fileofpath)
{
	//1. �򿪱�ѹ���ļ�����ȡ�ļ���ÿ���ַ������ֵ��ܴ�����

	FILE * pIn = fopen(Fileofpath.c_str(), "r");
	if (pIn == nullptr)
	{
		perror("open file for read");
		return;
	}
	char pRead[1024];
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
	
	//2. ��ÿ���ַ����ֵ��ܴ���ΪȨֵ����huffman����
	CreateHaffmanTree<CharInfo> hf;
	hf.GetHaffmanTree(info);
	//3. ͨ��huffman����ȡÿ���ַ���huffman���롣
	GetHaffmanCode(hf.GetRoot(), info);
	//4. ��ȡԴ�ļ�����Դ�ļ��е�ÿ���ַ�ʹ�û�ȡ��huffman������и�д������д���д��ѹ���ļ��У�
	//ֱ���ļ�������
	FILE* pOut = fopen("2.txt", "w");
	if (nullptr == pOut)
	{
		perror("open file for write");
		return;
	}

	std::string fileName = Fileofpath.substr(Fileofpath.rfind('.'));
	fwrite(fileName.c_str(), 1, fileName.size(), pOut);
	fputc('\n', pOut);

	int count = 0;
	for (size_t i = 0; i < info.size(); ++i)
	{
		if (info[i]._count != 0)
			count++;
	}
	char buff[33];
	_itoa(count, buff, 10);
	fwrite(buff, 1, strlen(buff), pOut);
	fputc('\n', pOut);

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

	Compress(pIn, pOut, info);
}


void FileCompressHaffman::UnCompressHaffman(const std::string& Fileofpath)
{
	//����ļ��ĺ�׺
	std::string strPosFix = Fileofpath.substr(Fileofpath.rfind('.'));
	if (".hzp" != strPosFix)
	{
		cout << "ѹ���ļ���ʽ������" << endl;
		return;
	}

	//��ȡ��ѹ������Ϣ
	FILE* pIn = fopen(Fileofpath.c_str(), "r");
	if (nullptr == pIn)
	{
		cout << "���ļ�ʧ��" << endl;
		return;
	}


	//��ȡԴ�ļ���׺
	strPosFix = "";
	GetLine(pIn, strPosFix);

	string strLine;
	GetLine(pIn, strLine);
	size_t lineCount = atoi(strLine.c_str());

	//�ַ���Ϣ
	std::vector<CharInfo> info(256);
	for (size_t i = 0; i < info.size(); ++i)
	{
		info[i]._ch = i;
	}

	for (size_t i = 0; i < lineCount; ++i)
	{
		strLine = "";
		GetLine(pIn, strLine);
		info[strLine[0]]._count = atoi(strLine.c_str() + 2);
	}

	//������������
	CreateHaffmanTree<CharInfo> hf;
	hf.GetHaffmanTree(info);
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
	hf.CompressHaffman("1.txt");
}