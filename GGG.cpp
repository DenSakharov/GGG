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
	STOP = 0, LEFT, RIGHT, UP, DOWN, LEFTS, RIGHTS, UPS, DOWNS, LEFTT, RIGHTT, UPT, DOWNT, LEFTST, RIGHTST, UPST, DOWNST
};
eDir dir;
//класс карты
class Map
{
public:
	Map(int height = 20, int width = 40);
	~Map();
	void addHero(int y, int x);
	void addHeroTwo(int y, int x);
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
				if (strcmp(cells[y][x]->getType(), "ShootTwo") == 0) {
					if (((ShootTwo*)cells[y][x])->lifetimeT == 0)
					{
						delete cells[y][x];
						cells[y][x] = new Cell(y, x, this);

					}
					else (((ShootTwo*)cells[y][x])->lifetimeT)--;
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
		if (strcmp(cells[y2][x2]->getType(), "HeroTwo") == 0) return;
		delete cells[y2][x2];
		cells[y2][x2] = cells[y1][x1];
		cells[y1][x1] = new Cell(y1, x1, this);
		cells[y2][x2]->x = x2;
		cells[y2][x2]->y = y2;
	}
	void MoveT(int x1, int y1, int x2, int y2) {
		if (x2 < 0 || y2 < 0 || x2 >= width || y2 >= height) return;
		if (strcmp(cells[y2][x2]->getType(), "Enemy") == 0) return;
		if (strcmp(cells[y2][x2]->getType(), "Hero") == 0) return;
		if (strcmp(cells[y2][x2]->getType(), "HeroTwo") == 0) return;
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
	void makeShootT(int y, int x) {
		delete cells[y][x];
		cells[y][x] = new ShootTwo(y, x, this);

	}
private:
	Cell*** cells;
	int height;
	int width;
	Hero* hero = NULL;
	HeroTwo* heroT = NULL;
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
HeroTwo::~HeroTwo()
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
ShootTwo::~ShootTwo()
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
void Map::addHeroTwo(int y, int x) {
	delete cells[y][x];
	cells[y][x] = new HeroTwo(y, x, this);
	heroT = (HeroTwo*)cells[y][x];
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
		for (Enemy* enemy : enemies)
		{
			enemy->Move();
		}
		//ввод данных
		if (_kbhit()) {
			switch (_getch())
			{
				//хотьба первым игроком
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
				//хотьба правым игроком
			case 'i':
				dir = UPT;
				break;
			case 'k':
				dir = DOWNT;
				break;
			case 'j':
				dir = LEFTT;
				break;
			case 'l':
				dir = RIGHTT;
				break;
				//стрельба первого игрока
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

			case 'L':
				dir = RIGHTST;
				break;
			case 'I':
				dir = UPST;
				break;
			case 'K':
				dir = DOWNST;
				break;
			case 'J':
				dir = LEFTST;
				break;

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
		case LEFTT:
			MoveT(heroT->x, heroT->y, heroT->x - 1, heroT->y);
			break;
		case RIGHTT:
			MoveT(heroT->x, heroT->y, heroT->x + 1, heroT->y);
			break;
		case UPT:
			MoveT(heroT->x, heroT->y, heroT->x, heroT->y - 1);
			break;
		case DOWNT:
			MoveT(heroT->x, heroT->y, heroT->x, heroT->y + 1);
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
			for (int x = hero->x + 1; x < width; x++)
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
			for (int y = hero->y + 1; y < height; y++)
			{
				if (strcmp(cells[y][hero->x]->getType(), "Cell") != 0) {
					makeShoot(y, hero->x);
					break;
				}
				makeShoot(y, hero->x);
			}
			break;
		case LEFTST:
			for (int x = heroT->x - 1; x >= 0; x--)
			{
				if (strcmp(cells[heroT->y][x]->getType(), "Cell") != 0) {
					makeShootT(heroT->y, x);
					break;
				}
				makeShootT(heroT->y, x);
			}
			break;
		case RIGHTST:
			for (int x = heroT->x + 1; x < width; x++)
			{
				if (strcmp(cells[heroT->y][x]->getType(), "Cell") != 0) {
					makeShootT(heroT->y, x);
					break;
				}
				makeShootT(heroT->y, x);
			}
			break;
		case UPST:
			for (int y = heroT->y - 1; y >= 0; y--)
			{
				if (strcmp(cells[y][heroT->x]->getType(), "Cell") != 0) {
					makeShootT(y, heroT->x);
					break;
				}
				makeShootT(y, heroT->x);
			}
			break;
		case DOWNST:
			for (int y = heroT->y + 1; y < height; y++)
			{
				if (strcmp(cells[y][heroT->x]->getType(), "Cell") != 0) {
					makeShootT(y, heroT->x);
					break;
				}
				makeShootT(y, heroT->x);
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
		game.addHeroTwo(2, 20);
		game.addEnemy(15, 30);
		game.addEnemy(5, 17);
		game.addEnemy(15, 17);
		game.Run();
	}
	
