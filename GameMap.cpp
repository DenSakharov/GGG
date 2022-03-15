#include "GameMap.hpp"
GameMap::GameMap(int height, int width)
{
	this->height = height;
	this->width = width;
	cells = new Cell * *[height];
	for (int i = 0; i < height; i++) {
		cells[i] = new Cell * [width];
		for (int j = 0; j < width; j++) {
			cells[i][j] = new Cell(i, j, this);
		}
	}
}

GameMap::~GameMap()
{
	for (int i = 0; i < height; i++) delete cells[i];
	delete cells;
}
void GameMap::addHero(int y, int x) {
	delete cells[y][x];
	cells[y][x] = new Hero(y, x, this);
	hero = (Hero*)cells[y][x];
}
void GameMap::addEnemy(int y, int x) {
	delete cells[y][x];
	cells[y][x] = new Enemy(y, x, this);
	/*hero = (Enemy*)cells[y][x];*/
}

void GameMap::Run() {
	//цикл ввода и обработки этого ввода
	while (true)
	{
		/*for (Enemy enemy : enemies)
		{
			enemy.Move();
		}*/
		//стирание экрана
		system("cls");
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
		}
		dir = STOP;
		Draw();
	}
}