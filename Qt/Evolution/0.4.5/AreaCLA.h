#pragma once
                    
#include <vector>
#include <Windows.h>
#include <random>  
#include "Prim.h"

#define MIN_COUNT_OF_CRT 1
#define COUNT_OF_CRT 8

#define UNI unsigned short int
using namespace std;

class Prim;
class CreationC;

class AreaCLA
{
    bool _countOfCtrnsChenged;
	Prim*** _map;
	int _heigh;//x
	int _width;//y
	UNI _countOfFood;
	UNI _countOfPoison;   
    UNI _FPCount;
    
    vector <CreationC*> _crtns;        
                        
    //void _regeneration();
	void _craftAreaCLA();
    void _cicle();
    void _refresh();
    void _print();

public:
	AreaCLA(int heigh);
	~AreaCLA();
	void Print();
	void SetFood(COORD coord);
	void SetPoison(COORD coord);
	void SetWall(COORD coord);
	void SetVoid(COORD coord);
	void Start();

	void minFood();//����� ���
	void minPoison();//����� ��
    void delCrt();// ������� �������� �� �������
	Prim*** GetArea();
};

