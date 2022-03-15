#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <ctime>
#include <conio.h>
#include <dos.h> 
#include <string.h>
#include <vector>
#include "Cell.hpp"
using namespace std;



	//структура хранения нажатия клавиш управления
	enum eDir
	{
		STOP = 0, LEFT, RIGHT, UP, DOWN, LEFTS, RIGHTS, UPS, DOWNS,
	};
	class GameMap
	{
	public:
		GameMap(int height = 20, int width = 40);
		~GameMap();
		void addHero(int y, int x);
		void addEnemy(int y, int x);

		void Draw() {
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					if (strcmp(cells[y][x]->getType(), "Shoot") == 0) {
						if (((Shoot*)cells[y][x])->lifetime == 0)
						{
							delete cells[y][x];
							cells[y][x] = new Cell(y, x, this);

						}
						else (((Shoot*)cells[y][x])->lifetime)--;
					}
					cout << cells[y][x]->Image();
				}
				cout << endl;
			}
		}
		void Move(int x1, int y1, int x2, int y2) {
			if (x2 < 0 || y2 < 0 || x2 >= width || y2 >= height) return;
			if (strcmp(cells[y2][x2]->getType(), "Enemy") == 0) return;
			delete cells[y2][x2];
			cells[y2][x2] = cells[y1][x1];
			cells[y1][x1] = new Cell(y1, x1, this);
			cells[y2][x2]->x = x2;
			cells[y2][x2]->y = y2;
		}
		void Run();
		void makeShoot(int y, int x) {
			delete cells[y][x];
			cells[y][x] = new Shoot(y, x, this);
		}
	private:
		Cell*** cells;
		int height;
		int width;
		Hero* hero = NULL;
		/*vector <Enemy> enemies;*/
		eDir dir;
	};
