#include"Lz77.h"
#include"FileCompressHaffman.h"

void fileCompress(const string& filePath)
{
	Lz77 lz;
	string prefix = lz.CompressFile(filePath);
	if (prefix == "")
		return;
	FileCompressHaffman hf;
	hf.CompressHaffman(prefix);
}

void file_UNcompress(const std::string& filePath)
{
	FileCompressHaffman hf;
	string prefix = hf.UNCompressHaffman(filePath);
	if (prefix == "")
		return;
	Lz77 lz;
	lz.UNCompressFile(prefix);
}

int main()
{
	fileCompress("814888piCEgQ.mp4");
	//file_UNcompress("IMG_9500À¶._gzip");
	return 0;
}