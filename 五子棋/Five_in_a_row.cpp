#include <conio.h>
#include <cstring>
#include <iostream>
#include <windows.h>
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <fstream>
#include <string>
#include <math.h>

#define NumOfLines (15)
#define huo5 if(count==5)return 1;
#define huo5_1 if(count==5) return 2;
#define samekey same(row + dx[u] * i, col + dy[u] * i, key)// judge if the next pieces's color is the same with the current piece's
#define sumkadd for (i = 1; samekey; i++)sumk++; //positive traversal
#define sumksub for (i = -1; samekey; i--)sumk++;//negative traversal
#define off if(!inboard(row + dx[u] * i, col + dy[u] * i) || chess_board[row + dx[u] * i][col + dy[u] * i] != 0)continue;//judge cross the border

int chess_board[NumOfLines][NumOfLines];//0 None，1 red，2blue  1●2○,red first drops and blue second drops
int player_state = 0, ais = 1, player0;//player_state=1,AI drops; player=2, player drops. player=player0, Red drops otherwise Blue drops.
bool is_end = false;
int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 }; //flat technology
int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };//（dx,dy） are 8 direction vectors

COORD cursor;

class Chess
{
	int chess_board[NumOfLines][NumOfLines];
public:
	bool init_board_array();
	bool win() const;
	bool dropout(int, int);
	bool AM_Win(int player);

	friend bool man2man_move_drop(int);
	friend int CalSumPieces(Chess);
} Red, Blue, AM;

void gotoxy(int, int);
int CalSumPieces(Chess);
bool ShowNumOfPieces(int, int, int);
bool is_full(void);
bool after_win(int);
inline bool my_isdigit(char);
int Find_Demarcator(std::string);
bool Output(void);
bool Input(int, int);
bool Scorelist(int, int);
void startgame(void);
void restartgame(void);
bool ManVsMan();
bool man2man_move_drop(int);
bool draw_board();
bool draw_dropout(int);
bool init_board_interface(int);

inline int my_abs(int T)
{
	return abs(T);
}

bool Chess::init_board_array()
{
	for (int i = 0; i < NumOfLines; i++)
		memset(chess_board[i], 0, sizeof(chess_board[i]));
	return true;
}

bool init_board_interface(int function)
{
	system("cls");
	gotoxy(70, 2);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN);
	if (function == 3)
	{
		std::cout << "Man VS Man";
		Red.init_board_array();
		Blue.init_board_array();
	}
	else if(function==2||function==1)
	{
		std::cout << "Man VS AI";
		AM.init_board_array();
	}
	else if (function == 5)
	{
		std::cout << "AI VS AI";
	}
	gotoxy(65, 5);
	std::cout << "press the SPACE to drop，use the DIRECTION key to move the cursor\n";
	gotoxy(65, 7);
	std::cout << "press ESC to restart the game(Ai VS Ai are not allowed)";
	gotoxy(70, 10);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	if (function == 3)
	{
		std::cout << "Red number of chess pieces: ";
		gotoxy(70, 12);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "Blue number of chess piecs: ";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	draw_board();
	cursor.X = 4 * ((NumOfLines - 1) / 2);
	cursor.Y = NumOfLines - 1;
	gotoxy(cursor.X, cursor.Y);
	return true;
}

inline bool my_isdigit(char ch)
{
	return std::isdigit(static_cast<unsigned char>(ch));
}

int Find_Demarcator(std::string Source)
{
	int i = 0;
	while (true)
	{
		if (my_isdigit(Source[i]))
			i++;
		else
			break;
	}
	i++;
	return i;
}

int read_score(const char *Source)
{
	int score = 0, temp;
	for (int j = 0; j < 3; j++)
	{
		if (Source[j] == ' ')
			return score;
		if (j != 0 && Source[j] == '0')
			score *= 10;
		else
		{
			temp = Source[j] - '0';
			score = score*10 + temp;
		}
	}
	return score;
}

bool Sort_Scorelist()
{
	int count = 0;
	std::fstream f("winnerlist.txt", std::ios::out | std::ios::in);
	if (f.fail())
		throw std::runtime_error("output file fail!");
	while (!f.eof())
		if (f.get() == '\n')
			count++;
	f.close();
	f.open("winnerlist.txt", std::ios::out | std::ios::in);
	char **p = NULL;
	p = new char *[count];
	for (int i = 0; i < count; i++)
		p[i] = new char[100];
	int TempNumOfRow = 0;
	while (!f.eof() && TempNumOfRow < count)
	{
		for (int j = 0; j < 100; j++)
		{
			p[TempNumOfRow][j] = f.get();
			if (p[TempNumOfRow][j] == '\n')
			{
				p[TempNumOfRow][j + 1] = '\0';
				break;
			}
		}
		TempNumOfRow++;
	}
	for (int i = 0; i < count; i++)
	{
		for (int j = i + 1; j < count; j++)
		{
			if (read_score(p[i]) < read_score(p[j]))
			{
				char *temp = p[i];
				p[i] = p[j];
				p[j] = temp;
			}
		}
	}
	f.close();
	f.open("winnerlist.txt", std::ios::out);
	f.close();
	f.open("winnerlist.txt", std::ios::out | std::ios::in);
	for (int i = 0; i < count; i++)
		for (int j = 0; j < strlen(p[i]); j++)
			f.put(p[i][j]);
	for (int i = 0; i < count; i++)
		delete[] p[i];
	delete[]p;
	f.close();
	return true;
}

bool Output(void)
{
	system("cls");
	std::fstream outf("winnerlist.txt", std::ios::out | std::ios::in);
	if (outf.fail())
		throw std::runtime_error("output file fail!");
	std::cout << "Rank   \t\tScore\t\tColor\t\tName\n";
	char s[100];
	int rank = 1;
	while (!outf.eof())
	{
		outf.getline(s, 100);
		std::string str = "";
		for (int i = 0; s[i] != '\0'; ++i)
			str += s[i];
		try {
			int score_point = Find_Demarcator(str), LengthOfRow = str.size(), player_point;
			std::string score, name;
			score = str.substr(0, score_point);
			std::string str2 = str.substr(score_point, LengthOfRow);
			player_point = Find_Demarcator(str2);
			std::string player = str2.substr(0, player_point);
			name = str2.substr(player_point, str2.size());
			printf("%-5d\t\t%-5s\t\t%1s\t\t%-18s\n", rank++, score.c_str(), player.c_str(), name.c_str());
		}
		catch (...) { break; }
	}
	outf.close();
	return true;
}

bool Input(int score, int player)
{
	std::fstream inf("winnerlist.txt", std::ios::app);
	std::string tempname, tempscore = "";
	while (true)
	{
		std::cin >> tempname;
		getchar();
		if (tempname != ""&&tempname.find(' ') == std::string::npos)
			break;
		gotoxy(70,22);
		std::cout << "input again: ";
	}
	if (inf.fail())
		throw std::runtime_error("Input file fail!");
	while (score / 10 != 0 || score % 10 != 0)
	{
		tempscore.push_back(score % 10 + '0');
		score /= 10;
	}
	std::reverse(tempscore.begin(), tempscore.end());
	for (int i = 0; i < tempscore.size(); ++i)
		inf.put(tempscore.at(i));
	inf.put(' ');
	if (player == 1)
		inf.put('R');
	else if (player == 2)
		inf.put('B');
	else
		inf.put('M');
	inf.put(' ');
	for (int i = 0; i < tempname.size(); ++i)
		inf.put(tempname.at(i));
	inf.put('\n');
	inf.close();
	return true;
}

bool Scorelist(int score, int player)
{
	gotoxy(60, 19);
	std::cout << "Your scores are " << score;
	gotoxy(60, 20);
	std::cout << "Please input Your Name: ";
	gotoxy(60, 22);
	Input(score, player);
	Sort_Scorelist();
	Output();
	system("pause");
	return true;
}

void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

inline bool Chess::dropout(int x, int y)
{
	return chess_board[y][x] = 1;
}

int CalSumPieces(Chess T)
{
	int sum = 0;
	for (int i = 0; i < NumOfLines; ++i)
		for (int j = 0; j < NumOfLines; ++j)
			if (T.chess_board[i][j])
				sum++;
	return sum;
}

bool ShowNumOfPieces(int x, int y, int func)
{
	gotoxy(100, 10);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	if (func == 1)
	{
		std::cout << CalSumPieces(Red);
		gotoxy(100, 12);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << CalSumPieces(Blue);
	}
	/*else
	{
		std::cout << CalSumPieces(Man);
		gotoxy(100, 12);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << CalSumPieces(AI);
	}*/
	gotoxy(x, y);
	return true;
}

bool is_full(void)
{
	if (CalSumPieces(Red) + CalSumPieces(Blue) == 225)
		return true;
	return false;
}

bool Chess::win() const
{
	int i, j, count;
	for (i = 0; i <= NumOfLines; i++)
	{ //judge by row
		j = count = 0;
		while (count < 5 && j <= NumOfLines)
		{
			if ( my_abs(chess_board[i][j]) )
				count++;
			else
				count = 0;
			j++;
		}
		huo5
	}
	for (j = 0; j <= NumOfLines; j++)
	{ //judge by column
		i = count = 0;
		while (count < 5 && i <= NumOfLines)
		{
			if (chess_board[i][j])
				count++;
			else
				count = 0;
			i++;
		}
		huo5
	}
	for (i = 0; i < NumOfLines - 3; i++)
	{ //judge by declivity
		j = count = 0;
		while (count < 5 && j < NumOfLines - 3)
		{
			if (chess_board[i + count][j + count])
				count++;
			else
			{
				count = 0;
				j++;
			}
		}
		huo5
	}
	for (i = 4; i <= NumOfLines; i++) //i=4 cause starting judge from the 4th row
	{								   //judge by acclivity
		j = count = 0;
		while (count < 5 && j < NumOfLines - 3)
		{ //j<ColumnsOfBoard-3 because stop judge at 12th column if there's no dropout
			if (chess_board[i - count][j + count])
				count++;
			else
			{
				count = 0;
				j++;
			}
		}
		huo5
	}
	return 0;
}

bool Chess::AM_Win(int player)
{
	int i, j, count;
	if (player == 1)
	{
		for (i = 0; i <= NumOfLines; i++)
		{ //judge by row
			j = count = 0;
			while (count < 5 && j <= NumOfLines)
			{
				if (my_abs(chess_board[i][j]) == 1)
					count++;
				else
					count = 0;
				j++;
			}
			huo5
		}
		for (j = 0; j <= NumOfLines; j++)
		{ //judge by column
			i = count = 0;
			while (count < 5 && i <= NumOfLines)
			{
				if (my_abs(chess_board[i][j]) == 1)
					count++;
				else
					count = 0;
				i++;
			}
			huo5
		}
		for (i = 0; i < NumOfLines - 3; i++)
		{ //judge by declivity
			j = count = 0;
			while (count < 5 && j < NumOfLines - 3)
			{
				if (my_abs(chess_board[i + count][j + count]) == 1)
					count++;
				else
				{
					count = 0;
					j++;
				}
			}
			huo5
		}
		for (i = 4; i <= NumOfLines; i++) //i=4 cause starting judge from the 4th row
		{								   //judge by acclivity
			j = count = 0;
			while (count < 5 && j < NumOfLines - 3)
			{ //j<ColumnsOfBoard-3 because stop judge at 12th column if there's no dropout
				if (my_abs(chess_board[i - count][j + count]) == 1)
					count++;
				else
				{
					count = 0;
					j++;
				}
			}
			huo5
		}
	}
	else
	{
		for (i = 0; i <= NumOfLines; i++)
		{ //judge by row
			j = count = 0;
			while (count < 5 && j <= NumOfLines)
			{
				if (my_abs(chess_board[i][j]) == 2)
					count++;
				else
					count = 0;
				j++;
			}
			huo5
		}
		for (j = 0; j <= NumOfLines; j++)
		{ //judge by column
			i = count = 0;
			while (count < 5 && i <= NumOfLines)
			{
				if (my_abs(chess_board[i][j]) == 2)
					count++;
				else
					count = 0;
				i++;
			}
			huo5
		}
		for (i = 0; i < NumOfLines - 3; i++)
		{ //judge by declivity
			j = count = 0;
			while (count < 5 && j < NumOfLines - 3)
			{
				if (my_abs(chess_board[i + count][j + count]) == 2)
					count++;
				else
				{
					count = 0;
					j++;
				}
			}
			huo5
		}
		for (i = 4; i <= NumOfLines; i++) //i=4 cause starting judge from the 4th row
		{								   //judge by acclivity
			j = count = 0;
			while (count < 5 && j < NumOfLines - 3)
			{ //j<ColumnsOfBoard-3 because stop judge at 12th column if there's no dropout
				if (my_abs(chess_board[i - count][j + count]) == 2)
					count++;
				else
				{
					count = 0;
					j++;
				}
			}
			huo5
		}
	}
	return false;
}

int ai2man_win()
{
	int i, j, count;
	for (i = 0; i <= NumOfLines; i++)
	{ //judge by row
		j = count = 0;
		while (count < 5 && j <= NumOfLines)
		{
			if (my_abs(chess_board[i][j]) == 1)	count++;
			else count = 0;
			j++;
		}
		huo5
	}
	for (j = 0; j <= NumOfLines; j++)
	{ //judge by column
		i = count = 0;
		while (count < 5 && i <= NumOfLines)
		{
			if (my_abs(chess_board[i][j]) == 1) count++;
			else count = 0;
			i++;
		}
		huo5
	}
	for (i = 0; i < NumOfLines - 3; i++)
	{ //judge by declivity
		j = count = 0;
		while (count < 5 && j < NumOfLines - 3)
		{
			if (my_abs(chess_board[i + count][j + count]) == 1) count++;
			else { count = 0; j++; }
		}
		huo5
	}
	for (i = 4; i <= NumOfLines; i++) //i=4 cause starting judge from the 4th row
	{								   //judge by acclivity
		j = count = 0;
		while (count < 5 && j < NumOfLines - 3)
		{ //j<ColumnsOfBoard-3 because stop judge at 12th column if there's no dropout
			if (my_abs(chess_board[i - count][j + count]) == 1) count++;
			else{count = 0;j++;}
		}
		huo5
	}
	for (i = 0; i <= NumOfLines; i++)
	{ //judge by row
		j = count = 0;
		while (count < 5 && j <= NumOfLines)
		{
			if (my_abs(chess_board[i][j]) == 2) count++;
			else count = 0;
	    	j++;
		}
		huo5_1
	}
	for (j = 0; j <= NumOfLines; j++)
	{ //judge by column
		i = count = 0;
		while (count < 5 && i <= NumOfLines)
		{
			if (my_abs(chess_board[i][j]) == 2)	count++;
			else count = 0;
			i++;
		}
		huo5_1
	}
	for (i = 0; i < NumOfLines - 3; i++)
	{ //judge by declivity
		j = count = 0;
		while (count < 5 && j < NumOfLines - 3)
		{
			if (my_abs(chess_board[i + count][j + count]) == 2) count++;
			else { count = 0; j++; }
		}
		huo5_1
	}
	for (i = 4; i <= NumOfLines; i++) //i=4 cause starting judge from the 4th row
	{								   //judge by acclivity
		j = count = 0;
		while (count < 5 && j < NumOfLines - 3)
		{ //j<ColumnsOfBoard-3 because stop judge at 12th column if there's no dropout
			if (my_abs(chess_board[i - count][j + count]) == 2) count++;
			else { count = 0; j++; }
		}
		huo5_1
	}
	return 0;
}

bool after_win(int player)
{
	gotoxy(60, 16);
	if (player == 1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cout << "Red Win!!!";
	}
	else if (player == 2)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "Blue Win!!!";
	}
	else if (player == 3)
		std::cout << "You Win!!!";
	gotoxy(60, 18);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	std::cout << "Press the ENTER to leave your name";
	while (_getch() != 13);
	return true;
}

bool reverse(int *row, int len)
{
	int temp, i, j;
	for (i = 0, j = len - 1; i <= j; i++, j--)
	{
		temp = row[i];
		row[i] = row[j];
		row[j] = temp;
	}
	return true;
}

bool man2man_move_drop(int player)
{
	int i, state, direction;
	while (state = _getch())
	{
		switch (state)
		{
		case 27://ESC 
			std::cin.sync();
			system("cls");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
			startgame();
			break;
		case 32: //use the space dropping
			if (!Red.chess_board[cursor.X / 4][cursor.Y / 2] && !Blue.chess_board[cursor.X / 4][cursor.Y / 2])
				draw_dropout(player);
			else
				continue;
			if (player == 1)
				Red.chess_board[cursor.X / 4][cursor.Y / 2] = 1;
			else
				Blue.chess_board[cursor.X / 4][cursor.Y / 2] = 1;
			ShowNumOfPieces(cursor.X, cursor.Y, 1);
			if (is_full())
			{
				state = 27;
				continue;
			}
			return 1;
		case 224: //move the cursor
			direction = _getch();
			switch (direction)
			{
			case 72: //up
				for (i = 1; cursor.Y - 2 * i >= 0; i++)
					if (!Red.chess_board[cursor.X / 4][(cursor.Y - 2 * i) / 2] && !Blue.chess_board[cursor.X / 4][(cursor.Y - 2 * i) / 2])
					{
						cursor.Y -= 2 * i;
						break;
					}
				break;
			case 80: //down
				for (i = 1; cursor.Y + 2 * i <= (NumOfLines - 1) * 2; i++)
				{
					if (!Red.chess_board[cursor.X / 4][(cursor.Y + 2 * i) / 2] && !Blue.chess_board[cursor.X / 4][(cursor.Y + 2 * i) / 2])
					{
						cursor.Y += 2 * i;
						break;
					}
				}
				break;
			case 75: //left
				for (i = 1; cursor.X - 4 * i >= 0; i++)
				{
					if (!Red.chess_board[(cursor.X - 4 * i) / 4][cursor.Y / 2] && !Blue.chess_board[(cursor.X - 4 * i) / 4][cursor.Y / 2])
					{
						cursor.X -= 4 * i;
						break;
					}
				}
				break;
			case 77: //right
				for (i = 1; cursor.X + 4 * i <= (NumOfLines - 1) * 4; i++)
				{
					if (!Red.chess_board[(cursor.X + 4 * i) / 4][cursor.Y / 2] && !Blue.chess_board[(cursor.X - 4 * i) / 4][cursor.Y / 2])
					{
						cursor.X += 4 * i;
						break;
					}
				}
				break;
			}
			gotoxy(cursor.X, cursor.Y);
		}
	}
	return true;
}

bool draw_board()
{ // draw the chessboard
	gotoxy(0, 0);
	std::cout << "┏━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┯━━━┓\n";
	for (int i = 0; i < NumOfLines - 2; i++)
	{
		std::cout << "┃   │   │   │   │   │   │   │   │   │   │   │   │   │   ┃\n";
		std::cout << "┠───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┨\n";
	}
	std::cout << "┃   │   │   │   │   │   │   │   │   │   │   │   │   │   ┃\n";
	std::cout << "┗━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┷━━━┛\n";
	return true;
}

bool draw_dropout(int player)
{
	if (player == 1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cout << "●\b";
	}
	else if (player == 2)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "◆\b";
	}
	return true;
}

bool ManVsMan()
{
	Red.init_board_array();
	Blue.init_board_array();
	init_board_interface(3);
	while (true)
	{
		man2man_move_drop(1);//Red drop 1
		if (Red.win())
		{
			after_win(1);
			Scorelist((NumOfLines*NumOfLines) - CalSumPieces(Red), 1);
			break;
		}
		man2man_move_drop(2);//Blue drop 2
		if (Blue.win())
		{
			after_win(2);
			Scorelist((NumOfLines*NumOfLines) - CalSumPieces(Blue), 2);
			break;
		}
	}
	return true;
}

int draw_pieces(int i, int j)
{
	gotoxy(i * 4, j * 2);
	if (chess_board[i][j] == 1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cout << "●\b";
		return 0;
	}
	if (chess_board[i][j] == 2)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "●\b";
		return 0;
	}
	return 0;
}

void init_board_array()
{
	if (player_state != 1 && player_state != 2)
		return;
	player0 = player_state;
	int i, j;
	for (i = 0; i < NumOfLines; i++)
		for (j = 0; j < NumOfLines; j++)
			chess_board[i][j] = 0;
}

bool inboard(int row, int col)
{
	if (row < 0 || row >= NumOfLines)return false;
	return col >= 0 && col < NumOfLines;
}

bool same(int row, int col, int key)
{
	if (!inboard(row, col))return false;
	return (chess_board[row][col] == key || chess_board[row][col] + key == 0);
}

int num(int row, int col, int u)
{
	int i = row + dx[u], j = col + dy[u], sum = 0, ref = chess_board[row][col];
	if (ref == 0)return 0;
	while (same(i, j, ref))sum++, i += dx[u], j += dy[u];
	return sum;
}

int live4(int row, int col)//live 4 's number
{
	int key = chess_board[row][col], sum = 0, i, u;
	for (u = 0; u < 4; u++)//4 directions 
	{
		int sumk = 1;
		sumkadd off sumksub off
			if (sumk == 4)sum++;
	}
	return sum;
}
int chong4(int row, int col)
{
	int key = chess_board[row][col], sum = 0, i, u;
	for (u = 0; u < 8; u++)
	{
		int sumk = 0;
		bool flag = true;
		for (i = 1; samekey || flag; i++)
		{
			if (!samekey)
			{
				if (flag&&chess_board[row + dx[u] * i][col + dy[u] * i])sumk -= 10;
				flag = false;
			}
			sumk++;
		}
		if (!inboard(row + dx[u] * --i, col + dy[u] * i))
			continue;
		sumksub
			if (sumk == 4)sum++;
	}
	return sum - live4(row, col) * 2;
}
int live3(int row, int col)
{
	int key = chess_board[row][col], sum = 0, i, u;
	for (u = 0; u < 4; u++)//san lian de huo san
	{
		int sumk = 1;
		sumkadd off i++; off;
		sumksub off i--; off;
		if (sumk == 3)sum++;
	}
	for (u = 0; u < 8; u++)//8个方向，每个方向最多1个非三连的活三
	{
		int  sumk = 0;
		bool flag = true;
		for (i = 1; samekey || flag; i++)//成活四点的方向
		{
			if (!samekey)
			{
				if (flag&&chess_board[row + dx[u] * i][col + dy[u] * i])sumk -= 10;
				flag = false;
			}
			sumk++;
		}
		off
			if (chess_board[row + dx[u] * --i][col + dy[u] * i] == 0)continue;
		sumkadd off
			if (sumk == 3)sum++;
	}
	return sum;
}
bool overline(int row, int col)//长连禁手
{
	bool flag = false;
	int u;
	for (u = 0; u < 4; u++)if (num(row, col, u) + num(row, col, u + 4) > 4)flag = true;
	return flag;
}
bool ban(int row, int col)//判断落子后是否成禁手
{
	if (same(row, col, 2))return false;//白方无禁手
	bool flag = live3(row, col) > 1 || overline(row, col) || live4(row, col) + chong4(row, col) > 1;
	return flag;
}

bool end_(int row, int col)//落子之后是否游戏结束
{
	int u;
	for (u = 0; u < 4; u++)if (num(row, col, u) + num(row, col, u + 4) >= 4)is_end = true;
	if (is_end)return true;
	is_end = ban(row, col);
	return is_end;
}

void ai_message(int state)
{
	int x = cursor.X, y = cursor.Y;
	gotoxy(70, 18);
	if (state == 1) std::cout << "AI考虑中！";
	else std::cout << "AI已落子！";
}

void ai_drop(int row, int col)
{
	if (player_state == player0)chess_board[row][col] = 1;//1代表红棋
	else chess_board[row][col] = 2;//2代表蓝棋
	draw_pieces(row, col);
	ai_message(2);//提示AI已经落子
	gotoxy(row * 4, col * 2);
	if (ban(row, col) || end_(row, col))
	{
		gotoxy(60, 16);
		if (player0 == 1 || player_state != ais)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
			std::cout << "红方胜利！！！";
		}
		else
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
			std::cout << "蓝方胜利！！！";
		}
		system("pause");
	}

}

inline bool ok(int row, int col)
{
	return inboard(row, col) && (chess_board[row][col] == 0);
}

int point(int row, int col)//非负分值
{
	if (ban(row, col))return 0;
	if (end_(row, col))
	{
		is_end = false;
		return 10000;
	}
	int ret = live4(row, col) * 1000 + (chong4(row, col) + live3(row, col)) * 100, u;
	for (u = 0; u < 8; u++)if (chess_board[row + dx[u]][col + dy[u]])ret++;
	return ret;
}

int AI3(int p2)
{
	int i, j;
	int keyp = -100000, temppoint;
	for (i = 0; i < NumOfLines; i++)
		for (j = 0; j < NumOfLines; j++)
		{
			if (!ok(i, j))continue;
			chess_board[i][j] = player0;
			temppoint = point(i, j);
			if (temppoint == 0)
			{
				chess_board[i][j] = 0;
				continue;
			}
			if (temppoint == 10000)
			{
				chess_board[i][j] = 0;
				return 10000;
			}
			chess_board[i][j] = 0;
			if (temppoint - p2 * 2 > keyp)keyp = temppoint - p2 * 2;//第三层取极大
		}
	return keyp;
}

int AI2()
{
	int i, j;
	int keyp = 100000, temppoint;
	for (i = 0; i < NumOfLines; i++)
		for (j = 0; j < NumOfLines; j++)
		{
			if (!ok(i, j))continue;
			chess_board[i][j] = 3 - player0;
			temppoint = point(i, j);
			if (temppoint == 0)
			{
				chess_board[i][j] = 0;
				continue;
			}
			if (temppoint == 10000)
			{
				chess_board[i][j] = 0;
				return -10000;
			}
			temppoint = AI3(temppoint);
			chess_board[i][j] = 0;
			if (temppoint < keyp)
				keyp = temppoint;//第二层取极小
		}
	return keyp;
}

void AI()
{
	ai_message(1);
	if (chess_board[7][7] == 0)	return ai_drop(7, 7);
	int i, j;
	int keyp = -100000, keyi, keyj, temppoint;
	for (i = 0; i < NumOfLines; i++)
	{
		for (j = 0; j < NumOfLines; j++)
		{
			if (!ok(i, j))continue;
			chess_board[i][j] = player0;
			temppoint = point(i, j);
			if (temppoint == 0)
			{
				chess_board[i][j] = 0;
				continue;
			}//剪枝,不选择禁手点和无效点
			if (temppoint == 10000)
				return ai_drop(i, j);
			temppoint = AI2();
			chess_board[i][j] = 0;
			if (temppoint > keyp)
				keyp = temppoint, keyi = i, keyj = j;//第一层取极大
		}
	}
	return ai_drop(keyi, keyj);
}

bool player_drop()
{
	int i, state, direction;
	while (state = _getch())
	{
		switch (state)
		{
		case 27://ESC键可随时退出当前游戏
			system("cls");
			startgame();
			//TODO
			break;
		case 32: //按下空格先检验再落子再绘出棋子
			if (!chess_board[cursor.X / 4][cursor.Y / 2])
			{
				if (player_state == player0)chess_board[cursor.X / 4][cursor.Y / 2] = 1;//1代表刚刚下了红棋
				else chess_board[cursor.X / 4][cursor.Y / 2] = 2;//2代表刚刚下了蓝棋
				draw_pieces(cursor.X / 4, cursor.Y / 2);
			}
			else continue;
			return true;
		case 224: //move the cursor
			direction = _getch();
			switch (direction)
			{
			case 72: //up
				for (i = 1; cursor.Y - 2 * i >= 0; i++)
					if (!chess_board[cursor.X / 4][(cursor.Y - 2 * i) / 2])
					{
						cursor.Y -= 2 * i;
						break;
					}
				break;
			case 80: //down
				for (i = 1; cursor.Y + 2 * i <= (NumOfLines - 1) * 2; i++)
				{
					if (!chess_board[cursor.X / 4][(cursor.Y + 2 * i) / 2])
					{
						cursor.Y += 2 * i;
						break;
					}
				}
				break;
			case 75: //left
				for (i = 1; cursor.X - 4 * i >= 0; i++)
				{
					if (!chess_board[(cursor.X - 4 * i) / 4][cursor.Y / 2])
					{
						cursor.X -= 4 * i;
						break;
					}
				}
				break;
			case 77: //right
				for (i = 1; cursor.X + 4 * i <= (NumOfLines - 1) * 4; i++)
				{
					if (!chess_board[(cursor.X + 4 * i) / 4][cursor.Y / 2])
					{
						cursor.X += 4 * i;
						break;
					}
				}
				break;
			}
			gotoxy(cursor.X, cursor.Y);
		}
	}
	return true;
}

bool ManVsAI(int a)
{
	player_state = a;
	player0 = player_state;
	init_board_interface(2);
	init_board_array();
	is_end = false;
	draw_board();
	gotoxy(28, 14);//(28/4,14/2)为棋盘中心
	while (!is_end)
	{
		if (player_state == ais)AI();
		else player_drop();
		if (ai2man_win())
		{
			if (a == 1 && ai2man_win() == 1 || (a == 2 && ai2man_win() == 2)) restartgame();
			else if (a == 1 && ai2man_win() == 2)
			{
				gotoxy(60, 16);
				std::cout << "You Win!";
				gotoxy(60, 18);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				std::cout << "Press the ENTER to leave your name";
				int res = 0;
				for (int i = 0; i < NumOfLines; i++)
					for (int j = 0; j < NumOfLines; j++)
						if (chess_board[i][j] == 2)
							res++;
				while (_getch() != 13);			
				Scorelist(res, 3);
				startgame();
			}
			else if (a == 2 && ai2man_win() == 1)
			{
				gotoxy(60, 16);
				std::cout << "You Win!";
				gotoxy(60, 18);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				std::cout << "Press the ENTER to leave your name";
				while (_getch() != 13);
				int res = 0;
				for (int i = 0; i < NumOfLines; i++)
					for (int j = 0; j < NumOfLines; j++)
						if (chess_board[i][j] == 1)
							res++;
				Scorelist(res, 3);
				startgame();
			}
		}
		player_state = 3 - player_state;
	}
	return true;
}

bool full()
{
	for (int i = 0; i < NumOfLines; i++)
		for (int j = 0; j < NumOfLines; j++)
			if (!chess_board[i][j])
				return false;
	return true;
}

bool AIvsAI()
{
	player_state = 1;
	player0 = player_state;
	init_board_interface(5);
	init_board_array();
	is_end = false;
	draw_board();
	gotoxy(28, 14);//(28/4,14/2)为棋盘中心
	while (!is_end)
	{
		if (full())
		{
			restartgame();
			break;
		}
		AI();
		player_state = 3 - player_state;
	}
	restartgame();
	return true;
}

void restartgame(void)
{
	gotoxy(60, 18);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	std::cout << "Press ENTER to restart game ";
	while (_getch() != 13);
	std::cin.sync();
	startgame();
}

void startgame(void)
{
	system("cls");
	std::cin.sync();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
	std::cout << "--------------------------------------------------------\n";
	std::cout << "|                  Five In A Row                       |\n";
	std::cout << "|Function:                                             |\n";
	std::cout << "|          1.Man Vs AI (AI first)                      |\n";
	std::cout << "|          2.Man Vs AI  (Man first)                    |\n";
	std::cout << "|          3.Man Vs Man                                |\n";
	std::cout << "|          4.Look up the scorelist                     |\n";
	std::cout << "|          5.AI Vs AI                                  |\n";
	std::cout << "|      When playing ,press ESC to quit the Game        |\n";
	std::cout << "--------------------------------------------------------\n";
	while (true)
	{
		switch (_getch())
		{
		case '1':
			ManVsAI(1);//AI先下
			break;
		case '2':
			ManVsAI(2);//Man first drops
			break;
		case '3':
			ManVsMan();
			break;
		case '4':
			Sort_Scorelist();
			Output();
			system("pause");
			break;
		case '5':
			AIvsAI();
			break;
		default:
			std::cout << "input again: ";
			break;
		}
		std::cin.sync();
		startgame();
	}
}

int main()
{
	startgame();
	return 0;
}
