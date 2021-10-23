#include <stdio.h>
#include<time.h>
#include<stdlib.h>
#include <conio.h>
#include <windows.h>

void RemoveCursor(void);
void gotoxy(int x, int y);
char getKey();
void make_map();
void make_NEXT();
void DEF_blocks(int array[7][4][4][4]);
int next_blocks(int array[7][4][4][4], int* next_block_num, int* next_block_tnum);
int check_block_can_down(int array[7][4][4][4], int Garray[24][22], int block_num, int block_tnum, int x, int y);
int check_block_can_goLR(int array[7][4][4][4], int Garray[24][22], int block_num, int block_tnum, int x, int y);
int check_block_can_spawn(int array[7][4][4][4], int Garray[24][22], int block_num, int block_tnum, int x, int y);
int check_block_can_spin(int array[7][4][4][4], int Garray[24][22], int block_num, int block_tnum, int x, int y);
int check_line(int Garray[24][22], int* score);
int control_blocks(int array[7][4][4][4], int Garray[24][22] ,int block_num, int block_tnum, int* score);
int down_straight(int array[7][4][4][4], int Garray[24][22], int block_num, int block_tnum, int x, int y);
int show_end(int* score);
int show_score(int* score);



int main(void)
{
	int block_num, block_tnum, next_block_num, next_block_tnum;
	int Blocks[7][4][4][4];
	int Garray[24][22];
	int result;
	int score = 0;

	for (int i = 0; i < 24; i++)
	{
		for (int j = 0; j < 22; j++)
		{
			Garray[i][j] = -1;
		}
	}
	
	system("mode con cols=40 lines=25");
	RemoveCursor();
	DEF_blocks(Blocks);
	make_map();
	make_NEXT();

	show_score(&score);
	next_blocks(Blocks, &next_block_num, &next_block_tnum);
	while (1)
	{
		block_num = next_block_num, block_tnum = next_block_tnum;
		next_blocks(Blocks, &next_block_num, &next_block_tnum);
		result = control_blocks(Blocks, Garray, block_num, block_tnum, &score);
		if (result == -1)
			break;
		check_line(Garray, &score);
		make_map();
		show_score(&score);
	}
	
	show_end(&score);

	gotoxy(0, 26);

}

int control_blocks(int array[7][4][4][4], int Garray[24][22] ,int block_num, int block_tnum, int* score)
{
	char a = NULL;
	time_t ot, nt;
	
	int time_diff = *score / 20;
	int result;
	int x = 8, y = 4, can_spawn;
	gotoxy(x, y);
	can_spawn = check_block_can_spawn(array, Garray, block_num, block_tnum, x, y);

	if (can_spawn != 1)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				gotoxy(x + j * 2, y + i);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9 + block_num);
				if (array[block_num][block_tnum][i][j] == 1)
					printf("▣");
			}
		}
		return -1;
	}
	

	
	ot = clock();
	nt = clock();

	do
	{
		nt = clock();
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				gotoxy(x + j * 2, y + i);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9 + block_num);
				if (array[block_num][block_tnum][i][j] == 1)
					printf("□");
			}
		}

		a = getKey();
		switch (a)
		{
		case 'a':
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					gotoxy(x + j * 2, y + i);
					if (array[block_num][block_tnum][i][j] == 1)
						printf("  ");
				}
			}
			if (block_tnum == 0)
				block_tnum = 3;
			else
				block_tnum -= 1;
			result = check_block_can_spin(array, Garray, block_num, block_tnum, x, y);
			if (result == -1)
				block_tnum += 1;
			else if (result == 1)
			{
			}
			else
				x = result;

			break;

		case 's':
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					gotoxy(x + j * 2, y + i);
					if (array[block_num][block_tnum][i][j] == 1)
						printf("  ");
				}
			}
			if (block_tnum == 3)
				block_tnum = 0;
			else
				block_tnum += 1;
			result = check_block_can_spin(array, Garray, block_num, block_tnum, x, y);
			if (result == -1)
				block_tnum -= 1;
			else if (result == 1)
			{
			}
			else
				x = result;

			break;

		case 80:
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					gotoxy(x + j * 2, y + i);
					if (array[block_num][block_tnum][i][j] == 1)
						printf("  ");
				}
			}
			gotoxy(x, y);
			y++;
			ot = clock();
			break;

		case 75:
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					gotoxy(x + j * 2, y + i);
					if (array[block_num][block_tnum][i][j] == 1)
						printf("  ");
				}
			}
			x -= 2;
			if (check_block_can_goLR(array, Garray, block_num, block_tnum, x, y) == 1)
			{
				break;
			}
			else
			{
				x += 2;
				break;
			}

		case 77:
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					gotoxy(x + j * 2, y + i);
					if (array[block_num][block_tnum][i][j] == 1)
						printf("  ");
				}
			}
			x += 2;
			if (check_block_can_goLR(array, Garray, block_num, block_tnum, x, y) == 1)
			{
				break;
			}
			else
			{
				x -= 2;
				break;
			}

		case 32:
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					gotoxy(x + j * 2, y + i);
					if (array[block_num][block_tnum][i][j] == 1)
						printf("  ");
				}
			}

			y = down_straight(array, Garray, block_num, block_tnum, x, y);

			break;
		}

		if (nt - ot > 500 - time_diff)
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					gotoxy(x + j * 2, y + i);
					if (array[block_num][block_tnum][i][j] == 1)
						printf("  ");
				}
			}
			y++;
			ot = clock();
		}
	} while (check_block_can_down(array, Garray, block_num, block_tnum, x, y) == 1);

	*score += 4;
	y--;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			gotoxy(x + j * 2, y + i);
			if (array[block_num][block_tnum][i][j] == 1)
			{
				Garray[y + i][x + j * 2] = block_num;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9  +block_num);
				printf("▣");
			}
		}
	}
	return 1;
}

char getKey()
{
	if (kbhit())
		return _getch();
	else
		return NULL;
}

void make_map()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	gotoxy(0, 4);
	for (int i = 0; i < 20; i++)
	{
		gotoxy(0, 4 + i);
		printf("▩");
		gotoxy(22, 4 + i);
		printf("▩");
	}

	for (int i = 0; i < 23; i++)
	{
		if (i < 23 && i % 2 == 0)
		{
			gotoxy(i, 3);
			printf("--");
			gotoxy(i, 23);
			printf("▩");
		}
	}
}

void make_NEXT()
{

	for (int i = 1; i < 7; i++)		// NEXT 칸 만들기, 좌표(28, 6){칸 내부에서 (0,0)인 좌표}
	{
		gotoxy(26, 4 + i);
		printf("▩");
		if (i == 1 || i == 6)
		{
			for (int j = 28; j < 35; j += 2)
			{
				gotoxy(j, 4 + i);
				printf("▩");
			}
		}
		gotoxy(36, 4 + i);
		printf("▩");
	}

	gotoxy(27, 4);
	printf("N  E  X  T");

	gotoxy(0, 26);
}

void DEF_blocks(int array[7][4][4][4])
{
	int Barray[7][4][4][4] =
	{
		{
			{
				{0, 0, 0, 0},		//
				{0, 1, 1, 0},		//  □□
				{0, 1, 1, 0},		//	□□
				{0, 0, 0, 0}		//
			},
			{
				{0, 0, 0, 0},		//
				{0, 1, 1, 0},		//  □□
				{0, 1, 1, 0},		//	□□
				{0, 0, 0, 0}		//
			},
			{
				{0, 0, 0, 0},		//
				{0, 1, 1, 0},		//  □□
				{0, 1, 1, 0},		//	□□
				{0, 0, 0, 0}		//
			},
			{
				{0, 0, 0, 0},		//
				{0, 1, 1, 0},		//  □□
				{0, 1, 1, 0},		//	□□
				{0, 0, 0, 0}		//
			}
		},

		{
			{
				{0, 0, 0, 0},		//
				{0, 1, 0, 0},		//   □
				{1, 1, 1, 0},		//	□□□
				{0, 0, 0, 0}		//
			},
			{
				{0, 1, 0, 0},		//	□
				{0, 1, 1, 0},		//  □□
				{0, 1, 0, 0},		//	□
				{0, 0, 0, 0}		//
			},
			{
				{0, 0, 0, 0},		//
				{0, 1, 1, 1},		//  □□□
				{0, 0, 1, 0},		//	 □
				{0, 0, 0, 0}		//
			},
			{
				{0, 0, 0, 0},		//	 □
				{0, 0, 1, 0},		//  □□
				{0, 1, 1, 0},		//	 □
				{0, 0, 1, 0}		//
			}
		},

		{
			{
				{0, 1, 0, 0},		//	□
				{0, 1, 0, 0},		//	□
				{0, 1, 0, 0},		//	□
				{0, 1, 0, 0}		//	□
			},
			{
				{0, 0, 0, 0},		//	
				{0, 0, 0, 0},		//	
				{1, 1, 1, 1},		//	□□□□
				{0, 0, 0, 0}		//	
			},
			{
				{0, 1, 0, 0},		//	□
				{0, 1, 0, 0},		//	□
				{0, 1, 0, 0},		//	□
				{0, 1, 0, 0}		//	□
			},
			{
				{0, 0, 0, 0},		//	
				{0, 0, 0, 0},		//	
				{1, 1, 1, 1},		//	□□□□
				{0, 0, 0, 0}		//	
			}
		},
		
		{
			{
				{0, 0, 0, 0},		//
				{1, 1, 0, 0},		//	□□
				{0, 1, 1, 0},		//	 □□
				{0, 0, 0, 0}		//
			},
			{
				{0, 0, 1, 0},		//	 □
				{0, 1, 1, 0},		//	□□
				{0, 1, 0, 0},		//	□
				{0, 0, 0, 0}		//
			},
			{
				{0, 0, 0, 0},		//
				{1, 1, 0, 0},		//	□□
				{0, 1, 1, 0},		//	 □□
				{0, 0, 0, 0}		//
			},
			{
				{0, 0, 1, 0},		//	 □
				{0, 1, 1, 0},		//	□□
				{0, 1, 0, 0},		//	□
				{0, 0, 0, 0}		//
			},
		},

		{
			{
				{0, 0, 0, 0},		//
				{0, 1, 1, 0},		//	 □□
				{1, 1, 0, 0},		//	□□
				{0, 0, 0, 0}		//
			},
			{
				{0, 1, 0, 0},		//	□
				{0, 1, 1, 0},		//	□□
				{0, 0, 1, 0},		//	 □
				{0, 0, 0, 0}		//
			},
			{
				{0, 0, 0, 0},		//
				{0, 1, 1, 0},		//	 □□
				{1, 1, 0, 0},		//	□□
				{0, 0, 0, 0}		//
			},
			{
				{0, 1, 0, 0},		//	□
				{0, 1, 1, 0},		//	□□
				{0, 0, 1, 0},		//	 □
				{0, 0, 0, 0}		//
			}
		},

		{
			{
				{0, 0, 0, 0},		//	
				{0, 1, 1, 1},		//	□□□
				{0, 1, 0, 0},		//	□
				{0, 0, 0, 0}		//	
			},
			{
				{0, 0, 0, 0},		//	
				{0, 1, 1, 0},		//	□□
				{0, 0, 1, 0},		//	 □
				{0, 0, 1, 0}		//	 □
			},
			{
				{0, 0, 0, 0},		//	
				{0, 0, 1, 0},		//	  □
				{1, 1, 1, 0},		//	□□□
				{0, 0, 0, 0}		//	
			},
			{
				{0, 1, 0, 0},		//	
				{0, 1, 0, 0},		//	□
				{0, 1, 1, 0},		//	□
				{0, 0, 0, 0}		//	□□
			}
		},

		{
			{
				{0, 0, 0, 0},		//
				{0, 1, 1, 0},		//	□□
				{0, 1, 0, 0},		//	□
				{0, 1, 0, 0}		//	□
			},
			{
				{0, 0, 0, 0},		//
				{1, 1, 1, 0},		//	□□□
				{0, 0, 1, 0},		//	  □
				{0, 0, 0, 0}		//	
			},
			{
				{0, 0, 1, 0},		//	 □
				{0, 0, 1, 0},		//	 □
				{0, 1, 1, 0},		//	□□
				{0, 0, 0, 0}		// 
			},
			{
				{0, 0, 0, 0},		//
				{0, 1, 0, 0},		//	□
				{0, 1, 1, 1},		//	□□□
				{0, 0, 0, 0}		// 
			}
		}	
	};

	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				for (int x = 0; x < 4; x++)
				{
					array[i][j][k][x] = Barray[i][j][k][x];
				}
			}
		}
	}
}

int next_blocks(int array[7][4][4][4], int* next_block_num, int* next_block_tnum)
{
	srand(time(NULL));
	gotoxy(28, 6);			// NEXT 블록 생성

	int a, b;
	a = rand() % 7, b = rand() % 4;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			gotoxy(28 + 2 * j, 6 + i);
			printf("  ");
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			gotoxy(28 + 2 * j, 6 + i);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9 + a);
			if (array[a][b][i][j] == 1)
				printf("□");
		}
	}
	*next_block_num = a;
	*next_block_tnum = b;
}

int down_straight(int array[7][4][4][4], int Garray[24][22], int block_num, int block_tnum, int x, int y)
{
	while (check_block_can_down(array, Garray, block_num, block_tnum, x, y) == 1)
	{
		y++;
	}

	return y;
}

int check_block_can_down(int array[7][4][4][4], int Garray[24][22], int block_num, int block_tnum, int x, int y)
{
	int result = 0;

	for (int i = 3; i >= 0; i--)
	{
		for (int j = 3; j >= 0; j--)
		{
			if (array[block_num][block_tnum][i][j] == 1)
			{
				result = i;
				break;
			}
		}
		if (result == i)
			break;
	}

	if (y + result == 23)
	{
		return -1;
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (array[block_num][block_tnum][i][j] == 1 && Garray[y + i][x + j * 2] != -1)
				return -3;
		}
	}

	return 1;
}

int check_block_can_goLR(int array[7][4][4][4], int Garray[24][22], int block_num, int block_tnum, int x, int y)
{
	if (x == 0)
	{
		for (int i = 0; i < 4; i++)
		{
			if (array[block_num][block_tnum][i][0] == 1)
				return -1;
		}
	}

	else if (x == -2)
	{
		for (int i = 0; i < 4; i++)
		{
			if (array[block_num][block_tnum][i][1] == 1)
				return -2;
		}
	}

	else if (x == 16)
	{
		for (int i = 0; i < 4; i++)
		{
			if (array[block_num][block_tnum][i][3] == 1)
				return -3;
		}
	}

	else if (x == 18)
	{
		for (int i = 0; i < 4; i++)
		{
			if (array[block_num][block_tnum][i][2] == 1)
				return -4;
		}
	}

	else if (x == 20)
	{
		for (int i = 0; i < 4; i++)
		{
			if (array[block_num][block_tnum][i][1] == 1)
				return -5;
		}
	}
	
	else 
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (array[block_num][block_tnum][i][j] == 1 && Garray[y + i][x + j * 2] != -1)
					return -6;
			}
		}
	}
	

	return 1;
}

int check_block_can_spin(int array[7][4][4][4], int Garray[24][22], int block_num, int block_tnum, int x, int y)
{
	

	if (check_block_can_goLR(array, Garray, block_num, block_tnum, x, y) == -1)
	{
		x += 2;
		return x;
	}
	else if (check_block_can_goLR(array, Garray, block_num, block_tnum, x, y) == -2)
	{
		x += 2;
		return x;
	}
	else if (check_block_can_goLR(array, Garray, block_num, block_tnum, x, y) == -3)
	{
		x -= 2;
		return x;
	}
	else if (check_block_can_goLR(array, Garray, block_num, block_tnum, x, y) == -4)
	{
		x -= 4;
		return x;
	}
	else if (check_block_can_goLR(array, Garray, block_num, block_tnum, x, y) == -5)
	{
		x -= 4;
		return x;
	}
	else if (check_block_can_goLR(array, Garray, block_num, block_tnum, x, y) == -6)
	{
		return -1;
	}

	return 1;

}

int check_block_can_spawn(int array[7][4][4][4], int Garray[24][22], int block_num, int block_tnum, int x, int y)
{
	if (check_block_can_down(array, Garray, block_num, block_tnum, x, y) != 1)
	{
		return -1;
	}

	return 1;
}

int check_line(int Garray[24][22], int* score)
{
	for (int i = 23; i > 3; i--)
	{
		if (Garray[i][2] != -1 && Garray[i][4] != -1 && Garray[i][6] != -1 && Garray[i][8] != -1 && Garray[i][10] != -1 && Garray[i][12] != -1 && Garray[i][14] != -1 && Garray[i][16] != -1 && Garray[i][18] != -1 && Garray[i][20] != -1)
		{
			for (int a = i; a > 3; a--)
			{
				for (int b = 2; b < 22; b++)
				{
					gotoxy(b, a);
					printf("  ");
					Garray[a][b] = Garray[a-1][b];
					Garray[a - 1][b] = -1;
				}
			}
			i++;
			*score += 10;
		}
	}


	for (int i = 3; i < 23; i++)
	{
		for (int j = 2; j < 22; j += 2)
		{
			gotoxy(j, i);
			if (Garray[i][j] != -1)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9 + Garray[i][j]);
				printf("▣");
			}
		}
	}
}

int show_end(int* score)
{
	int i = 4, j = 2;
	time_t ot, nt;
	ot = clock();
	nt = clock();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

	while (i != 23)
	{
		nt = clock();
		if(nt - ot > 30 - i)
		{
			ot = clock();
			gotoxy(j, i);
			printf("★");
			j += 2;
			if (j > 20)
			{
				j = 2;
				i++;
			}
		}
	}

	i = 0;
	while (i != 24)
	{
		nt = clock();
		if (nt - ot > 100 - (i * 3))
		{
			ot = clock();
			gotoxy(0, i);
			printf("                                                  ");
			i++;
		}
	}

	gotoxy(0, 10);
	ot = clock();
	nt = clock();
	printf("========================\n\n");
	gotoxy(0, 12);
	printf("        Game Over       \n\n");
	gotoxy(0, 14);
	printf("     S C O R E: %d\n\n", *score);
	gotoxy(0, 16);
	printf("========================");
	do
	{
		nt = clock();
		gotoxy(0, 12);
		if (((nt - ot) / 700) % 2 == 0)
			printf("        Game Over       \n\n");
		else
			printf("                        \n\n");
	} while (nt - ot < 6000);
	system("cls");
	gotoxy(0, 0);
}

int show_score(int* score)
{
	gotoxy(26, 11);
	printf("S C O R E: %d", *score);
}


void gotoxy(int x, int y)
{
	COORD Cur = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void RemoveCursor(void)
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0; // bVisible 멤버 변경
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo); // 변경값 적용
}