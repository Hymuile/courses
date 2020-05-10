#include <fstream>
#include <iostream>
#include <string>
#include <cassert>
#include <stack>
const long Infinity = 999999;
typedef struct {
	int num;
	std::string message;
}viewSpot;//景点

typedef struct {
	viewSpot* viewSpots;//存放顶点的一维数组，数组第零单元未用上
	double** length;//存放路径长度
	int m, n;
}Mgraph;

class ParkDragoman
{
private:
	Mgraph Mgr;
	int numOfViewSpots;//景点数
	std::ifstream* pInFile;
	int maxNumOfShowPath;//最多显示路线条数
	//佛洛伊德算法
	double** path;//存贮路径
	double** shortest;//贮存最小路径
	//迪杰斯特拉算法
	double *D;
	bool* S;
	std::stack<double> stack;
	int source, sink, step = 1, * Path;//source 和 sink为起点和终点
	int locateVertex(std::string& e)const;//返回顶点元素在网中的位置
	int locateSpace(std::string& e)const;//返回e的第一个空格的位置
	std::string num2str(int i)const;//返回第i号景点的名称
	void ReadFile();//扫描文件
	void Init();//初始化
	char GetChar(std::ifstream* pFile);//返回除换行符和'\t'之外的字符
	void Readln();//跳过一行
	
	void FloydHelp(int i, int j);
	void ShortestPathFloyd();

	void input();//迪杰斯特拉算法的输入
	double minD();
	void ShortestPathDIJHelp();
	void ShortestPathDIJ();

	bool* TAG;//tag数组，标志该景点是否被访问过，在不重复地游览所有景点中使用
	int firstAdjVex(int v)const;//返回景点v的第一个邻接点
	int nextAdjVex(int v, int w)const;//返回顶点v的相对于w的下一个邻接点
	void ShowTourismPath(int tourismPath[]);//输出游览线路
	void createTourismPath(int to, int v, int tourismPath[], int pathVertexNum);//tourisimPath为游览路径，出口为to
	//v为当前顶点，pathVertexNum为路径当前顶点数
	
public:
	ParkDragoman();
	virtual ~ParkDragoman() ;
	void Run();
};

int ParkDragoman::locateVertex(std::string& e)const
{
	for (int i = 1; i < numOfViewSpots + 3; i++)
		if (Mgr.viewSpots[i].message == e)
			return Mgr.viewSpots[i].num;
	return 0;
}

int ParkDragoman::locateSpace(std::string& e)const
{
	int length = e.length();
	for (int i = 0; i < length; i++)
		if (e.at(i) == ' ')
			return i;
	return -1;
}

std::string ParkDragoman::num2str(int i)const
{
	std::string ans = Mgr.viewSpots[i].message;
	return ans;
}

void ParkDragoman::ReadFile()
{//读取景点名称，景点数，最大显示线路数，并初始化每个景点信息
	char ch;
	ch = GetChar(pInFile);
	while (ch != 'n') {
		if (ch != '\n')
			Readln();
		ch = GetChar(pInFile);
	}
	ch=GetChar(pInFile);//跳过=
	std::string tmpNumOfViewSpots;
	std::getline(*pInFile,tmpNumOfViewSpots);
	numOfViewSpots=std::stoi(tmpNumOfViewSpots);
	Mgr.viewSpots = new viewSpot[numOfViewSpots + 3];

	(*pInFile).seekg(0);
	ch = GetChar(pInFile);
	while (ch != 'v') {
		if (ch != '\n')
			Readln();
		ch = GetChar(pInFile);
	}
	ch=GetChar(pInFile);
	int i=1;
	for (i = 1; i <= numOfViewSpots; i++) {
		Mgr.viewSpots[i].num = i;
		(*pInFile) >> Mgr.viewSpots[i].message;
	}
	Mgr.viewSpots[numOfViewSpots + 1].num = numOfViewSpots +1;
	Mgr.viewSpots[numOfViewSpots + 1].message = "入口";
	Mgr.viewSpots[numOfViewSpots + 2].num = numOfViewSpots + 2;
	Mgr.viewSpots[numOfViewSpots + 2].message = "出口";

	(*pInFile).seekg(0);
	ch = GetChar(pInFile);
	while (ch != 's') {
		if (ch != '\n')Readln();
		ch = GetChar(pInFile);
	}
	GetChar(pInFile);
	(*pInFile) >> maxNumOfShowPath;
}

void ParkDragoman::Init()
{
	int i, j;
	//初始化length数组,赋值为无穷大，即任意两个景点间距离为无穷大
	Mgr.length = new double* [numOfViewSpots + 3];
	for (i = 0; i < numOfViewSpots + 3; i++)
		Mgr.length[i] = new double[numOfViewSpots + 3];
	for (i = 1; i <= numOfViewSpots + 2; i++)
		for (j = 1; j <= numOfViewSpots + 2; j++)
			Mgr.length[i][j] = Infinity;
	//初始化shortest数组
	shortest = new double* [numOfViewSpots + 3];
	for (i = 0; i < numOfViewSpots + 3; i++)
		shortest[i] = new double[numOfViewSpots + 3];
	for (i = 1; i <= numOfViewSpots + 2; i++)
		shortest[i][j] = 0;

	//初始化path数组
	path = new double* [numOfViewSpots + 3];
	for (i = 0; i < numOfViewSpots + 3; i++)
		path[i] = new double[numOfViewSpots + 3];

	//读取两景点间的距离
	(*pInFile).seekg(0);
	char ch = GetChar(pInFile);
	while (true) {
		while (ch != '(' && ch != 's') {
			if (ch != '\n')
				Readln();
			ch = GetChar(pInFile);
		}
		if (ch == 's')break;
		std::string firstStr,secondStr,e1, e2;
		int v1, v2;
		double w;
		std::getline(*pInFile,firstStr);
		int firstSpace = locateSpace(firstStr);
		e1 = firstStr.substr(0, firstSpace);
		secondStr = firstStr.substr(firstSpace + 1);
		int secondSpace = locateSpace(secondStr);
		e2 = secondStr.substr(0,secondSpace);
		w = std::stod(secondStr.substr(secondSpace+1));
		if ((v1 = locateVertex(e1)) == 0|| (v2 = locateVertex(e2)) == 0)
			throw "在储存完景点名称后，检索时不存在该景点。";
		Mgr.length[v1][v2] = Mgr.length[v2][v1] = w;
		ch = GetChar(pInFile);
	}
	(*pInFile).close();
}

char ParkDragoman::GetChar(std::ifstream* pFile)
{
	char ans;
	*pFile >> ans;
	while (ans == '\n' || ans == '\t')
		*pFile >> ans;
	return ans;
}

void ParkDragoman::Readln()
{
	std::string tmpLine;
	std::getline(*pInFile,tmpLine);
}

void ParkDragoman::FloydHelp(int i, int j)
{
	int tmpI = i, tmpJ = j;
	std::cout << num2str(tmpI);//输出起点的景点名称
	while (path[i][j] != 0) {
		std::cout << "-->" << num2str(path[i][j]);
		if (i < j) j = path[i][j];
		else i = path[i][j];
	}
	std::cout << "-->" << num2str(tmpJ);
	std::cout << "\n长度:" << shortest[tmpI][tmpJ]<<std::endl;
}

void ParkDragoman::ShortestPathFloyd()
{
	for(int i=1;i<numOfViewSpots+3;i++)
		for (int j = 1; j < numOfViewSpots + 3; j++) {
			shortest[i][j] = Mgr.length[i][j];
			path[i][j] = 0;
		}
	for (int k = 1; k < numOfViewSpots + 3; k++)
		for (int i = 1; i < numOfViewSpots + 3; i++)
			for (int j = 1; j < numOfViewSpots + 3; j++)
				if (shortest[i][j] > (shortest[i][k] + shortest[k][j])) {
					shortest[i][j] = shortest[i][k] + shortest[k][j];
					path[i][j] = path[j][i] = k;
				}
	std::string From, To;
	do {
		std::cout << "输入起点:";
		std::cin >> From;
		if (locateVertex(From) != 0)
			break;
		else
			std::cout<<"重新";
	} while (true);
	do {
		std::cout << "\n输入终点:";
		std::cin >> To;
		if (locateVertex(To) != 0)
			break;
		else
			std::cout<< "重新";
	} while (true);
	std::cout << "从[" << From << "]到[" << To << "]的最短路径为:\n";
	FloydHelp(locateVertex(From), locateVertex(To));
	
	
}

void ParkDragoman::input()
{
	std::string From="", To="";
	do {
		std::cout << "输入起点:";
		std::getline(std::cin, From);
	} while (locateVertex(From) == 0);
	do {
		std::cout << "\n输入终点:";
		std::cin >> To;
	} while (locateVertex(To) == 0);
	S = new bool[numOfViewSpots + 3];
	Path = new int[numOfViewSpots + 3];
	D = new double[numOfViewSpots + 3];
	source = locateVertex(From);
	sink = locateVertex(To);
	for (int i = 0; i < numOfViewSpots+3; i++){
		S[i] = false;
		Path[i] = source;
		D[i] = Mgr.length[source][i];
	}
	S[source] = true;
	D[source] = 0;
}

double ParkDragoman::minD()
{
	int i, t = 0;
	long minimum = Infinity;
	for (int i = 1; i < numOfViewSpots + 3; i++) 
		if (!S[i] && D[i] < minimum) {
			minimum = D[i];
			t = i;
		}
	return t;
}

void ParkDragoman::ShortestPathDIJHelp()
{
	int node=sink;
	if ((sink == source) || D[sink] == Infinity) {
		std::cout << Mgr.viewSpots[source].message << "-" << Mgr.viewSpots[sink].message << "无路径\n";
		return;
	}
	while (node != source) {
		stack.push(node);
		node = Path[node];
	}
	std::cout << "[" << Mgr.viewSpots[source].message << "]到[" << Mgr.viewSpots[sink].message << "]的路径为：\n";
	std::cout << Mgr.viewSpots[source].message;
	while (node != sink) {
		node = stack.top();
		stack.pop();
		std::cout << "--" << Mgr.length[Path[node]][node] << "-->" << Mgr.viewSpots[node].message;
	}
	std::cout << "\n长度：" << D[sink]<<std::endl;
}

void ParkDragoman::ShortestPathDIJ()
{
	int t;
	input();
	for (step = 2; step < numOfViewSpots + 3; step++) {
		t = minD();
		S[t] = true;
		for (int i = 1; i < numOfViewSpots + 3; i++)
			if (!S[i] && (D[t] + Mgr.length[t][i] <= D[i])) {
				D[i] = D[t] + Mgr.length[t][i];
				Path[i] = t;
			}
	}
	ShortestPathDIJHelp();
	
}

int ParkDragoman::firstAdjVex(int v)const
{
	for (int i = 1; i < numOfViewSpots + 3; i++)
		if (Mgr.length[v][i] != Infinity && v != i)
			return i;
	return 0;
}

int ParkDragoman::nextAdjVex(int v, int w)const
{
	for (int i = w+1; i < numOfViewSpots + 3; i++)
		if (Mgr.length[v][i] != Infinity && v!=i)
			return i;
	return 0;
}

void ParkDragoman::ShowTourismPath(int tourismPath[])
{
	std::cout << "入口-->";
	for (int i = 1; i <= numOfViewSpots; i++)
		std::cout << Mgr.viewSpots[tourismPath[i - 1]].message<<"-->";
	std::cout << "出口\n";
	double len = 0;
	for (int i = 0; i <= numOfViewSpots - 1; i++)
		len += Mgr.length[tourismPath[i]][tourismPath[i + 1]];
	std::cout << "长度: " << len << std::endl;
}

void ParkDragoman::createTourismPath(int to, int v, int tourismPath[], int pathVertexNum)
{
	static int n = 1;
	TAG[numOfViewSpots + 1] = true;
	for (int w = firstAdjVex(v); w > 0; w = nextAdjVex(v, w)) {
		if (n == maxNumOfShowPath)
			break;
		else if (!TAG[w] && pathVertexNum == numOfViewSpots && w == to && n < maxNumOfShowPath) {
			++n;
			tourismPath[pathVertexNum++] = w;
			ShowTourismPath(tourismPath);
		}
		else if (!TAG[w]) {//w未被访问过，将w加入到访问路线中
			tourismPath[pathVertexNum++] = w;
			TAG[w] = true;
			createTourismPath(to, w, tourismPath, pathVertexNum);
			pathVertexNum--;//回复路径顶点个数，回溯
			TAG[w] = false;
		}
	}

}

ParkDragoman::ParkDragoman()
{
	pInFile = new std::ifstream("park_infor.txt");
	assert((*pInFile).is_open());
	ReadFile();
	Init();
}

ParkDragoman::~ParkDragoman() 
{
	for (int i = 0; i < numOfViewSpots + 3; i++) {
		delete[] path[i];
		delete[] Mgr.length[i];
	}
	delete[] path;
	delete[] shortest;
	delete[] Mgr.viewSpots;
	delete[] Mgr.length;
	delete[] S;
	delete[] D;
	delete[] Path;
	delete[]TAG; 
}

void ParkDragoman::Run()
{	
	int option = 0;
	while (option!=5) {
		std::cout << "\n1:查看所有景点名称\n";
		std::cout << "2:最短路径-弗洛伊德算法\n";
		std::cout << "3:最短路径-迪杰斯拉特算法\n";
		std::cout << "4:游览路线，游览全部景点\n";
		std::cout << "5:退出\n";
		std::cout << "请输入选项:";
		std::cin >> option;
		std::string Terminal = "出口", Entrance = "入口";
		int to = locateVertex(Terminal), v = locateVertex(Entrance), pathVertexNum = 0, * tourismPath = new int[numOfViewSpots + 3];
		switch (option){
		case 1:
			for (int i = 0; i < numOfViewSpots + 3; i++)
				std::cout << Mgr.viewSpots[i].message << std::endl;
			break;
		case 2:
			ShortestPathFloyd(); 
			break;
		case 3:
			ShortestPathDIJ();
			break;
		case 4:
			TAG = new bool[numOfViewSpots + 3];
			for (int i = 0; i < numOfViewSpots + 3; i++)
				TAG[i] = false;
			createTourismPath(to, v, tourismPath, pathVertexNum);
			ShowTourismPath(tourismPath);
			break;
		default:
			break;
		}
	}
}

int main()
{
	ParkDragoman test;
	test.Run();
	return 0;
}