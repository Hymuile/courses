#pragma once
#define _CRT_SECURE_NO_WARNINGS 
#include <cstdio>
#include "HuffmanTree.cpp"

struct BufferType
{
	char ch;//�ַ�
	unsigned int bits;//ʵ�ʱ�����
};

class HuffmanCompress
{
protected:
	HuffmanTree<char, unsigned long>* pHuffmanTree;
	FILE* infp, * outfp;//��������ļ�
	BufferType buf;//�ַ�����

	void Write(unsigned int bit);//��Ŀ���ļ���д��һ������
	void WriteToOutfp();//ǿ�н��ַ�����д��Ŀ���ļ�

public:
	HuffmanCompress();//�޲ι��캯��
	~HuffmanCompress() {};//��������
	void Compress();//ѹ���㷨
	void Decompress();//��ѹ���㷨
};

HuffmanCompress::HuffmanCompress()
{
	Compress();
	Decompress();
}

void HuffmanCompress::Write(unsigned int bit)
{//�����������Ŀ���ļ���д��һ������
	buf.bits++;//�������������1
	buf.ch = (buf.ch << 1) | bit;//��bit���뵽�����ַ�����
	if (buf.bits == 8) {//������������д��Ŀ���ļ�
		fputc(buf.ch, outfp);//д��Ŀ���ļ�
		buf.bits = 0;//��ʼ��bits
		buf.ch = 0;//��ʼ��ch
	}
}

void HuffmanCompress::WriteToOutfp()
{//ǿ�н��ַ�����д��Ŀ���ļ�
	unsigned int len = buf.bits;//����ʵ�ʱ�����
	if (len > 0) //����ǿգ�������ı��ظ������ӵ�8���Զ�д��Ŀ���ļ�
		for (unsigned int i=0;i < 8 - len; i++)
			Write(0);
}

void HuffmanCompress::Compress()//��Դ�ļ��и��ֽڣ��ַ������й��������룬�������Ա���Ϊ��λд�뵽Ŀ���ļ�
{
	char infName[256], outfName[256];
	std::cout << "������Դ�ļ������ļ�С��4GB��:";
	std::cin >> infName;
	if ((infp = fopen(infName, "rb")) == NULL)
		throw "��Դ�ļ�ʧ��";
	fgetc(infp);
	if (feof(infp))
		throw "��Դ�ļ��ļ���";
	std::cout << "������Ŀ���ļ�:";
	std::cin >> outfName;
	if ((outfp = fopen(outfName, "wb")) == NULL)
		throw "��Ŀ���ļ�ʧ��";
	std::cout << "���ڴ������Ժ�..." << std::endl;

	const unsigned long n = 256;//n���ַ�����
	char ch[n],cha;//ch[n]���ַ�����
	unsigned long w[n],i,size=0;//w[n]��Ȩ
	for (int i = 0; i < n; i++) {//��ʼ��ch[]��w[]
		ch[i] = (char)i;
		w[i] = 0;
	}
	rewind(infp);//ʹԴ�ļ�ָ��ָ���ļ���ʼ��
	cha = fgetc(infp);
	while(!feof(infp)) {
		w[(unsigned char)cha]++;//�ַ�cha����Ƶ���Լ�1
		size++;//�ļ���С�Լ�1
		cha = fgetc(infp);
	}
	if (pHuffmanTree != NULL)//�ͷſռ�
		delete[]pHuffmanTree;
	pHuffmanTree = new HuffmanTree<char, unsigned long>(ch, w, n);//���ɹ�������
	rewind(outfp);
	fwrite(&size, sizeof(unsigned long), 1, outfp);//ʹĿ���ļ�д��Դ�ļ���С
	for (i = 0; i < n; i++)//��Ŀ���ļ�д���ַ�����Ƶ��
		fwrite(&w[i], sizeof(unsigned long), 1, outfp);
	buf.bits = 0;
	buf.ch = 0;
	rewind(infp);
	cha = fgetc(infp);
	int count = 0;
	while (!feof(infp)) {//��Դ�ļ��ַ����б��룬��������д��Ŀ���ļ�
		std::string strTmp = pHuffmanTree->EnCode(cha);//�ַ�����
		int length_strTmp = strTmp.length();
		for (i = 0; i < length_strTmp; i++) {
			if (strTmp[i] == '0')Write(0);
			else Write(1);
		}
		count+=length_strTmp;
		cha = fgetc(infp);
	}
	WriteToOutfp();//ǿ��д��Ŀ���ļ�
	fclose(infp);
	fclose(outfp);
	std::cout << "�������."<<std::endl;
}

void HuffmanCompress::Decompress()
{//��ѹ������ĸ��ֽڽ��н��룬�����������ַ�д��Ŀ���ļ���
	char infName[256], outfName[256];
	std::cout << "������ѹ���ļ������ļ�С��4GB��:";
	std::cin >> infName;
	if ((infp = fopen(infName, "rb")) == NULL)
		throw "��ѹ���ļ�ʧ��";
	fgetc(infp);
	if (feof(infp))
		throw "ѹ���ļ�Ϊ�գ�";
	std::cout << "������Ŀ���ļ���:";
	std::cin >> outfName;
	if ((outfp = fopen(outfName, "wb")) == NULL)
		throw "��Ŀ���ļ�ʧ��";
	std::cout << "���ڴ������Ժ�..." << std::endl;

	const unsigned long n = 256;
	char ch[n], cha;
	unsigned long w[n], i, size = 0;

	rewind(infp);
	fread(&size, sizeof(unsigned long), 1, infp);//��ȡĿ���ļ��Ĵ�С
	for (i = 0; i < n; i++){
		ch[i] = (char)i;//��ʼ��ch[i]
		fread(&w[i], sizeof(unsigned long), 1, infp);//��ȡ�ַ�Ƶ��
	}
	if (pHuffmanTree != NULL)
		delete[]pHuffmanTree;
	pHuffmanTree = new HuffmanTree<char, unsigned long>(ch, w, n);

	unsigned long len = 0;
	cha = fgetc(infp);
	while (!feof(infp)) {//��ѹ���ļ��ַ����н��룬����������ַ�д��Ŀ���ļ�
		std::string strTmp = "";//��chaת����������ʽ�Ĵ�
		unsigned char c = (unsigned char)cha;//��chaת����unsigned char����
		for (i = 0; i < 8; i++) {//��cת���ɶ����ƴ�
			if (c < 128)
				strTmp+="0";//���λΪ0
			else 
				strTmp+="1";//���λΪ1
			c = c << 1;//����1λ
		}
		DblLinkList<char> tmp = pHuffmanTree->DeCode(strTmp);
		int length_tmp = tmp.Length();
		std::string strTemp;
		for (int i = 1; i <= length_tmp; i++) {
			char ch;
			tmp.GetElem(i, ch);
			strTemp += ch;
		}//����
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
	std::cout << "�������." <<std::endl;
}

int main()
{
	HuffmanCompress test;
	return 0;
}