#include"Lz77.h"
#include<iostream>
#include<assert.h>
#include"Common.h"
using namespace std;


const USH MIN_LOOKAHEAD = MAX_MATCH + 1;
const USH MAX_DIST = WSIZE - MIN_LOOKAHEAD;

Lz77::Lz77()
    :_pWin(new UCH[WSIZE * 2])
    , _ht(WSIZE)
    , _start(0)
	, _lookAhead(0)
{}


Lz77::~Lz77()
{
	if (_pWin)
		delete[] _pWin;
}

std::string Lz77::CompressFile(const std::string& fileNamePath)
{
	FILE* pIn = fopen(fileNamePath.c_str(), "rb");
	if (nullptr == pIn)
	{
		cout << "fail to open file " << endl;
		return "";
	}

	//获取文件的大小
	fseek(pIn, 0, SEEK_END);
	ULL fileSize = ftell(pIn);
	fseek(pIn, 0, SEEK_SET);

	//fileSize<3;不需要压缩
	if (fileSize < 3)
	{
		fclose(pIn);
		return "";
	}


	//先读取一个缓冲区的数据
	//_lookAhead:待压缩数据
	_lookAhead = fread(_pWin, 1, 2 * WSIZE, pIn);

	//计算前两个字符的地址
	USH hashAddr = 0;
	for (size_t i = 0; i < MIN_MATCH - 1; ++i)
		_ht.HashFunc(hashAddr, _pWin[i]);

	//写源文件后缀
	std::string prefix = fileNamePath.substr(0, fileNamePath.find('.'));
	FILE* pOutD = fopen(prefix.c_str(), "wb");
	assert(pOutD);
	std::string postFix = fileNamePath.substr(fileNamePath.rfind('.'));
	fwrite(postFix.c_str(), postFix.size(), 1, pOutD);
	fputc('\n', pOutD);

	FILE* pOutF = fopen("2.lzp", "wb");
	assert(pOutF);

	//压缩
	USH matchHead = 0;
	UCH chFlag = 0;
	UCH bitCount = 0;

	while (_lookAhead)
	{
		_ht.Insert(hashAddr, _pWin[_start + 2], _start, matchHead);

		USH curMatchDist = 0;
		UCH curMatchLen = 0;
		if (matchHead && _lookAhead > MIN_LOOKAHEAD)
		{
			curMatchLen = maxLongMatch(matchHead, curMatchDist);
		}

		if (curMatchLen < MIN_MATCH)
		{
			//没有匹配
			//no match
			fputc(_pWin[_start], pOutD);
			++_start;
			--_lookAhead;

			//write flag 源字符-0标记---false
			WriteFlag(pOutF, chFlag, bitCount, false);

		}
		else
		{
			//write distance add length
			//fputc(curMatchDist, pOutD);
			fwrite(&curMatchDist, 2, 1, pOutD);
			fputc(curMatchLen, pOutD);

			//写标记    长度距离对 --  用 1 标记
			WriteFlag(pOutF, chFlag, bitCount, true);

			_lookAhead -= curMatchLen;

			//更新哈希表
			curMatchLen -= 1;
			while (curMatchLen)
			{
				++_start;
				_ht.Insert(hashAddr, _pWin[_start + 2], _pWin[_start], matchHead);
				curMatchLen--;
			}
			_start++;
		}

		
		//窗口中数据不够，向窗口中填充数据
		if (_lookAhead <= MIN_LOOKAHEAD)
			FillWindow(pIn);
	}

	if (bitCount > 0 && bitCount < 8)
	{
		chFlag <<= (8 - bitCount);
		fputc(chFlag, pOutF);
	}


	fclose(pIn);
	fclose(pOutF);

	FILE* pInF = fopen("2.lzp", "rb");
	assert(pInF);
	UCH* pReadBuff = new UCH[1024];
	size_t flagSize = 0;
	while (true)
	{
		size_t rdSize = fread(pReadBuff, 1, 1024, pInF);
		if (0 == rdSize)
			break;

		flagSize += rdSize;
		fwrite(pReadBuff, 1, rdSize, pOutD);
	}

	fclose(pInF);

	fwrite(&fileSize, sizeof(fileSize), 1, pOutD);
	fwrite(&flagSize, sizeof(flagSize), 1, pOutD);

	fclose(pOutD);
	remove("2.lzp");

	return prefix;
}


//matchHead----哈希匹配链的起始位置
UCH Lz77::maxLongMatch(USH matchHead, USH& curMatchDist)
{
	UCH curMatchLen = 0;
	UCH maxlen = 0;
	USH pos = 0;
	UCH Matchchainlen = 255;
	USH limit = _start > MAX_DIST ? _start - MAX_DIST : 0;
	do
	{
		UCH* pStart = _pWin + _start;
		//找到匹配串的位置
		UCH* pCurStart = _pWin + matchHead;
		UCH* pEnd = pStart + MAX_MATCH;
		curMatchLen = 0;

		//找单挑连的匹配长度
		while (pStart < pEnd && *pStart == *pCurStart)
		{
			pStart++;
			pCurStart++;
			curMatchLen++;
		}

		if (curMatchLen > maxlen)
		{
			maxlen = curMatchLen;
			pos = matchHead;
		}

	} while ((matchHead = _ht.GetNextList(matchHead)) >  limit && Matchchainlen--);

	curMatchDist = _start - pos;
	return maxlen;
}



void Lz77::WriteFlag(FILE* pOutF, UCH& chFlag, UCH& bitCount, bool IsChar)
{
	chFlag <<= 1;

	//检测是否为距离长度对
	if (IsChar)
		chFlag |= 1;
	bitCount++;
	if (8 == bitCount)
	{
		fputc(chFlag, pOutF);
		chFlag = 0;
		bitCount = 0;
	}
}



void Lz77::FillWindow(FILE* pIn)
{
	//滑动窗口中数据不足时
	//把右窗数据（32k）移至左窗
	if (_start >= WSIZE + MAX_DIST)
	{
		memcpy(_pWin, _pWin + WSIZE, WSIZE);
		memset(_pWin + WSIZE, 0, WSIZE);
		_start -= WSIZE;

		//更新哈希表
		_ht.Update();

		//向右窗填充数据
		if (!feof(pIn))
		{
			_lookAhead += fread(_pWin + WSIZE, 1, WSIZE, pIn);
		}
	}
}



void Lz77::UNCompressFile(const std::string& fileNamePath)
{
	//string strPostFix = fileNamePath.substr(fileNamePath.rfind('.'));
	//if (strPostFix != ".lzp")
	//{
	//	cout << "压缩文件格式不支持" << endl;
	//	return;
	//}
														  
	FILE* pInD = fopen(fileNamePath.c_str(), "rb");
	if (nullptr == pInD)
	{
		cout << "打开压缩文件失败" << endl;
		return;
	}

	//获取标记的大小
	size_t flagSize = 0;
	int offset = 0;
	offset = 0 - sizeof(flagSize);
	fseek(pInD, offset, SEEK_END);
	fread(&flagSize, sizeof(flagSize), 1, pInD);

	//获取源文件大小
	ULL fileSize = 0;
	offset = 0 - sizeof(flagSize) - sizeof(fileSize);
	fseek(pInD, offset, SEEK_END);
	fread(&fileSize, sizeof(fileSize), 1, pInD);

	//pIF作用，读取标记数据
	FILE* pInF = fopen(fileNamePath.c_str(), "rb");
	assert(pInF);
	offset = 0 - (sizeof(flagSize)+sizeof(fileSize)+flagSize);
	fseek(pInF, offset, SEEK_END);

	//解压缩文件
	fseek(pInD, 0, SEEK_SET);
	string strUNComFileName = fileNamePath;
	string strPostFix = "";
	GetLine(pInD, strPostFix);
	strUNComFileName += "-副本";
	strUNComFileName += strPostFix;
	FILE* pOut = fopen(strUNComFileName.c_str(), "wb");
	assert(pOut);

	FILE* pWin = fopen(strUNComFileName.c_str(), "rb");
	assert(pWin);

	UCH charFlag = 0;
	char bitCount = -1;
	while (fileSize)
	{
		//读取标记
		if (bitCount < 0)
		{
			charFlag = fgetc(pInF);
			bitCount = 7;
		}

		if (charFlag & (1 << bitCount))
		{
			//长度距离对
			USH dist = 0;
			fread(&dist, 2, 1, pInD);
			UCH len = fgetc(pInD);

			fflush(pOut);
			fseek(pWin, 0 - dist, SEEK_END);
			fileSize -= len;
			while (len)
			{
				UCH ch = fgetc(pWin);
				fputc(ch, pOut);
				len--;

				//压缩时可能有重叠
				fflush(pOut);
			}
			
			fseek(pOut, 0, SEEK_END);
		}
		else
		{
			UCH ch = fgetc(pInD);
			fputc(ch, pOut);
			fileSize--;
		}
		bitCount--;
	}

	fclose(pInD);
	fclose(pInF);
	fclose(pOut);
	fclose(pWin);

	remove(fileNamePath.c_str());
}