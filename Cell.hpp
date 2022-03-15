#pragma once
#ifndef __Cell_h__
#define __Cell_h__

using namespace std;

class Map;
class Cell
{
public:
	virtual char Image() {
		return ' ';
	}
	Cell(int y, int x, Map* map) {
		this->x = x;
		this->y = y;
		this->map = map;
	}
	~Cell();
	virtual const char* getType() { return "Cell"; }
	//Cell(Cell&);
protected:
	int x, y;
	Map* map;
	friend class Map;

};
class Hero :public Cell
{
public:

	Hero(int y, int x, Map* map) : Cell(y, x, map) {
	}
	~Hero();
	virtual char Image() {
		return '0';
	}
	void Move(int y, int x) {

	}
	virtual const char* getType() { return "Hero"; }
private:
};
class Enemy :public Cell
{
public:
	Enemy(int y, int x, Map* map) : Cell(y, x, map) {
		direct = rand() % 4;
	}
	~Enemy();
	virtual char Image() {
		return 'E';
	}
	virtual const char* getType() { return "Enemy"; }
	void Move();
private:
	int direct;
};
class Shoot : public Cell
{
public:
	Shoot(int y, int x, Map* map) : Cell(y, x, map) {
	}
	~Shoot();
	virtual char Image() {
		return '*';
	}
	virtual const char* getType() { return "Shoot"; }

private:
	int lifetime = 1;
	friend class Map;
};
#endif