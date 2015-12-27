#include<stdio.h>
#include<conio.h>
#include<iostream>
# include <fstream>
# include <string>
#include <windows.h>

using namespace std;


#define FOREGROUND_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
class TextAttr {
	friend std::ostream& operator<<(std::ostream& out, TextAttr attr)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attr.value);
		return out;
	}
public:
	explicit TextAttr(WORD attributes) : value(attributes) {}
private:
	WORD value;
};

void gotoxy(int x, int y)
{
	HANDLE hConsoleOutput;
	COORD dwCursorPosition;

	cout.flush();
	dwCursorPosition.X = x;
	dwCursorPosition.Y = y;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}

void createwindow(int Width, int Height)
{
	_COORD coord;
	coord.X = Width;
	coord.Y = Height;

	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = Height - 1;
	Rect.Right = Width - 1;

	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);      // Get Handle 
	SetConsoleScreenBufferSize(Handle, coord);            // Set Buffer Size 
	SetConsoleWindowInfo(Handle, TRUE, &Rect);            // Set Window Size 
}


class cell
{
	char* data;
	int datalength;
public:
	void setdataH()
	{
		string s;
		cin >> s;
		datalength = s.size() + 1;
		data = new char[s.size() + 1];
		data[s.size()] = 0;
		memcpy(data, s.c_str(), s.size());
	}
	void setdataF(string in)
	{
		datalength = in.size() + 1;
		data = new char[in.size() + 1];
		data[in.size()] = 0;
		memcpy(data, in.c_str(), in.size());
	}
	char* getdata()
	{
		return data;
	}
	int getdatalength()
	{
		return datalength;
	}
	void printdata()
	{
		int i = 0;
		for (i = 0; i<datalength; i++)
		{
			cout << data[i];
		}
	}
};

class page
{
	cell*** pagetable;
	static int axisx;
	static int axisy;
public:
	void initializepagetable(int nrows, int ncolumns)
	{
		int i;
		int j = 0;
		pagetable = (cell***)malloc(nrows * sizeof(cell**));
		for (i = 0; i<nrows; i++)
		{
			pagetable[i] = (cell**)malloc(ncolumns*sizeof(cell*));
		}

		for (i = 0; i<nrows; i++)
		{
			for (j = 0; j<ncolumns; j++)
			{
				pagetable[i][j] = NULL;
			}
		}

	}
	cell*** getpagetable()
	{
		return pagetable;
	}
	static int getaxisx()
	{
		return axisx;
	}
	static int getaxisy()
	{
		return axisy;
	}
	page()
	{
		initializepagetable(axisy, axisx);
	}

};

int page::axisx = 9;
int page::axisy = 10;

class sheet
{
	page*** sheettable;
	static int maxwidth;
	static int maxheight;
public:
	void initializesheettable(int nrows, int ncolumns)
	{
		int i = 0;
		int j = 0;
		sheettable = (page***)malloc(nrows * sizeof(page**));
		for (i = 0; i<nrows; i++)
		{
			sheettable[i] = (page**)malloc(ncolumns*sizeof(page*));
		}

		for (i = 0; i<nrows; i++)
		{
			for (j = 0; j<ncolumns; j++)
			{
				sheettable[i][j] = NULL;
			}
		}
	}
	page*** getsheettable()
	{
		return sheettable;
	}
	static int getmaxwidth()
	{
		return maxwidth;
	}
	static int getmaxheight()
	{
		return maxheight;
	}
	sheet()
	{
		initializesheettable(maxheight, maxwidth);
	}
};

int sheet::maxwidth = 12;
int sheet::maxheight = 26;

class controller
{
	sheet s1;
	int cpsx;
	int cpsy;
	int cppx;
	int cppy;
	int x;
	int y;
	int xx;
	int yy;
	page* currentpage;
	ofstream myoutput;
	bool readmode;
public:
	void inputpage(int rows, int columns)
	{
		s1.getsheettable()[rows][columns] = new page;
	}
	page* returnpage(int rows, int columns)
	{
		return s1.getsheettable()[rows][columns];
	}
	void inputcell(int rows, int columns, int rowp, int columnp)
	{
		s1.getsheettable()[rows][columns]->getpagetable()[rowp][columnp] = new cell;
	}
	void enterdata(int rows, int columns, int rowp, int columnp)
	{
		s1.getsheettable()[rows][columns]->getpagetable()[rowp][columnp]->setdataH();
	}
	void enterdataF(int rows, int columns, int rowp, int columnp, string in)
	{
		s1.getsheettable()[rows][columns]->getpagetable()[rowp][columnp]->setdataF(in);
	}
	char* getdata(int rows, int columns, int rowp, int columnp)
	{
		return s1.getsheettable()[rows][columns]->getpagetable()[rowp][columnp]->getdata();
	}

	int getdatalength(int rows, int columns, int rowp, int columnp)
	{
		return s1.getsheettable()[rows][columns]->getpagetable()[rowp][columnp]->getdatalength();
	}
	bool checkcell(int rows, int columns, int rowp, int columnp)
	{
		if (s1.getsheettable()[rows][columns]->getpagetable()[rowp][columnp] == NULL)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	bool checkpage(int rows, int columns)
	{
		if (s1.getsheettable()[rows][columns] == NULL)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	void setreadmodeT()
	{
		readmode = true;
	}

	void setreadmodeF()
	{
		readmode = false;
	}

	bool getreadmode()
	{
		return readmode;
	}

	void printcell(int rows, int columns, int rowp, int columnp)
	{
		char* temp = s1.getsheettable()[rows][columns]->getpagetable()[rowp][columnp]->getdata();
		int templength = s1.getsheettable()[rows][columns]->getpagetable()[rowp][columnp]->getdatalength();
		int i = 0;
		if (templength > 4)
		{
			cout << "|";
			for (i = 0; i<4; i++)
			{
				cout << temp[i];
			}
			cout << "__|";
		}
		else if (templength == 3)
		{
			cout << "|";
			for (i = 0; i<3; i++)
			{
				cout << temp[i];
			}
			cout << "___|";
		}
		else if (templength == 2)
		{
			cout << "|";
			for (i = 0; i<2; i++)
			{
				cout << temp[i];
			}
			cout << "__|";
		}
		else if (templength == 1)
		{
			cout << "|";
			for (i = 0; i<1; i++)
			{
				cout << temp[i];
			}
			cout << "______|";
		}
	}

	void displaypage(int y, int x)
	{
		gotoxy(0, 0);
		printf("\n");
		int xaxis = 0;
		int yaxis = 0;
		for (xaxis = 0; xaxis < page::getaxisx(); xaxis++)//should this be legal?
		{
			printf("\t   %d", x);
			x++;
		}
		printf("\t");
		for (int i = 0; i < page::getaxisy(); i++)
		{
			printf("%d \t", y);
			for (int j = 0; j < page::getaxisx(); j++)
			{
				if (currentpage->getpagetable()[i][j] == NULL)
				{
					printf("|______|");
				}
				else
				{
					printcell(cpsy, cpsx, i, j);// << TextAttr(FOREGROUND_WHITE);
				}
			}
			y++;
		}

	}

	void uncolour_current_cell()
	{
		gotoxy(x, y);
		if (checkcell(cpsy, cpsx, cppy, cppx) == true)
		{
			printcell(cpsy, cpsx, cppy, cppx);
		}
		else
		{
			cout << "|______|";
		}
		gotoxy(x + 1, y);
	}

	void highlight_new_cell()
	{
		gotoxy(x, y);
		if (checkcell(cpsy, cpsx, cppy, cppx) == true)
		{
			cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
			printcell(cpsy, cpsx, cppy, cppx);
			cout << TextAttr(FOREGROUND_WHITE);
		}
		else
		{
			cout << TextAttr(FOREGROUND_INTENSITY | FOREGROUND_GREEN) << "|______|" << TextAttr(FOREGROUND_WHITE);
		}
		gotoxy(x + 1, y);
	}

	void displaynewpage()
	{
		uncolour_current_cell();
		currentpage = s1.getsheettable()[cpsy][cpsx];
		gotoxy(0, 0);
		displaypage(yy, xx);
		highlight_new_cell();
	}

	void cnpage()
	{
		if (checkpage(cpsy, cpsx) == false)
		{
			inputpage(cpsy, cpsx);
			currentpage = returnpage(cpsy, cpsx);
			if (readmode == true)
			{
				filing_read();
			}
		}
	}

	void tempstore(int opt)
	{
		int temp_cpsy = 0;
		int temp_cpsx = 0;
		int temp_cppy = 0;
		int temp_cppx = 0;
		int temp_x = 0;
		int temp_y = 0;
		int temp_xx = 0;
		int temp_yy = 0;

		if (opt == 0)
		{
			temp_cpsy = cpsy;
			temp_cpsx = cpsx;
			temp_cppy = cppy;
			temp_cppx = cppx;
			temp_x = x;
			temp_y = y;
			temp_xx = xx;
			temp_yy = yy;
		}
		if (opt == 1)
		{
			cpsy = temp_cpsy;
			cpsx = temp_cpsx;
			cppy = temp_cppy;
			cppx = temp_cppx;
			x = temp_x;
			y = temp_y;
			xx = temp_xx;
			yy = temp_yy;
		}
	}

	void filing_write()
	{
		myoutput.open("File.txt", ios::out);
		tempstore(0);
		for (cpsy = 0; cpsy < sheet::getmaxheight(); cpsy++)
		{
			for (cpsx = 0; cpsx < sheet::getmaxwidth(); cpsx++)
			{
				if (checkpage(cpsy, cpsx) == true)
				{
					myoutput << cpsx << "," << cpsy << endl;

					for (cppy = 0; cppy < page::getaxisy(); cppy++)
					{
						for (cppx = 0; cppx < page::getaxisx(); cppx++)
						{
							if (checkcell(cpsy, cpsx, cppy, cppx) == true)
							{
								char *c = new char[100];
								c = getdata(cpsy, cpsx, cppy, cppx);
								myoutput << c;
							}
							else
							{
								myoutput << "NULL";
							}
							myoutput << endl;
						}
					}
					myoutput << endl;
				}
			}
		}
		myoutput.close();
		tempstore(1);
	}

	void search()
	{
		for (cpsy = 0; cpsy < sheet::getmaxheight(); cpsy++)
		{
			for (cpsx = 0; cpsx < sheet::getmaxwidth(); cpsx++)
			{
				if (checkpage(cpsy, cpsx) == true)
				{
					for (cppy = 0; cppy < page::getaxisy(); cppy++)
					{
						for (cppx = 0; cppx < page::getaxisx(); cppx++)
						{
							if (checkcell(cpsy, cpsx, cppy, cppx) == true)
							{
							}
						}
					}
				}
			}
		}
	}

	void filing_read()
	{
		ifstream myInput("File.txt", ios::in | ios::beg);
		string sy = to_string(cpsy);
		string sx = to_string(cpsx);
		string find = sx + ',' + sy;
		string line;
		string lineN;
		int a = 0;
		int b = 0;
		//search find in file
		if (myInput.is_open())
		{
			while (getline(myInput, line))
			{
				if (line == find)
				{
					for (a = 0; a < page::getaxisy(); a++)
					{
						for (b = 0; b < page::getaxisx(); b++)
						{
							getline(myInput, lineN);
							if (lineN != "NULL")
							{
								inputcell(cpsy, cpsx, a, b);
								line.erase();
								enterdataF(cpsy, cpsx, a, b, lineN);
							}
						}
					}
				}
			}
		}
	}

	controller()
	{
		y = 2;
		x = 8;
		cppy = 0;
		cppx = 0;
		cpsy = 0;
		cpsx = 0;
		xx = 0;
		yy = 0;
		createwindow(80, 15);
		setreadmodeF();
		inputpage(0, 0);
		currentpage = s1.getsheettable()[0][0];
		displaypage(0, 0);
		highlight_new_cell();
		getinput();
	}

	void getinput()
	{
		int key = 0;
		while (1)
		{
			key = _getch();
			key = _getch();
			if (key == 80)//down
			{
				if (cppy == page::getaxisy() - 1)
				{
					if (cpsy < sheet::getmaxheight()-1)
					{
						cpsy++;
						cppy = 0;
						y = 2;
						yy = yy + 10;
						cnpage();
						displaynewpage();
					}
				}
				else
				{
					uncolour_current_cell();
					cppy++;
					y++;
					highlight_new_cell();
				}
			}
			else if (key == 72)//up
			{
				if (cppy == 0)//tbc
				{
					if (cpsy != 0)//tbc
					{
						cpsy--;
						cppy = page::getaxisy() - 1;
						y = 11;
						yy = yy - 10;
						cnpage();
						displaynewpage();
					}
				}
				else
				{
					uncolour_current_cell();
					cppy--;
					y--;
					highlight_new_cell();
				}
			}
			else if (key == 75)//left
			{
				if (cppx == 0)
				{
					if (cpsx != 0)
					{
						cpsx--;
						x = 72;
						cppx = page::getaxisx() - 1;
						xx = xx - 9;
						cnpage();
						displaynewpage();
					}
				}
				else
				{
					uncolour_current_cell();
					cppx--;
					x = x - 8;
					highlight_new_cell();
				}
			}
			else if (key == 77)//right
			{
				if (cppx == page::getaxisx() - 1)
				{
					if (cpsx < sheet::getmaxwidth()-1)
					{
						cpsx++;
						x = 8;
						xx = xx + 9;
						cppx = 0;
						cnpage();
						displaynewpage();
					}
				}
				else
				{
					uncolour_current_cell();
					cppx++;
					x = x + 8;
					highlight_new_cell();
				}
			}
			else if (key == 13)//enter
			{
				if (checkcell(cpsy, cpsx, cppy, cppx) == false)
				{
					inputcell(cpsy, cpsx, cppy, cppx);
				}
				enterdata(cpsy, cpsx, cppy, cppx);
				gotoxy(x + 1, y);
			}

			else if (key == 116)//ctrl+right
			{
				uncolour_current_cell();
				tempstore(0);
				while (checkcell(cpsy, cpsx, cppy, cppx) == false)
				{
					if (cppx == page::getaxisx() - 1)
					{
						if (cpsx == sheet::getmaxwidth()-1)
						{
							tempstore(1);
							break;
						}
						if (cpsx < sheet::getmaxwidth() - 1)
						{
							cpsx++;
							x = 8;
							xx = xx + 9;
							cppx = 0;
						}
					}
					else
					{
						cppx++;
						x = x + 8;
					}
				}
				displaynewpage();
				highlight_new_cell();
			}

			else if (key == 115)//ctrl + left
			{
				uncolour_current_cell();
				tempstore(0);
				while (checkcell(cpsy, cpsx, cppy, cppx) == false)
				{
					if (cppx == 0)
					{
						if (cpsx == 0)
						{
							tempstore(1);
							break;
						}
						if (cpsx != 0)
						{
							cpsx--;
							x = 72;
							cppx = page::getaxisx() - 1;
							xx = xx - 9;
						}
					}
					else
					{
						cppx--;
						x = x - 8;
					}
				}
				displaynewpage();
				highlight_new_cell();
			}

			else if (key == 141)
			{
				uncolour_current_cell();
				tempstore(0);
				while (checkcell(cpsy, cpsx, cppy, cppx) == false)
				{
					if (cppy == 0)
					{
						if (cpsy == 0)
						{
							tempstore(1);
							break;
						}
						if (cpsy > 0)//tbc
						{
							cpsy--;
							cppy = page::getaxisy() - 1;
							y = 11;
							yy = yy - 10;
						}
					}
					else
					{
						cppy--;
						y--;
					}
				}
				displaynewpage();
				highlight_new_cell();
			}

			/*else if (key == 145)
			{
				uncolour_current_cell();
				tempstore(0);
				while (checkcell(cpsy, cpsx, cppy, cppx) == false)
				{
					if (cppy == page::getaxisy - 1)
					{
						if (cpsy == sheet::getmaxheight - 1)
						{
							tempstore(1);
							break;
						}
						if (cpsy < sheet::getmaxheight())
						{
							cpsy++;
							cppy = 0;
							y = 2;
							yy = yy + 10;
						}
					}
					else
					{
						cpsy++;
						y++;
					}
				}
				displaynewpage();
				highlight_new_cell();
			}*/

			else if (key == 59)
			{
				setreadmodeT();
				filing_read();
				displaynewpage();
			}

			else if (key == 60)
			{
				filing_write();
			}
			else if (key == 61)
			{
				//exit
			}
			else if (key == 62)
			{
				//	search
			}
			else if (key == 63)
			{
				controller();
			}
		}
	}
};

int main()
{
	controller c1;
	_getch();
}