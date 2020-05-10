#include <iostream>
#include <fstream>
#include <cassert>
#include "DblLinkList.cpp"

class LineEditor final
{
private:
	DblLinkList<std::string> textBuffer;//文本缓存
	int curLineNo;//当前行号
	DblLinkList<std::string> textUndoBuffer;//用于恢复的文本缓存
	int curUndoLineNo;//用于恢复的当前行号
	std::ifstream inFile;//输入文件
	std::ofstream outFile;//输出文件
	std::string inFileStr,outFileStr;

	char GetChar();//从输入流中跳过空格及制表符获取一字符
	bool NextLine();//转到下一行
	bool PreviousLine();//转到上一行
	bool GotoLine();//转到指定行
	bool InsertLine();//插入一行
	void ChangeLine();//替换当前行或所有行的指定文本串
	void ReadFile();//读入文本文件
	void WriteFile();//写文本文件
	void FindString();//查找串
	void Substitute();
	void GetNext(const std::string &str, int* next);
	int Index(const std::string&,const std::string&);//KMP算法

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
	std::cout << "\n请输入要转到的行号:";
	std::cin >> lineNumber;
	if (lineNumber >= 1 && lineNumber <= textBuffer.Length()) {
		curLineNo = lineNumber;
		return true;
	}
	do {
		std::cout << "\n行号错误，不存在该行号,是否重新输入?(y/n):";
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
	std::cout << "请输入要插在第几行(行号过大则插入最后一行；行号<=1则插在第一行):";
	std::cin >> tmpLine;
	getchar();
	std::cout << "请输入要插入的内容:";
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
			std::cout << "替换当前行c(urrent)或者替换所有行a(ll):";
		else 
			std::cout << "用c或a回答:";
		answer = GetChar();
		while (std::cin.get() != '\n');
		answer = tolower(answer);
		initialResponse = false;
	} while (answer != 'c' && answer != 'a');
	std::string strOld, strNew;
	std::cout << "输入要被替换的指定文本串:";
	getline(std::cin,strOld);
	std::cout << "输入新文本串:";
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

void LineEditor::ReadFile()//读入一个新的文本文件到文本缓存内
{
	std::string infStr = "";
	do {
		std::cout << "请输入要读取的文本文件名:";
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

void LineEditor::WriteFile()//将文本缓存写入文件内
{
	int function = 0;
	do {
		std::cout << "\n想在文件尾添加新内容还是重写文件?--1:添加--2:重写--:";
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
		if (initialResponse) std::cout << "从第1行开始f(irst)或从当前行开始c(urrent):";
		else std::cout << "用f或c回答:";
		answer = GetChar();
		while (std::cin.get() != '\n');
		answer = tolower(answer);
		initialResponse = false;
	} while (answer != 'f' && answer != 'c');
	int index;
	std::cout << "输入被查找的文本串:";
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
		std::cout << "查找串失败!";
	else {
		std::cout << curLine.c_str() << std::endl;
		for (int i = 0; i < index; i++) 
			std::cout << " ";//在查找串前的位置显行空格
		for (int j = 0; j < (int)strlen(searchString.c_str()); j++)
			std::cout << "^";//在查找串前的位置显行^
	}
	std::cout << std::endl;
	curLineNo = originalCurLineNo;
}

void LineEditor::Substitute() {
	std::string tmpString;
	std::cout << "输入要替换的文本串：";
	getline(std::cin, tmpString);
	textBuffer.SetElem(curLineNo, tmpString);
}

void LineEditor::GetNext(const std::string &str, int* next)
{
	next[0] = -1;
	int j = 0, k = -1;
	while (j < str.length() - 1) {
		if (k == -1 || str[k] == str[j]){//当且仅当 j = 0 时 k=-1
			next[j + 1] = k + 1;
			j++; k++;
		}
		else
			k = next[k];
	}
}

int LineEditor::Index(const std::string &source,const std::string &target)//KMP算法
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
			std::cout << curLineNo << ":" << curLine.c_str() << std::endl << "选择一个功能:";
		}
		else
			std::cout << "文件缓存空" << std::endl << "选择一个功能:";
		userCommand =tolower(GetChar());
		while (std::cin.get() != '\n');
		if (userCommand != 'u' && userCommand != 'h' && userCommand != '?' && userCommand != 'v') {
			textUndoBuffer = textBuffer;
			curUndoLineNo = curLineNo;
		}
		switch (userCommand) {
		case 'b'://转到第1行b(egin)
			if (textBuffer.Empty())
				std::cout << "警告：文本缓存空" << std::endl;
			else
				curLineNo = 1;
			break;
		case 'c'://替换当前行或所有行
			if (textBuffer.Empty())
				std::cout << "警告：文本缓存空" << std::endl;
			else
				ChangeLine();
			break;
		case 'd'://删除当前行
			if (textBuffer.Delete(curLineNo, tempString) != statusCode::SUCCESS)
				std::cout << "错误:删除失败" << std::endl;
			break;
		case 'e'://转到最后一行e(nd)
			if (textBuffer.Empty())
				std::cout << "警告：文本缓存空" << std::endl;
			else
				curLineNo = textBuffer.Length();
			break;
		case 'f':
			if (textBuffer.Empty())
				std::cout << "警告：文本缓存空" << std::endl;
			else
				FindString();
			break;
		case 'g':
			if (GotoLine() != true)
				std::cout << "警告：不存在该行" << std::endl;
			break;
		case '?':case 'h':
			std::cout << "有效命令：b(egin) c(hange) d(el) e(nd)" << std::endl
				<< "f(ind) g(o) h(elp) i(nsert) n(ext) p(rivious)" << std::endl
				<< "q(uit) r(ead) s(ubstitute) u(ndo) v(iew) w(rite)" << std::endl;
			break;
		case 'i':
			if (!InsertLine())
				std::cout << "错误：插入行出错" << std::endl;
			break;
		case 'n':
			if (!NextLine())
				std::cout << "警告：转入下一行失败" << std::endl;
			break;
		case 'p':
			if(!PreviousLine())
				std::cout << "警告：转入上一行失败" << std::endl;
			break;
		case 'q':
			break;
		case 'r':
			ReadFile();//读输入文件到文本缓存中
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
				std::cout << "警告：文本缓存空" << std::endl;
			else
				WriteFile();//写文本缓存到输出文件中
			break;
		default:
			std::cout << "输入h或?获得帮助或输入有效命令字符：\n";
		}
	} while (userCommand != 'q');
}

LineEditor::LineEditor(std::string infName, std::string outfName,int choice)//构造函数，读入文件赋给文本缓存
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
		std::cout << "\n打开已有文件还是只建立新文件？--1:打开已有文件--2:新建文件:";
		std::cin >> choice;
	} while (choice != 1 && choice != 2);
	getchar();
	if (choice == 1) {
		std::string infStr, outfStr;
		std::cout << "输入文件名(默认:file_in.txt):";
		getline(std::cin, infStr);
		std::cout << "输出文件名(默认:file_out.txt):";
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
		std::cout << "新建输出文件名(默认:file_out.txt):";
		getline(std::cin, outfStr);
		if (outfStr == "")
			outfStr = "file_out.txt";
		LineEditor* test = new LineEditor(infStr, outfStr,2);
		test->Run();
		delete test;
	}
	return 0;
}