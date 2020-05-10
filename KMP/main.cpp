#include <iostream>
#include <fstream>
#include <cassert>
#include "DblLinkList.cpp"

class LineEditor final
{
private:
	DblLinkList<std::string> textBuffer;//�ı�����
	int curLineNo;//��ǰ�к�
	DblLinkList<std::string> textUndoBuffer;//���ڻָ����ı�����
	int curUndoLineNo;//���ڻָ��ĵ�ǰ�к�
	std::ifstream inFile;//�����ļ�
	std::ofstream outFile;//����ļ�
	std::string inFileStr,outFileStr;

	char GetChar();//���������������ո��Ʊ����ȡһ�ַ�
	bool NextLine();//ת����һ��
	bool PreviousLine();//ת����һ��
	bool GotoLine();//ת��ָ����
	bool InsertLine();//����һ��
	void ChangeLine();//�滻��ǰ�л������е�ָ���ı���
	void ReadFile();//�����ı��ļ�
	void WriteFile();//д�ı��ļ�
	void FindString();//���Ҵ�
	void Substitute();
	void GetNext(const std::string &str, int* next);
	int Index(const std::string&,const std::string&);//KMP�㷨

public:
	LineEditor(std::string infName, std::string outfName, int choice);
	~LineEditor();
	void Run();
};

char LineEditor::GetChar()
{
	char tmpChar;
	while ((tmpChar = std::cin.get()) == ' ' || tmpChar == '\t'||tmpChar == '\n');
	return tmpChar;
}

bool LineEditor::NextLine()
{
	if (curLineNo + 1 <= textBuffer.Length()) {
		curLineNo++;
		return true;
	}
	return false;
}

bool LineEditor::PreviousLine()
{
	if (curLineNo - 1 >= 1) {
		curLineNo--;
		return true;
	}
	return false;
}

bool LineEditor::GotoLine()
{
	int lineNumber;
	char tmpChar;
	std::cout << "\n������Ҫת�����к�:";
	std::cin >> lineNumber;
	if (lineNumber >= 1 && lineNumber <= textBuffer.Length()) {
		curLineNo = lineNumber;
		return true;
	}
	do {
		std::cout << "\n�кŴ��󣬲����ڸ��к�,�Ƿ���������?(y/n):";
		tmpChar = tolower(GetChar());
		if (tmpChar == 'y')
			GotoLine();
		else if (tmpChar == 'n')
			return false;
	} while (tmpChar != 'y' && tmpChar != 'n');
	return false;
}

bool LineEditor::InsertLine()
{
	int tmpLine;
	std::string tmpString;
	std::cout << "������Ҫ���ڵڼ���(�кŹ�����������һ�У��к�<=1����ڵ�һ��):";
	std::cin >> tmpLine;
	getchar();
	std::cout << "������Ҫ���������:";
	getline(std::cin, tmpString);
	if (tmpLine < 1)
		textBuffer.Insert(1, tmpString);
	else if (tmpLine > textBuffer.Length())
		textBuffer.Insert(textBuffer.Length() + 1, tmpString);
	else
		textBuffer.Insert(tmpLine, tmpString);
	return true;
}

void LineEditor::ChangeLine()
{
	char answer;
	bool initialResponse = true;
	do {
		if (initialResponse) 
			std::cout << "�滻��ǰ��c(urrent)�����滻������a(ll):";
		else 
			std::cout << "��c��a�ش�:";
		answer = GetChar();
		while (std::cin.get() != '\n');
		answer = tolower(answer);
		initialResponse = false;
	} while (answer != 'c' && answer != 'a');
	std::string strOld, strNew;
	std::cout << "����Ҫ���滻��ָ���ı���:";
	getline(std::cin,strOld);
	std::cout << "�������ı���:";
	getline(std::cin, strNew);
	for (int row = 1; row <= textBuffer.Length(); row++)
	{
		if (answer == 'c' && row != curLineNo)
			continue;
		std::string strRow;
		textBuffer.GetElem(row, strRow);
		int index = Index(strRow, strOld);
		if (index != -1) {
			std::string newLine="";
			newLine += strRow.substr(0,index);
			newLine += strNew;
			newLine += strRow.substr(index + strOld.size());
			textBuffer.SetElem(row, newLine);
		}
	}
}

void LineEditor::ReadFile()//����һ���µ��ı��ļ����ı�������
{
	std::string infStr = "";
	do {
		std::cout << "������Ҫ��ȡ���ı��ļ���:";
		getline(std::cin, infStr);
	} while (infStr == "");
	outFileStr=inFileStr=infStr;
	std::ifstream inFile(infStr.c_str(), std::ios::in);
	assert(inFile.is_open());
	std::string s;
	int i = 1;
	while (getline(inFile, s)) {
		textBuffer.Insert(i++,s);
	}
	inFile.close();
}

void LineEditor::WriteFile()//���ı�����д���ļ���
{
	int function = 0;
	do {
		std::cout << "\n�����ļ�β��������ݻ�����д�ļ�?--1:���--2:��д--:";
		std::cin >> function;
	} while (function != 1 && function != 2);
	if (function == 1)
		outFile.open(outFileStr.c_str(), std::ios::app);
	else
		outFile.open(outFileStr.c_str(), std::ios::out);
	
	assert(outFile.is_open());
	std::string tmpString;
	int i=1;
	while(i<=textBuffer.Length()){
		textBuffer.GetElem(i,tmpString);
		if(i!=textBuffer.Length())
			outFile<<tmpString<<"\n";
		else
			outFile<<tmpString;
		i++;
	}
	outFile.close();
}

void LineEditor::FindString() 
{
	int originalCurLineNo = curLineNo;
	char answer;
	bool initialResponse = true;
	do {
		if (initialResponse) std::cout << "�ӵ�1�п�ʼf(irst)��ӵ�ǰ�п�ʼc(urrent):";
		else std::cout << "��f��c�ش�:";
		answer = GetChar();
		while (std::cin.get() != '\n');
		answer = tolower(answer);
		initialResponse = false;
	} while (answer != 'f' && answer != 'c');
	int index;
	std::cout << "���뱻���ҵ��ı���:";
	std::string searchString, curLine;
	std::getline(std::cin, searchString);
	if (answer == 'f') curLineNo = 1;
	textBuffer.GetElem(curLineNo, curLine);
	while ((index = Index(curLine, searchString)) == -1){
		if (curLineNo <= textBuffer.Length())
			textBuffer.GetElem(++curLineNo, curLine);
		else
			break;
	}
	if (index == -1) 
		std::cout << "���Ҵ�ʧ��!";
	else {
		std::cout << curLine.c_str() << std::endl;
		for (int i = 0; i < index; i++) 
			std::cout << " ";//�ڲ��Ҵ�ǰ��λ�����пո�
		for (int j = 0; j < (int)strlen(searchString.c_str()); j++)
			std::cout << "^";//�ڲ��Ҵ�ǰ��λ������^
	}
	std::cout << std::endl;
	curLineNo = originalCurLineNo;
}

void LineEditor::Substitute() {
	std::string tmpString;
	std::cout << "����Ҫ�滻���ı�����";
	getline(std::cin, tmpString);
	textBuffer.SetElem(curLineNo, tmpString);
}

void LineEditor::GetNext(const std::string &str, int* next)
{
	next[0] = -1;
	int j = 0, k = -1;
	while (j < str.length() - 1) {
		if (k == -1 || str[k] == str[j]){//���ҽ��� j = 0 ʱ k=-1
			next[j + 1] = k + 1;
			j++; k++;
		}
		else
			k = next[k];
	}
}

int LineEditor::Index(const std::string &source,const std::string &target)//KMP�㷨
{
	int i = 0, j = 0, srcLen = source.length(), trgLen = target.length();
	int* next = new int[trgLen];
	GetNext(target, next);
	while (i < srcLen && j < trgLen){
		if (j == -1 || source[i] == target[j]){
			i++;
			j++;
		}
		else
			j = next[j];
	}
	if (j == target.length())
		return i-j;
	return -1;
}

template<typename T>
void Swap(T &x, T &y)
{
	T tmp;
	tmp = x, x = y, y = tmp;
}

void LineEditor::Run()
{
	char userCommand;
	do {
		std::cin.sync();
		std::string tempString,curLine;
		if (curLineNo != 0) {
			textBuffer.GetElem(curLineNo, curLine);
			std::cout << curLineNo << ":" << curLine.c_str() << std::endl << "ѡ��һ������:";
		}
		else
			std::cout << "�ļ������" << std::endl << "ѡ��һ������:";
		userCommand =tolower(GetChar());
		while (std::cin.get() != '\n');
		if (userCommand != 'u' && userCommand != 'h' && userCommand != '?' && userCommand != 'v') {
			textUndoBuffer = textBuffer;
			curUndoLineNo = curLineNo;
		}
		switch (userCommand) {
		case 'b'://ת����1��b(egin)
			if (textBuffer.Empty())
				std::cout << "���棺�ı������" << std::endl;
			else
				curLineNo = 1;
			break;
		case 'c'://�滻��ǰ�л�������
			if (textBuffer.Empty())
				std::cout << "���棺�ı������" << std::endl;
			else
				ChangeLine();
			break;
		case 'd'://ɾ����ǰ��
			if (textBuffer.Delete(curLineNo, tempString) != statusCode::SUCCESS)
				std::cout << "����:ɾ��ʧ��" << std::endl;
			break;
		case 'e'://ת�����һ��e(nd)
			if (textBuffer.Empty())
				std::cout << "���棺�ı������" << std::endl;
			else
				curLineNo = textBuffer.Length();
			break;
		case 'f':
			if (textBuffer.Empty())
				std::cout << "���棺�ı������" << std::endl;
			else
				FindString();
			break;
		case 'g':
			if (GotoLine() != true)
				std::cout << "���棺�����ڸ���" << std::endl;
			break;
		case '?':case 'h':
			std::cout << "��Ч���b(egin) c(hange) d(el) e(nd)" << std::endl
				<< "f(ind) g(o) h(elp) i(nsert) n(ext) p(rivious)" << std::endl
				<< "q(uit) r(ead) s(ubstitute) u(ndo) v(iew) w(rite)" << std::endl;
			break;
		case 'i':
			if (!InsertLine())
				std::cout << "���󣺲����г���" << std::endl;
			break;
		case 'n':
			if (!NextLine())
				std::cout << "���棺ת����һ��ʧ��" << std::endl;
			break;
		case 'p':
			if(!PreviousLine())
				std::cout << "���棺ת����һ��ʧ��" << std::endl;
			break;
		case 'q':
			break;
		case 'r':
			ReadFile();//�������ļ����ı�������
			break;
		case 's':
			Substitute();
			break;
		case 'u':
			Swap(textBuffer, textUndoBuffer);
			Swap(curUndoLineNo, curLineNo);
			break;
		case 'v':
			textBuffer.Traverse();
			break;
		case 'w':
			if (textBuffer.Empty())
				std::cout << "���棺�ı������" << std::endl;
			else
				WriteFile();//д�ı����浽����ļ���
			break;
		default:
			std::cout << "����h��?��ð�����������Ч�����ַ���\n";
		}
	} while (userCommand != 'q');
}

LineEditor::LineEditor(std::string infName, std::string outfName,int choice)//���캯���������ļ������ı�����
{
	curUndoLineNo=curLineNo = 1;
	inFileStr = infName;
	outFileStr = outfName;
	if (choice == 1)
	{
		inFile.open(inFileStr.c_str());
		assert(inFile.is_open());
		std::string s;
		int i = 1;
		while (getline(inFile, s))
			textBuffer.Insert(i++, s);
		inFile.close();
	}
}

LineEditor::~LineEditor()
{
	inFile.close();
	outFile.close();
}

int main()
{
	int choice = 0;
	do {
		std::cout << "\n�������ļ�����ֻ�������ļ���--1:�������ļ�--2:�½��ļ�:";
		std::cin >> choice;
	} while (choice != 1 && choice != 2);
	getchar();
	if (choice == 1) {
		std::string infStr, outfStr;
		std::cout << "�����ļ���(Ĭ��:file_in.txt):";
		getline(std::cin, infStr);
		std::cout << "����ļ���(Ĭ��:file_out.txt):";
		getline(std::cin, outfStr);
		if (infStr == "")
			infStr = "file_in.txt";
		if (outfStr == "")
			outfStr = "file_out.txt";
		LineEditor* test = new LineEditor(infStr, outfStr,1);
		test->Run();
		delete test;
	}
	else {
		std::string infStr, outfStr;
		std::cout << "�½�����ļ���(Ĭ��:file_out.txt):";
		getline(std::cin, outfStr);
		if (outfStr == "")
			outfStr = "file_out.txt";
		LineEditor* test = new LineEditor(infStr, outfStr,2);
		test->Run();
		delete test;
	}
	return 0;
}