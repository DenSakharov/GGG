#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
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
//#pragma comment(lib, "ws2_32.lib")
//#include <winsock2.h>
//#include <iostream>
//
//#pragma warning(disable: 4996)

using namespace std;
//структура хранения нажатия клавиш управления
enum eDir
{
	STOP = 0, LEFT, RIGHT, UP, DOWN, LEFTS, RIGHTS, UPS, DOWNS
};
eDir dir;

SOCKET Connection;

//класс карты
class Map
{
public:
	Map(int height = 20, int width = 40);
	~Map();
	void addHero(int y, int x);
	/*void addHeroTwo(int y, int x);*/
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
				/*if (strcmp(cells[y][x]->getType(), "ShootTwo") == 0) {
					if (((ShootTwo*)cells[y][x])->lifetimeT == 0)
					{
						delete cells[y][x];
						cells[y][x] = new Cell(y, x, this);

					}
					else (((ShootTwo*)cells[y][x])->lifetimeT)--;
				}*/
				cout << cells[y][x]->Image();
			}
			cout << endl;
		}
	}
	void Move(int x1, int y1, int x2, int y2) {
		if (x2 < 0 || y2 < 0 || x2 >= width || y2 >= height) return;
		if (strcmp(cells[y2][x2]->getType(), "Enemy") == 0) return;
		if (strcmp(cells[y2][x2]->getType(), "Hero") == 0) return;
		/*if (strcmp(cells[y2][x2]->getType(), "HeroTwo") == 0) return;*/
		delete cells[y2][x2];
		cells[y2][x2] = cells[y1][x1];
		cells[y1][x1] = new Cell(y1, x1, this);
		cells[y2][x2]->x = x2;
		cells[y2][x2]->y = y2;
	}
	/*void MoveT(int x1, int y1, int x2, int y2) {
		if (x2 < 0 || y2 < 0 || x2 >= width || y2 >= height) return;
		if (strcmp(cells[y2][x2]->getType(), "Enemy") == 0) return;
		if (strcmp(cells[y2][x2]->getType(), "Hero") == 0) return;
		if (strcmp(cells[y2][x2]->getType(), "HeroTwo") == 0) return;
		delete cells[y2][x2];
		cells[y2][x2] = cells[y1][x1];
		cells[y1][x1] = new Cell(y1, x1, this);
		cells[y2][x2]->x = x2;
		cells[y2][x2]->y = y2;
	}*/
	void Run();
	void makeShoot(int y, int x) {
		delete cells[y][x];
		cells[y][x] = new Shoot(y, x, this);

	}
	/*void makeShootT(int y, int x) {
		delete cells[y][x];
		cells[y][x] = new ShootTwo(y, x, this);

	}*/
private:
	Cell*** cells;
	int height;
	int width;
	Hero* hero = NULL;
	Enemy* enemy = NULL;
	//HeroTwo* heroT = NULL;
	eDir dirEnemyAction= STOP;
	friend class Enemy;
	friend void ClientHandler();
};
//начальный класс базовой ячейки


Cell::~Cell()
{
}
//класс Главного героя, нследуемый от класса Ячейки


Hero::~Hero()
{
}
//HeroTwo::~HeroTwo()
//{
//}
//класс Врага, наследуемый от класса Ячейки


Enemy::~Enemy()
{
}
//класс выстрела 


Shoot::~Shoot()
{
}
//ShootTwo::~ShootTwo()
//{
//}


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
//void Map::addHeroTwo(int y, int x) {
//	delete cells[y][x];
//	cells[y][x] = new HeroTwo(y, x, this);
//	heroT = (HeroTwo*)cells[y][x];
//}
void Map::addEnemy(int y, int x) {
	delete cells[y][x];
	cells[y][x] = new Enemy(y, x, this);
	enemy = (Enemy*)cells[y][x];
}

void Map::Run() {
	//цикл ввода и обработки этого ввода
	while (true)
	{
		//стирание экрана
		system("cls");
		/*vector <Enemy*> enemies;
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
		}*/
		//ввод данных
		if (_kbhit()) {
			char ch = _getch();
			send(Connection, &ch, 1, NULL);
			switch (ch)
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
			/*case 'i':
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
				break;*/
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

			/*case 'L':
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
				break;*/

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
		/*case LEFTT:
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
			break;*/
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
		/*case LEFTST:
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
			break;*/
		}
		dir = STOP;  





		switch (dirEnemyAction)
		{
		case LEFT:
			Move(enemy->x, enemy->y, enemy->x - 1, enemy->y);
			break;
		case RIGHT:
			Move(enemy->x, enemy->y, enemy->x + 1, enemy->y);
			break;
		case UP:
			Move(enemy->x, enemy->y, enemy->x, enemy->y - 1);
			break;
		case DOWN:
			Move(enemy->x, enemy->y, enemy->x, enemy->y + 1);
			break;
		
		case LEFTS:
			for (int x = enemy->x - 1; x >= 0; x--)
			{
				if (strcmp(cells[enemy->y][x]->getType(), "Cell") != 0) {
					makeShoot(enemy->y, x);
					break;
				}
				makeShoot(enemy->y, x);
			}
			break;
		case RIGHTS:
			for (int x = enemy->x + 1; x < width; x++)
			{
				if (strcmp(cells[enemy->y][x]->getType(), "Cell") != 0) {
					makeShoot(enemy->y, x);
					break;
				}
				makeShoot(enemy->y, x);
			}
			break;
		case UPS:
			for (int y = enemy->y - 1; y >= 0; y--)
			{
				if (strcmp(cells[y][enemy->x]->getType(), "Cell") != 0) {
					makeShoot(y, enemy->x);
					break;
				}
				makeShoot(y, enemy->x);
			}
			break;
		case DOWNS:
			for (int y = enemy->y + 1; y < height; y++)
			{
				if (strcmp(cells[y][enemy->x]->getType(), "Cell") != 0) {
					makeShoot(y, enemy->x);
					break;
				}
				makeShoot(y, enemy->x);
			}
			break;
			
		}
		dirEnemyAction = STOP;
		Draw();

	}
}
	
	/*void Enemy::Move() {
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
	}*/


#pragma warning(disable: 4996)

	Map game(20, 40);

	void ClientHandler() {
		char msg[256];
		while (true) {
			recv(Connection, msg, sizeof(msg), NULL);
			
			if (msg[0]=='1')
			{
				//Sleep(100);
				recv(Connection, msg, sizeof(msg), NULL);
				int x = atoi(msg);
				//Sleep(100);
				recv(Connection, msg, sizeof(msg), NULL);
				int y = atoi(msg);
				game.addEnemy(y,x);
				continue;
			}

			switch (msg[0])
			{
				//хотьба врага
			case 'w':
				game.dirEnemyAction = UP;
				break;
			case 's':
				game.dirEnemyAction = DOWN;
				break;
			case 'a':
				game.dirEnemyAction = LEFT;
				break;
			case 'd':
				game.dirEnemyAction = RIGHT;
				break;
				//стрельба врага
			case 'D':
				game.dirEnemyAction = RIGHTS;
				break;
			case 'W':
				game.dirEnemyAction = UPS;
				break;
			case 'S':
				game.dirEnemyAction = DOWNS;
				break;
			case 'A':
				game.dirEnemyAction = LEFTS;
				break;
			}
		}
	}

	int main() {
		//WSAStartup
		WSAData wsaData;
		WORD DLLVersion = MAKEWORD(2, 1);
		if (WSAStartup(DLLVersion, &wsaData) != 0) {
			std::cout << "Error" << std::endl;
			exit(1);
		}

		SOCKADDR_IN addr;
		int sizeofaddr = sizeof(addr);
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		addr.sin_port = htons(1111);
		addr.sin_family = AF_INET;

		Connection = socket(AF_INET, SOCK_STREAM, NULL);
		if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
			std::cout << "Error: failed connect to server.\n";
			return 1;
		}
		std::cout << "Connected!\n";

		

		char msg1[256];
	
		recv(Connection, msg1, sizeof(msg1), NULL);
		int x1 = atoi(msg1);
		recv(Connection, msg1, sizeof(msg1), NULL);
		int y1 = atoi(msg1);
		//recv(Connection, msg1, sizeof(msg1), NULL);
		//int x2 = atoi(msg1);
		//recv(Connection, msg1, sizeof(msg1), NULL);
		//int y2 = atoi(msg1);
		Sleep(100);
		game.addHero(y1, x1);
		/*game.addEnemy(y2, x2);*/

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(0), NULL, NULL);

		game.Run();
	}
	
