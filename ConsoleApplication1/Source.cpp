#include <iostream>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include <curses.h>
#include <windows.h>
#include <fstream>

using namespace std;
int start_menu();
void road(int*, int*);
void scroll(int*);
void delay_(float);
void pause();
int p_car(int**, int, int*, int*);
void pine(int, int);
void car(int**, int, int, int k = 0, int c = 5);
void explosion(int, int);
int gameover();
void hud(int*, unsigned long, int);

int main()
{
	initscr();
	keypad(stdscr, TRUE);
	curs_set(FALSE);
	start_color();
	init_pair(2, COLOR_WHITE, COLOR_WHITE);
	init_pair(3, COLOR_RED, COLOR_RED);
	init_pair(4, COLOR_GREEN, COLOR_GREEN);
	init_pair(5, COLOR_CYAN, COLOR_CYAN);
	init_pair(6, COLOR_MAGENTA, COLOR_MAGENTA);
	init_pair(7, COLOR_WHITE, COLOR_BLACK);
	init_pair(8, COLOR_YELLOW, COLOR_BLACK);
	init_pair(9, COLOR_RED, COLOR_BLACK);
	init_pair(10, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(11, COLOR_BLACK, COLOR_BLACK);

	int sidecol[24] = { 3,2,2,2,3,3,3,2,2,2,3,3,3,2,2,2,3,3,3,2,2,2,3,3 };    // разметка бордюра (красный/белый)
	int midcol[24] = { 2,2,2,7,7,2,2,2,2,7,7,2,2,2,2,7,7,2,2,2,2,7,7,2 };     // разметка на дороге (белые полосы)
	int delay = 80, speed = 1;                                                  // скорость игры (чем ниже тем больше)
	unsigned long ctr = 0;                                                    // счетчик
	srand(time(NULL));
	int ypr1 = 0, ypr2 = 0, xpr = 0, xp1, xp2, sp = 0;                                   // переменные для отображения сосен
	int ycr1 = 0, ycr2 = 0, xcr = 0, xc1, xc2, sc = 0, y2cr1, y2cr2, col1, col2, scd = 8;       // переменные для отображения машин
	int pcy = 0;                                                              // переменная для перемещения машины игрока
	int**collmas = new int*[4]; int i, j;                                    // выделение памяти под массив для
	for (i = 0; i<4; i++) collmas[i] = new int[11];                              // просчета столкновений
	int colx, coly;
	if (start_menu() == 1) { clear(); refresh(); return 0; }

	int record;                                                             // чтение рекорда из файла
	ifstream fin("record.txt");
	fin >> record;
	fin.close();

	do {                                                                    // запуск основного цикла программы
		clear();
		road(sidecol, midcol);                                              // отрисовка дороги

		if (xpr == 0) {                                                        // отрисовка сосен
			xpr = 35 + rand() % 10;                  // установка расстояния между появлением сосен
			xp1 = 0;                             // отрисовка сосны с края экрана
			ypr1 = rand() % 2;                     // случайное появление 1й сосны слева или справа
			sp = rand() % 2;                       // случайное появление второй сосны
			xp2 = xp1 - rand() % 10 - 6;               // установка расстояния между 1 и 2 соснами
			ypr2 = rand() % 2;
		}                    // случайное появление 2й сосны слева или справа
		pine(xp1, ypr1);                        // функция отрисовки сосны (для 1й сосны)
		xpr--;                                 // счетчик расстояния между соснами уменьшается
		xp1++;                                 // положение сосны на экране меняется (имитация движения)
		if (sp) { pine(xp2, ypr2); xp2++; }        // функция отрисовки сосны (для 2й сосны) и изменение положения

		hud(&record, ctr, speed);

		for (i = 0; i<4; i++) for (j = 0; j<11; j++) collmas[i][j] = 0;                 // обнуление массива столкновений

		if (xcr == 0) {
			xcr = 45 + rand() % 10; xc1 = 0; ycr1 = rand() % 2;                 // отрисовка машин
			sc = rand() % 2; xc2 = xc1 - rand() % 5 - scd; ycr2 = rand() % 2;                   // тут все аналогично отрисовке сосен
			if (rand() % 2) col1 = 6; else col1 = 5; y2cr1 = rand() % 3 - 1;
			if (sc) { if (rand() % 2) col2 = 6; else col2 = 5; y2cr2 = rand() % 3 - 1; }
		}       // но добавляются переменные для случайного
		car(collmas, xc1, ycr1, y2cr1, col1); xcr--; xc1++;                     // выбора цвета и положения машины
		if (sc) { car(collmas, xc2, ycr2, y2cr2, col2); xc2++; }                   // со смещением от центра полосы

																				   // отрисовка машины игрока
		if (pcy>-7) if (GetKeyState(VK_LEFT)<0) pcy--;                        // движение влево
		if (pcy<1) if (GetKeyState(VK_RIGHT)<0) pcy++;                        // движение вправо

		if (p_car(collmas, pcy, &colx, &coly)) {
			explosion(colx, coly);           // функция отрисовки машины вместе с проверкой на столкновения
			if (gameover() == 1) break; else {
				clear(); ctr = 0; speed = 1; delay = 80;
				for (i = 0; i<4; i++) for (j = 0; j<11; j++) collmas[i][j] = 0; pcy = 0;
				ycr1 = 0, ycr2 = 0, xcr = 0, xc1, xc2, sc = 0, y2cr1, y2cr2, col1, col2, scd = 8;
				continue;
			}
		}

		if (ctr == 200) { delay = 60, speed = 2; scd = 9; }                            // изменение скорости в зависимости от растояния
		if (ctr == 500) { delay = 45, speed = 3; scd = 10; }
		if (ctr == 1000) { delay = 35, speed = 4; scd = 12; }
		if (ctr == 2000) { delay = 25, speed = 5; scd = 14; }

		refresh();                                                          // обновить экран
		if (GetKeyState(VK_SPACE) == 1) pause();                               // пауза
		delay_(delay);                                                      // задержка/скорость
		ctr++;                                                              // прирост счетчика
	} while (1);                                                            // основной цикл вечный (пока не разобъешься)

	for (i = 0; i<4; i++) delete[]collmas[i];                           // очистка памяти, выделенной под массив столкновений
	delete[]collmas;

	ofstream fout("record.txt", ios_base::out | ios_base::trunc);    // запись рекорда в файл
	fout << record;
	fin.close();

	endwin();                                                       // выход из расширенного режима
}

int start_menu()
{
	int i, j, x, y, k = 0; unsigned char a;
	char mas[13][30] = {
		{ 1,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,1,1,0 },
		{ 1,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1 },
		{ 1,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0 },
		{ 1,1,1,1,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0 },
		{ 1,0,1,0,0,0,0,1,1,1,1,0,0,1,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,0 },
		{ 1,0,0,1,0,0,1,1,0,0,1,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1 },
		{ 1,0,0,0,1,0,1,0,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1 },
		{ 1,0,0,0,1,0,1,0,0,0,0,1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,1,1,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1 },
		{ 1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1 },
		{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
		{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 } };
	attron(COLOR_PAIR(3) | A_BOLD);
	while (k<3) {
		clear(); x = 3;
		for (j = 0; j<8; j++) {
			y = 26; for (i = 0; i<30; i++) {
				move(x, y);
				if (mas[j][i])
				{
					if (k == 0) addch(ACS_BOARD);
					if (k == 1) addch(ACS_CKBOARD);
					if (k == 2) addch(ACS_BLOCK);
				}
				y++;
			} x++;
		} refresh(); delay_(1000); k++;
	}

	attron(COLOR_PAIR(2) | A_BOLD); x = 12;
	for (j = 9; j<13; j++) { y = 26; for (i = 0; i<30; i++) { move(x, y); if (mas[j][i])addch(ACS_BLOCK); y++; } x++; }
	j = 18;
	attron(COLOR_PAIR(8) | A_BOLD); x = 17;
	for (k = 0; k<2; k++, x += 3) {
		move(x, 36); addch(ACS_ULCORNER); for (i = 0; i<8; i++) addch(ACS_HLINE); addch(ACS_URCORNER);
		move(x + 1, 36); addch(ACS_VLINE); move(x + 1, 45); addch(ACS_VLINE);
		move(x + 2, 36); addch(ACS_LLCORNER); for (i = 0; i<8; i++) addch(ACS_HLINE); addch(ACS_LRCORNER);
	}
	attron(COLOR_PAIR(9) | A_BOLD); move(18, 39); addstr("START"); move(21, 39); addstr("EXIT");
	attron(COLOR_PAIR(7) | A_BOLD); move(j, 37); addch(ACS_DIAMOND);
	refresh();
	do {
		a = getch();
		if (a == 2 && j == 18) { move(j, 37); addch(' '); j = 21; move(j, 37); addch(ACS_DIAMOND); }
		else if (a == 3 && j == 21) { move(j, 37); addch(' '); j = 18; move(j, 37); addch(ACS_DIAMOND); }
	} while (a != '\n');
	if (j == 21) return 1;
}

void road(int*mas1, int*mas2)
{
	int x;
	for (x = 0; x<22; x++)
	{
		move(x, 25);
		attron(COLOR_PAIR(4) | A_BOLD); for (int i = 0; i<9; i++) addch(ACS_BLOCK);
		attron(COLOR_PAIR(mas1[x]) | A_BOLD); addch(ACS_BLOCK);
		move(x, 40);
		if (mas2[x] != 7) { attron(COLOR_PAIR(2) | A_BOLD); addch(ACS_BLOCK); }
		move(x, 46);
		attron(COLOR_PAIR(mas1[x]) | A_BOLD); addch(ACS_BLOCK);
		attron(COLOR_PAIR(4) | A_BOLD); for (int i = 0; i<9; i++) addch(ACS_BLOCK);
	}
	scroll(mas1); scroll(mas2);
}

void scroll(int*mas)
{
	int TMP = mas[23];
	for (int i = 23; i >= 0; i--) mas[i] = mas[i - 1];
	mas[0] = TMP;
}

int p_car(int**mas, int y, int *x1, int *y1)
{
	int x = 17, i, k = 0;
	attron(COLOR_PAIR(3) | A_BOLD);
	move(x, 43 + y); addch(ACS_BLOCK); if (x>17 && x<22) if (mas[x - 18][8 + y] == 1) { k = 1; *x1 = x; *y1 = 43 + y; }
	else mas[x - 18][8 + y] = 1;
	move(x + 1, 42 + y); for (i = 0; i<3; i++) { addch(ACS_BLOCK); if (x + 1>17 && x + 1<22) if (mas[x - 17][7 + y + i] == 1) { k = 1; *x1 = x + 1; *y1 = 42 + y; } else mas[x - 17][7 + y + i] = 1; }
	move(x + 2, 43 + y); addch(ACS_BLOCK); if (x + 2>17 && x + 2<22) if (mas[x - 16][8 + y] == 1) { k = 1; *x1 = x + 2; *y1 = 43 + y; }
	else mas[x - 16][8 + y] = 1;
	move(x + 3, 42 + y); for (i = 0; i<3; i++) { addch(ACS_BLOCK); if (x + 3>17 && x + 3<22) if (mas[x - 15][7 + y + i] == 1) { k = 1; *x1 = x + 3; *y1 = 42 + y; } else mas[x - 15][7 + y + i] = 1; }
	return k;
}

void car(int**mas, int x, int y, int k, int c)
{
	int i;
	if (y) y = 6; y += k;
	attron(COLOR_PAIR(c) | A_BOLD);
	if (x >= 0 && x<22) { move(x, 36 + y); for (i = 0; i<3; i++) { addch(ACS_BLOCK); if (x>17 && x<22) mas[x - 18][1 + y + i] = 1; } }
	if (x - 1 >= 0 && x - 1<22) { move(x - 1, 37 + y); addch(ACS_BLOCK); if (x - 1>17 && x - 1<22) mas[x - 19][2 + y] = 1; }
	if (x - 2 >= 0 && x - 2<22) { move(x - 2, 36 + y); for (i = 0; i<3; i++) { addch(ACS_BLOCK); if (x - 2>17 && x - 2<22) mas[x - 20][1 + y + i] = 1; } }
	if (x - 3 >= 0 && x - 3<22) { move(x - 3, 37 + y); addch(ACS_BLOCK); if (x - 3>17 && x - 3<22) mas[x - 21][2 + y] = 1; }
}

void pine(int x, int y)
{
	int i;
	if (y) y = 22;
	attron(COLOR_BLACK | A_BOLD);
	if (x >= 0 && x<22) { move(x, 27 + y); for (i = 0; i<5; i++) addch(ACS_BOARD); }
	if (x - 1 >= 0 && x - 1<22) { move(x - 1, 28 + y); for (i = 0; i<3; i++) addch(ACS_BOARD); }
	if (x - 2 >= 0 && x - 2<22) { move(x - 2, 29 + y); addch(ACS_BOARD); }
}

void explosion(int x, int y)
{
	int i, c1, c2, c3;
	for (i = 0; i<3; i++) {
		if (i == 0) { c1 = 2; c2 = 10; c3 = 8; }
		if (i == 1) { c1 = 10; c2 = 8; c3 = 7; }
		if (i == 2) { c1 = 8; c2 = 7; c3 = 11; }
		attron(COLOR_PAIR(11) | A_BOLD);
		move(x - 1, y - 1); addstr("   ");
		move(x, y - 1); addstr("   ");
		move(x + 1, y - 1); addstr("   ");
		attron(COLOR_PAIR(c1) | A_BOLD);
		move(x, y); addch(ACS_BLOCK);
		attron(COLOR_PAIR(c2) | A_BOLD);
		move(x - 1, y); addch(ACS_BOARD);
		move(x + 1, y); addch(ACS_BOARD);
		move(x, y + 1); addch(ACS_BOARD);
		move(x, y - 1); addch(ACS_BOARD);
		attron(COLOR_PAIR(c3));
		move(x - 1, y - 1); addch(ACS_CKBOARD);
		move(x - 1, y + 1); addch(ACS_CKBOARD);
		move(x + 1, y - 1); addch(ACS_CKBOARD);
		move(x + 1, y + 1); addch(ACS_CKBOARD);
		refresh(); delay_(500);
	}
}

int gameover()
{
	int i, j, x = 6, y = 25, k; unsigned char a;
	for (i = 0; i<8; i++, x++) { move(x, y); clrtoeol(); }
	char mas[6][49] = {
		{ 0,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,1,1,1,0,0,0,0,0,1,1,1,0,0,1,0,0,0,1,0,1,1,1,1,0,0,1,1,1,1,0 },
		{ 1,0,0,0,1,0,0,0,1,0,0,0,1,1,0,1,1,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1 },
		{ 1,0,0,0,0,0,0,1,0,1,0,0,1,0,1,0,1,0,1,1,1,0,0,0,0,0,1,0,0,0,1,0,0,1,0,1,0,0,1,1,1,0,0,0,1,0,0,0,1 },
		{ 1,0,1,1,1,0,0,1,1,1,0,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,1,1,1,0 },
		{ 1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1 },
		{ 0,1,1,1,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,0,1,0,0,0,1,1,1,1,1,0,1,0,0,0,1 } };
	attron(COLOR_PAIR(3) | A_BOLD); x = 7;
	for (j = 0; j<6; j++, x++) { y = 17; for (i = 0; i<49; i++, y++) { move(x, y); if (mas[j][i]) addch(ACS_BLOCK); } }
	refresh();
	delay_(1500); x = 7;
	for (i = 0; i<8; i++, x++) { move(x, 17); clrtoeol(); refresh(); }

	attron(COLOR_PAIR(8) | A_BOLD); x = 7;
	for (k = 0; k<2; k++, x += 3) {
		move(x, 35); addch(ACS_ULCORNER); for (i = 0; i<9; i++) addch(ACS_HLINE); addch(ACS_URCORNER);
		move(x + 1, 35); addch(ACS_VLINE); move(x + 1, 45); addch(ACS_VLINE);
		move(x + 2, 35); addch(ACS_LLCORNER); for (i = 0; i<9; i++) addch(ACS_HLINE); addch(ACS_LRCORNER);
	}
	attron(COLOR_PAIR(7) | A_BOLD); move(8, 36); addch(ACS_DIAMOND);
	attron(COLOR_PAIR(9) | A_BOLD); move(8, 38); addstr("RESTART"); move(11, 38); addstr("EXIT");
	refresh(); j = 8; attron(COLOR_PAIR(7) | A_BOLD);
	do {
		a = getch();
		if (a == 2 && j == 8) { move(j, 36); addch(' '); j = 11; move(j, 36); addch(ACS_DIAMOND); refresh(); }
		else if (a == 3 && j == 11) { move(j, 36); addch(' '); j = 8; move(j, 36); addch(ACS_DIAMOND); refresh(); }
	} while (a != '\n');
	if (j == 11) return 1;
}

void hud(int*record, unsigned long ctr, int speed)
{
	if (ctr / 2>*record) *record = ctr / 2;
	attron(COLOR_PAIR(8) | A_BOLD);
	move(2, 60); addstr("Record:"); move(2, 68); printw("%d", *record);
	move(3, 60); addstr("Score:"); move(3, 68); printw("%d", ctr / 2);
	move(4, 60); addstr("Speed:"); move(4, 68); printw("%d", speed);

}

void delay_(float secs)
{
	srand(time(0));
	clock_t delay = secs;
	clock_t start = clock();
	while (clock() - start < delay);
}

void pause()
{
	attron(COLOR_PAIR(8) | A_BOLD); int i = 0;
	while (1) {
		move(2, 16); if (i % 2) addstr("     "); else addstr("PAUSE");
		delay_(500); i++; refresh(); if (GetKeyState(VK_SPACE) == 0) break;
	}
}
