#pragma once
#define _CRT_SECURE_NO_WARNINGS 
#include <cstdio>
#include "HuffmanTree.cpp"

struct BufferType
{
	char ch;//字符
	unsigned int bits;//实际比特数
};

class HuffmanCompress
{
protected:
	HuffmanTree<char, unsigned long>* pHuffmanTree;
	FILE* infp, * outfp;//输入输出文件
	BufferType buf;//字符缓存

	void Write(unsigned int bit);//向目标文件中写入一个比特
	void WriteToOutfp();//强行将字符缓存写入目标文件

public:
	HuffmanCompress();//无参构造函数
	~HuffmanCompress() {};//析构函数
	void Compress();//压缩算法
	void Decompress();//解压缩算法
};

HuffmanCompress::HuffmanCompress()
{
	Compress();
	Decompress();
}

void HuffmanCompress::Write(unsigned int bit)
{//操作结果：向目标文件中写入一个比特
	buf.bits++;//缓存比特数自增1
	buf.ch = (buf.ch << 1) | bit;//将bit加入到缓存字符区中
	if (buf.bits == 8) {//缓存区已满，写入目标文件
		fputc(buf.ch, outfp);//写入目标文件
		buf.bits = 0;//初始化bits
		buf.ch = 0;//初始化ch
	}
}

void HuffmanCompress::WriteToOutfp()
{//强行将字符缓存写入目标文件
	unsigned int len = buf.bits;//缓存实际比特数
	if (len > 0) //缓存非空，将缓存的比特个数增加到8，自动写入目标文件
		for (unsigned int i=0;i < 8 - len; i++)
			Write(0);
}

void HuffmanCompress::Compress()//对源文件中各字节（字符）进行哈夫曼编码，将编码以比特为单位写入到目标文件
{
	char infName[256], outfName[256];
	std::cout << "请输入源文件名（文件小于4GB）:";
	std::cin >> infName;
	if ((infp = fopen(infName, "rb")) == NULL)
		throw "打开源文件失败";
	fgetc(infp);
	if (feof(infp))
		throw "空源文件文件！";
	std::cout << "请输入目标文件:";
	std::cin >> outfName;
	if ((outfp = fopen(outfName, "wb")) == NULL)
		throw "打开目标文件失败";
	std::cout << "正在处理，请稍后..." << std::endl;

	const unsigned long n = 256;//n：字符个数
	char ch[n],cha;//ch[n]：字符数组
	unsigned long w[n],i,size=0;//w[n]：权
	for (int i = 0; i < n; i++) {//初始化ch[]和w[]
		ch[i] = (char)i;
		w[i] = 0;
	}
	rewind(infp);//使源文件指针指向文件开始处
	cha = fgetc(infp);
	while(!feof(infp)) {
		w[(unsigned char)cha]++;//字符cha出现频度自加1
		size++;//文件大小自加1
		cha = fgetc(infp);
	}
	if (pHuffmanTree != NULL)//释放空间
		delete[]pHuffmanTree;
	pHuffmanTree = new HuffmanTree<char, unsigned long>(ch, w, n);//生成哈夫曼树
	rewind(outfp);
	fwrite(&size, sizeof(unsigned long), 1, outfp);//使目标文件写入源文件大小
	for (i = 0; i < n; i++)//向目标文件写入字符出现频度
		fwrite(&w[i], sizeof(unsigned long), 1, outfp);
	buf.bits = 0;
	buf.ch = 0;
	rewind(infp);
	cha = fgetc(infp);
	int count = 0;
	while (!feof(infp)) {//对源文件字符进行编码，并将编码写入目标文件
		std::string strTmp = pHuffmanTree->EnCode(cha);//字符编码
		int length_strTmp = strTmp.length();
		for (i = 0; i < length_strTmp; i++) {
			if (strTmp[i] == '0')Write(0);
			else Write(1);
		}
		count+=length_strTmp;
		cha = fgetc(infp);
	}
	WriteToOutfp();//强行写入目标文件
	fclose(infp);
	fclose(outfp);
	std::cout << "处理结束."<<std::endl;
}

void HuffmanCompress::Decompress()
{//对压缩软件的各字节进行解码，并将解码后的字符写入目标文件中
	char infName[256], outfName[256];
	std::cout << "请输入压缩文件名（文件小于4GB）:";
	std::cin >> infName;
	if ((infp = fopen(infName, "rb")) == NULL)
		throw "打开压缩文件失败";
	fgetc(infp);
	if (feof(infp))
		throw "压缩文件为空！";
	std::cout << "请输入目标文件名:";
	std::cin >> outfName;
	if ((outfp = fopen(outfName, "wb")) == NULL)
		throw "打开目标文件失败";
	std::cout << "正在处理，请稍后..." << std::endl;

	const unsigned long n = 256;
	char ch[n], cha;
	unsigned long w[n], i, size = 0;

	rewind(infp);
	fread(&size, sizeof(unsigned long), 1, infp);//读取目标文件的大小
	for (i = 0; i < n; i++){
		ch[i] = (char)i;//初始化ch[i]
		fread(&w[i], sizeof(unsigned long), 1, infp);//读取字符频度
	}
	if (pHuffmanTree != NULL)
		delete[]pHuffmanTree;
	pHuffmanTree = new HuffmanTree<char, unsigned long>(ch, w, n);

	unsigned long len = 0;
	cha = fgetc(infp);
	while (!feof(infp)) {//对压缩文件字符进行解码，并将解码的字符写入目标文件
		std::string strTmp = "";//将cha转换二进制形式的串
		unsigned char c = (unsigned char)cha;//将cha转换成unsigned char类型
		for (i = 0; i < 8; i++) {//将c转换成二进制串
			if (c < 128)
				strTmp+="0";//最高位为0
			else 
				strTmp+="1";//最高位为1
			c = c << 1;//左移1位
		}
		DblLinkList<char> tmp = pHuffmanTree->DeCode(strTmp);
		int length_tmp = tmp.Length();
		std::string strTemp;
		for (int i = 1; i <= length_tmp; i++) {
			char ch;
			tmp.GetElem(i, ch);
			strTemp += ch;
		}//译码
		int length_strTemp = strTemp.length();
		for (i = 1; i <= length_strTemp; i++) {
			len++;
			fputc(strTemp[i - 1], outfp);
			if (len == size)
				break;
		}
		if (len == size)
			break;
		cha = fgetc(infp);
	}
	fclose(infp); fclose(outfp);
	std::cout << "处理结束." <<std::endl;
}

int main()
{
	HuffmanCompress test;
	return 0;
}