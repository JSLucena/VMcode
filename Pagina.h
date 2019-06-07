#pragma once




class Pagina {
protected:	
		int base;
		int limite;
		int id;
		int idFrame;
public:
	Pagina(int b, int l, int i, int f);
	int getBase();
	int getLimite();
	int getID();
	int getFrameID();
	void setBase(int x);
	void setLimite(int x);
	void setID(int x);
	void setFrameID(int x);
};