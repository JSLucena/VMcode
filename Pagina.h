#pragma once

//Feito por Joaquim Lucena e Christian Bobsin


class Pagina {
protected:	
		int id;
		int idFrame;
public:
	Pagina( int i, int f);
	
	int getID();
	int getFrameID();
	void setID(int x);
	void setFrameID(int x);
};