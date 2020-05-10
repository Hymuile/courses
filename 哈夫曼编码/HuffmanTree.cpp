#pragma once
#include <climits>
#include "DblLinkList.cpp"

template<typename WeightType>
struct HuffmanTreeNode 
{
	WeightType weight;//权数据域
	unsigned int parent, leftChild, rightChild;//双亲、左右孩子域

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
	HuffmanTreeNode<WeightType>* nodes;//存储结点信息,nodes[0]未用
	CharType* LeafChars;//叶结点字符信息,LeafChars[0]未用
	std::string* LeafCharCodes;//叶结点字符编码信息
	int curPos;//译码时从根节点到叶结点路径的当前结点
	int num;//叶结点个数
	void Select(int cur, int& r1, int& r2);//nodes[1~cur]中选择双亲为0，权值最小的两个结点r1,r2
	void CreateHuffmanTree(CharType ch[], WeightType w[], int n);
public:
	HuffmanTree(CharType ch[], WeightType w[], int n);
	virtual ~HuffmanTree();
	std::string EnCode(CharType ch);//编码
	DblLinkList<CharType> DeCode(std::string strCode);//译码
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
		{//筛选没有父节点的最小和次小权值下标
			if (nodes[i].weight < min1) {//如果比最小值小
				min2 = min1;
				r2 = r1;
				min1 = nodes[i].weight;
				r1 = i;
			}
			else if ((nodes[i].weight >= min1) && (nodes[i].weight < min2)) {//如果大于等于最小值且小于次小值
				min2 = nodes[i].weight;
				r2 = i;
			}
		}
	}
}

template<typename CharType, typename WeightType>
void HuffmanTree<CharType, WeightType>::CreateHuffmanTree(CharType ch[], WeightType w[], int n) 
{
	num = n;//叶结点个数
	int m = 2 * n - 1;//结点个数
	nodes = new HuffmanTreeNode<WeightType>[m + 1];//nodes[0]未用
	LeafChars = new CharType[n + 1];//LeafChars[0]未用
	LeafCharCodes = new std::string[n + 1];//LeafCharCodes[0]未用
	for (int pos = 1; pos <= n; pos++) {//存储叶结点信息
		nodes[pos].weight = w[pos - 1];//权值
		LeafChars[pos] = ch[pos - 1];//字符
	}
	for (int pos = n + 1; pos <= m; pos++) {//建立哈夫曼树
		int r1, r2;
		Select(pos - 1, r1, r2);//nodes[1~pos-1]中选择双亲为0，权值最小的两个结点r1,r2
		nodes[r1].parent = nodes[r2].parent = pos;
		nodes[pos].leftChild = r1;
		nodes[pos].rightChild = r2;
		nodes[pos].weight = nodes[r1].weight + nodes[r2].weight;
	}
	for (int pos = 1; pos <= n; pos++) {//求n个叶结点字符的编码
		DblLinkList<char> charCode;
		char ch;
		for (unsigned int child = pos, parent = nodes[child].parent; parent != 0; 
			child = parent, parent = nodes[child].parent) {//从叶结点到根节点逆向求编码
			if (nodes[parent].leftChild == child)//左分支编码为'0'
				charCode.Insert(1, '0');
			else//右分支编码为'1'
				charCode.Insert(1, '1');
		}
		int length_charCode = charCode.Length();
		LeafCharCodes[pos] = "";
		for (int i = 1; i <= length_charCode; i++) {
			charCode.GetElem(i, ch);
			LeafCharCodes[pos] += ch;//charCode中存储字符编码
		}
	}
	curPos = m;//译码时从根节点开始，m为根
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
	throw std::runtime_error("非法字符，无法编码！");
}

template<typename CharType, typename WeightType>
DblLinkList<CharType> HuffmanTree<CharType, WeightType>::DeCode(std::string strCode)
{
	DblLinkList<CharType> charList;
	int len = strCode.length();
	for (int pos = 0; pos < len; pos++) {//处理每位编码
		if (strCode[pos] == '0')//'0'表示左分支
			curPos = nodes[curPos].leftChild;
		else//'1'表示右分支
			curPos = nodes[curPos].rightChild;
		if (nodes[curPos].leftChild == 0 && nodes[curPos].rightChild == 0) {
			//译码时从根结点到叶结点路径的当前结点为叶结点
			charList.Insert(charList.Length() + 1, LeafChars[curPos]);
			curPos = 2 * num - 1;
		}
	}
	return charList;
}