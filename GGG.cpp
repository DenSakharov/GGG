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
eDir dir;
//класс карты
class Map
{
public:
	Map(int height = 20, int width = 40);
	~Map();
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
		if (strcmp(cells[y2][x2]->getType(), "Hero") == 0) return;
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
	friend class Enemy;
};
//начальный класс базовой ячейки


Cell::~Cell()
{
}
//класс Главного героя, нследуемый от класса Ячейки


Hero::~Hero()
{
}
//класс Врага, наследуемый от класса Ячейки


Enemy::~Enemy()
{
}
//класс выстрела 


Shoot::~Shoot()
{
}


Map::Map(int height, int width)
{
	this->height = height;
	this->width = width;
	cells = new Cell ** [height];
	for (int i = 0; i < height; i++) {
		cells[i] = new Cell*[width];
		for (int j = 0; j <width ; j++) {
			cells[i][j] = new Cell(i,j,this);
		}
	}
}

Map::~Map()
{
	for (int i = 0; i < height; i++) delete cells[i];
	delete cells;
}
void Map::addHero(int y, int x) {
	delete cells[y][x];
	cells[y][x] = new Hero(y,x,this);
	hero =(Hero*) cells[y][x];
}
void Map::addEnemy(int y, int x) {
	delete cells[y][x];
	cells[y][x] = new Enemy(y, x, this);
}

void Map::Run() {
	//цикл ввода и обработки этого ввода
	while (true)
	{
		//стирание экрана
		system("cls");
		vector <Enemy*> enemies;
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				if (strcmp(cells[y][x]->getType(), "Enemy") == 0)
				{
					enemies.push_back((Enemy*)cells[y][x]);
				}
			}
		}
		for (Enemy* enemy:enemies)
		{
			enemy->Move();
		}
		//ввод данных
		if (_kbhit()) {
			switch (_getch())
			{
			case 'w':
				dir = UP;
				break;
			case 's':
				dir = DOWN;
				break;
			case 'a':
				dir = LEFT;
				break;
			case 'd':
				dir = RIGHT;
				break;
			case 'D':
				dir = RIGHTS;
				break;
			case 'W':
				dir = UPS;
				break;
			case 'S':
				dir = DOWNS;
				break;
			case 'A':
				dir = LEFTS;
				break;
				//стрельба, нажатие клавишы f

			}
		}
		//движение главного героя и реализация стрельбы
		switch (dir)
		{
		case LEFT:
			Move(hero->x, hero->y, hero->x - 1, hero->y);
			break;
		case RIGHT:
			Move(hero->x, hero->y, hero->x + 1, hero->y);
			break;
		case UP:
			Move(hero->x, hero->y, hero->x, hero->y - 1);
			break;
		case DOWN:
			Move(hero->x, hero->y, hero->x, hero->y + 1);
			break;
		case LEFTS:
			for (int x = hero->x - 1; x >= 0; x--)
			{
				if (strcmp(cells[hero->y][x]->getType(), "Cell") != 0) {
					makeShoot(hero->y, x);
					break;
				}
				makeShoot(hero->y, x);
			}
			break;
		case RIGHTS:
			for (int x = hero->x + 1; x < width ; x++)
			{
				if (strcmp(cells[hero->y][x]->getType(), "Cell") != 0) {
					makeShoot(hero->y, x);
					break;
				}
				makeShoot(hero->y, x);
			}
			break;
		case UPS:
			for (int y = hero->y - 1; y >= 0; y--)
			{
				if (strcmp(cells[y][hero->x]->getType(), "Cell") != 0) {
					makeShoot(y, hero->x);
					break;
				}
				makeShoot(y, hero->x);
			}
			break;
		case DOWNS:
			for (int y = hero->y + 1; y <height; y++)
			{
				if (strcmp(cells[y][hero->x]->getType(), "Cell") != 0) {
					makeShoot(y, hero->x);
					break;
				}
				makeShoot(y, hero->x);
			}
			break;
		}
			dir = STOP;
			Draw();
		}
	}
	void Enemy::Move() {
		int sr = rand() % 20;
		if (sr < 4) direct = sr;
		switch (direct)
		{
		case 0:
			map->Move(x, y, x - 1, y);
			break;
		case 1:
			map->Move(x, y, x + 1, y);
			break;
		case 2:
			map->Move(x, y, x, y - 1);
			break;
		case 3:
			map->Move(x, y, x, y + 1);
			break;
		}
	}
	int main() {
		srand(unsigned int(time(NULL)));
		Map game(20, 40);
		game.addHero(10, 20);
		game.addEnemy(15, 30);
		game.addEnemy(5, 17);
		game.addEnemy(15, 17);
		game.Run();
	}
	
