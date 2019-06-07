#pragma once


class Frame {

protected:
	int base;
	int limite;
	int id;
public:
	Frame(int b, int l, int i);
	int getBase();
	int getLimite();
	int getID();
	void setBase(int x);
	void setLimite(int x);
	void setID(int x);
};