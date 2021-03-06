//Include libraries
#include "pch.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <ctype.h>


//Game settings
const int rowsCount = 10;
const int columnsCount = 15;

const unsigned char symbolHero = 2;
const unsigned char symbolWall = 177;
const unsigned char symbolBox = 254;
const unsigned char symbolExit = 176;

const unsigned char levelData0[rowsCount][columnsCount + 1] =
{
	"#####2#########",
	"#  X   #   X ##",
	"# X ### X  #  #",
	"#X X  X  ## X #",
	"# X    ##  #  #",
	"#######    # X#",
	"#   X   XX #X #",
	"#XXX # # X   ##",
	"#1 X #   X X  #",
	"###############",
};
const unsigned char levelData1[rowsCount][columnsCount + 1] =
{
	"#########2#####",
	"#X  ### # ###1#",
	"#  X#  X  #   #",
	"#X  # X  #  ###",
	"# X #   ## #  #",
	"# XX ##X##  X #",
	"# #     # ### #",
	"#  ####X  X   #",
	"#   X     # # #",
	"###############",
};


//Logics variables
HANDLE consoleHandle = 0;
bool isGameActive = true;
unsigned char levelData[rowsCount][columnsCount];
int heroRow = 0;
int heroColumn = 0;
int levelNumber = 0;


//Functions
void SetupSystem()
{
	srand(time(0));

	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}


void Initialize()
{
	//Load level
	for (int i = 0; i < rowsCount; i++)
	{
		for (int j = 0; j < columnsCount; j++)
		{
			// Init
			unsigned char symbol = 0;

			switch (levelNumber)
			{
				//First level
				case 0:
				{
					symbol = levelData0[i][j];
					break;
				}

				//Second level
				case 1:
				{
					symbol = levelData1[i][j];
					break;
				}
			}
			

			switch (symbol)
			{
				//Wall
				case '#':
				{
					levelData[i][j] = symbolWall;
					break;
				}

				//Hero
				case '1':
				{
					levelData[i][j] = symbolHero;

					//Catch hero position
					heroRow = i;
					heroColumn = j;

					break;
				}
				
				//Exit
				case '2':
				{
					levelData[i][j] = symbolExit;
					break;
				}

				//Box
				case 'X':
				{
					levelData[i][j] = symbolBox;
					break;
				}
				default:
				{
					levelData[i][j] = symbol;
					break;
				}
			}
		}
	}
}


void Render()
{
	//Move console cursor to (0,0)
	COORD cursorCoord;
	cursorCoord.X = 0;
	cursorCoord.Y = 0;
	SetConsoleCursorPosition(consoleHandle, cursorCoord);
	
	printf("\n\n\t");
	for (int i = 0; i < rowsCount; i++)
	{
		for (int j = 0; j < columnsCount; j++)
		{
			unsigned char symbol = levelData[i][j];

			switch (symbol)
			{
				//Walls - white
				case symbolWall:
				{
					SetConsoleTextAttribute(consoleHandle, 15);
					break;
				}

				//Hero - green
				case symbolHero:
				{
					SetConsoleTextAttribute(consoleHandle, 10);
					break;
				}

				//Boxes - yellow
				case symbolBox:
				{
					SetConsoleTextAttribute(consoleHandle, 14);
					break;
				}

				//Exit - red
				case symbolExit:
				{
					SetConsoleTextAttribute(consoleHandle, 12);
					break;
				}
			}

			printf("%c", symbol);
		}
		printf("\n\t");
	}

	SetConsoleTextAttribute(consoleHandle, 7);
	printf("\n\n\tUse AWSD to move ");
	SetConsoleTextAttribute(consoleHandle, 10);
	printf("Hero");
	SetConsoleTextAttribute(consoleHandle, 7);
	printf(". R - Restart level.");
}


void MoveHeroTo(int row, int column)
{
	unsigned char destinationCell = levelData[row][column];
	bool canMoveToCell = false;

	switch (destinationCell)
	{
		//Empty cell
		case ' ':
		{
			canMoveToCell = true;
			break;
		}
		
		//Exit cell
		case symbolExit:
		{
			if (levelNumber < 1)
			{
				levelNumber++;
				Initialize();
			}
			else
			{
				isGameActive = false;
			}
			
			break;
		}

		//Box cell
		case symbolBox:
		{
			//Calculate hero move direction
			int heroDirectionI = row - heroRow;
			int heroDirectionJ = column - heroColumn;

			//Check for empty space behind the box
			if (levelData[row + heroDirectionI][column + heroDirectionJ] == ' ')
			{
				canMoveToCell = true;

				//Remove box symbol from previous position
				levelData[row][column] = ' ';

				//Set box symbol to new position
				levelData[row + heroDirectionI][column + heroDirectionJ] = symbolBox;
			}

			break;
		}

	}

	if (canMoveToCell)
	{
		//Remove hero symbol from previous position
		levelData[heroRow][heroColumn] = ' ';

		//Set new hero position
		heroRow = row;
		heroColumn = column;

		//Set hero symbol on new position
		levelData[heroRow][heroColumn] = symbolHero;
	}
}


void Update()
{
	unsigned char inputChar = _getch();
	inputChar = tolower(inputChar);

	switch (inputChar)
	{
		//Up
		case 'w':
		{
			MoveHeroTo(heroRow - 1, heroColumn);
			break;
		}

		//Down
		case 's':
		{
			MoveHeroTo(heroRow + 1, heroColumn);
			break;
		}

		//Left
		case 'a':
		{
			MoveHeroTo(heroRow, heroColumn - 1);
			break;
		}

		//Right
		case 'd':
		{
			MoveHeroTo(heroRow, heroColumn + 1);
			break;
		}

		//Restart level
		case 'r':
		{
			Initialize();
			break;
		}
	}
}


void Shutdown()
{
	system("cls");
	system("pause");
}


int main()
{
	SetupSystem();
	Initialize();

	while (isGameActive)
	{
		Render();
		Update();
	}

	Shutdown();

	return 0;
}