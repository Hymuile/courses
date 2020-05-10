#pragma once
#include <climits>
#include "DblLinkList.cpp"

template<typename WeightType>
struct HuffmanTreeNode 
{
	WeightType weight;//Ȩ������
	unsigned int parent, leftChild, rightChild;//˫�ס����Һ�����

	HuffmanTreeNode() {parent = leftChild = rightChild = 0; };
	HuffmanTreeNode(WeightType w, int p = 0, int lChild = 0, int rChild = 0);
};

template<typename WeightType>
HuffmanTreeNode<WeightType>::HuffmanTreeNode(WeightType w, int p, int lChild , int rChild)
{
	weight = w;
	parent = p;
	leftChild = lChild;
	rightChild = rChild;
}

template<typename CharType, typename WeightType>
class HuffmanTree
{
protected:
	HuffmanTreeNode<WeightType>* nodes;//�洢�����Ϣ,nodes[0]δ��
	CharType* LeafChars;//Ҷ����ַ���Ϣ,LeafChars[0]δ��
	std::string* LeafCharCodes;//Ҷ����ַ�������Ϣ
	int curPos;//����ʱ�Ӹ��ڵ㵽Ҷ���·���ĵ�ǰ���
	int num;//Ҷ������
	void Select(int cur, int& r1, int& r2);//nodes[1~cur]��ѡ��˫��Ϊ0��Ȩֵ��С���������r1,r2
	void CreateHuffmanTree(CharType ch[], WeightType w[], int n);
public:
	HuffmanTree(CharType ch[], WeightType w[], int n);
	virtual ~HuffmanTree();
	std::string EnCode(CharType ch);//����
	DblLinkList<CharType> DeCode(std::string strCode);//����
	//HuffmanTree(const HuffmanTree<CharType, WeightType>& copy);
	//HuffmanTree<CharType, WeightType>& operator=(const HuffmanTree<CharType, WeightType>& copy);
};

template<typename CharType, typename WeightType>
void HuffmanTree<CharType, WeightType>::Select(int cur, int& r1, int& r2)
{
	r1 = r2 = 0;
	int min1 = INT_MAX, min2 = INT_MAX;
	for (int i = 1; i <= cur; i++) {
		if (nodes[i].parent == 0)
		{//ɸѡû�и��ڵ����С�ʹ�СȨֵ�±�
			if (nodes[i].weight < min1) {//�������СֵС
				min2 = min1;
				r2 = r1;
				min1 = nodes[i].weight;
				r1 = i;
			}
			else if ((nodes[i].weight >= min1) && (nodes[i].weight < min2)) {//������ڵ�����Сֵ��С�ڴ�Сֵ
				min2 = nodes[i].weight;
				r2 = i;
			}
		}
	}
}

template<typename CharType, typename WeightType>
void HuffmanTree<CharType, WeightType>::CreateHuffmanTree(CharType ch[], WeightType w[], int n) 
{
	num = n;//Ҷ������
	int m = 2 * n - 1;//������
	nodes = new HuffmanTreeNode<WeightType>[m + 1];//nodes[0]δ��
	LeafChars = new CharType[n + 1];//LeafChars[0]δ��
	LeafCharCodes = new std::string[n + 1];//LeafCharCodes[0]δ��
	for (int pos = 1; pos <= n; pos++) {//�洢Ҷ�����Ϣ
		nodes[pos].weight = w[pos - 1];//Ȩֵ
		LeafChars[pos] = ch[pos - 1];//�ַ�
	}
	for (int pos = n + 1; pos <= m; pos++) {//������������
		int r1, r2;
		Select(pos - 1, r1, r2);//nodes[1~pos-1]��ѡ��˫��Ϊ0��Ȩֵ��С���������r1,r2
		nodes[r1].parent = nodes[r2].parent = pos;
		nodes[pos].leftChild = r1;
		nodes[pos].rightChild = r2;
		nodes[pos].weight = nodes[r1].weight + nodes[r2].weight;
	}
	for (int pos = 1; pos <= n; pos++) {//��n��Ҷ����ַ��ı���
		DblLinkList<char> charCode;
		char ch;
		for (unsigned int child = pos, parent = nodes[child].parent; parent != 0; 
			child = parent, parent = nodes[child].parent) {//��Ҷ��㵽���ڵ����������
			if (nodes[parent].leftChild == child)//���֧����Ϊ'0'
				charCode.Insert(1, '0');
			else//�ҷ�֧����Ϊ'1'
				charCode.Insert(1, '1');
		}
		int length_charCode = charCode.Length();
		LeafCharCodes[pos] = "";
		for (int i = 1; i <= length_charCode; i++) {
			charCode.GetElem(i, ch);
			LeafCharCodes[pos] += ch;//charCode�д洢�ַ�����
		}
	}
	curPos = m;//����ʱ�Ӹ��ڵ㿪ʼ��mΪ��
}

template<typename CharType, typename WeightType>
HuffmanTree<CharType, WeightType>::HuffmanTree(CharType ch[], WeightType w[], int n){CreateHuffmanTree(ch, w, n);}

template<typename CharType, typename WeightType>
HuffmanTree<CharType, WeightType>::~HuffmanTree()
{
	delete[] nodes;
	delete[] LeafCharCodes;
	delete[] LeafChars;
}

template<typename CharType, typename WeightType>
std::string HuffmanTree<CharType, WeightType>::EnCode(CharType ch)
{
	for (int pos = 1; pos <= num; pos++)
		if (LeafChars[pos] == ch) 
			return LeafCharCodes[pos];
	throw std::runtime_error("�Ƿ��ַ����޷����룡");
}

template<typename CharType, typename WeightType>
DblLinkList<CharType> HuffmanTree<CharType, WeightType>::DeCode(std::string strCode)
{
	DblLinkList<CharType> charList;
	int len = strCode.length();
	for (int pos = 0; pos < len; pos++) {//����ÿλ����
		if (strCode[pos] == '0')//'0'��ʾ���֧
			curPos = nodes[curPos].leftChild;
		else//'1'��ʾ�ҷ�֧
			curPos = nodes[curPos].rightChild;
		if (nodes[curPos].leftChild == 0 && nodes[curPos].rightChild == 0) {
			//����ʱ�Ӹ���㵽Ҷ���·���ĵ�ǰ���ΪҶ���
			charList.Insert(charList.Length() + 1, LeafChars[curPos]);
			curPos = 2 * num - 1;
		}
	}
	return charList;
}