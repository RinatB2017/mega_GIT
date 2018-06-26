#pragma once

#include "AreaCLA.h"

AreaCLA::AreaCLA(int heigh)
{
	this->_heigh = heigh;
	this->_width = (int) (heigh * 1.5);

    _maxCrtnsCount = _heigh * _width / 4;
    _crtnsCount = 0;

    _crtns = new CreationC* [_maxCrtnsCount];

	this->_craftAreaCLA();

    _countOfCtrnsChenged = 0;
}                                                             
AreaCLA::~AreaCLA()
{
    for(int i = 0; i < _crtnsCount; i++)
    {
        if(_crtns[i]) 
            delete _crtns[i];
    }

    delete[] _crtns;

	COORD coord;
	for (coord.Y = 0; coord.Y < this->_heigh; ++coord.Y)
		for (coord.X = 0; coord.X < this->_width; ++coord.X)
		{
            if(_map[coord.Y][coord.X])
			delete this->_map[coord.Y][coord.X];
		}
	for (int i = 0; i < this->_heigh; ++i)
	{
		delete[] this->_map[i];
	}
	delete[] this->_map;
}
void AreaCLA::_craftAreaCLA()
	{

        _countOfFood = 0;
        _countOfPoison = 0;

        _FPCount = _heigh * _width / 8;

		this->_map = new Prim**[this->_heigh];
		for (int i = 0; i < this->_heigh; ++i)
		{
			this->_map[i] = new Prim*[this->_width];
		}
		COORD coord;
		for (coord.Y = 0; coord.Y < this->_heigh; ++coord.Y)
			for (coord.X = 0; coord.X < this->_width; ++coord.X)
			{
				if (coord.X == 0 || coord.Y == 0 || coord.Y == this->_heigh - 1 || coord.X == this->_width - 1)
				{
					this->_map[coord.Y][coord.X] = new Prim(Wall);
				}
				else
				{
					this->_map[coord.Y][coord.X] = new Prim(Void);
				}
				this->_map[coord.Y][coord.X]->SetX(coord.X);
				this->_map[coord.Y][coord.X]->SetY(coord.Y);
			}
        
        _refresh();

	}  
void AreaCLA::_refresh()
{
    random_device rd;   // non-deterministic generator  
    mt19937 gen(rd());  // to seed mersenne twister.  
    uniform_int_distribution<> rdH(1, _heigh - 1);
    uniform_int_distribution<> rdW(1, _width - 1);

    int y, x;
    while(_countOfFood < _FPCount)
    {
        y = rdH(gen);
        x = rdW(gen);
        if(_map[y][x]->GetType() == Void)
        {
            _map[y][x]->SetType(Food);
            _countOfFood++;
        }
    }
    while(_countOfPoison < _FPCount)
    {
        y = rdH(gen);
        x = rdW(gen);
        if(_map[y][x]->GetType() == Void)
        {
            _map[y][x]->SetType(Poison);
            _countOfPoison++;
        }
    }
}
void AreaCLA::Print()
{
	for (int y = 0; y < this->_heigh; ++y)
	{
		for (int x = 0; x < this->_width; ++x)
		{
            this->_map[y][x]->GetType();
		}
		std::cout << std::endl;
	}
}                
void AreaCLA::SetFood(COORD coord)
{
    this->_map[coord.Y][coord.X]->SetType(Food);
}        
void AreaCLA::SetPoison(COORD coord)
{
	delete this->_map[coord.Y][coord.X];
	this->_map[coord.Y][coord.X] = new Prim(Poison);
}    
void AreaCLA::SetVoid(COORD coord)
{
    this->_map[coord.Y][coord.X]->SetType(Void);
}            
void AreaCLA::SetWall(COORD coord)
{
    this->_map[coord.Y][coord.X]->SetType(Wall);
}                                 
Prim*** AreaCLA::GetArea()
{
	return this->_map;
}                 
void AreaCLA::minFood()
{
	this->_countOfFood--;
}              
void AreaCLA::minPoison()
{
	this->_countOfPoison--;
}                             
void AreaCLA::delCrt(int i, int x, int y)
{
    CreationC* temp;

    
            temp = _crtns[i];
            for(int j = i; j < _crtnsCount - 1; j++)
            {
                _crtns[j] = _crtns[j + 1];
            }
            _crtns[--_crtnsCount] = nullptr;
                    
    _countOfCtrnsChenged = 1;

    delete temp;
    _map[y][x] = new Prim(Void);
    _map[y][x]->SetX(x);
    _map[y][x]->SetY(y);
}
void AreaCLA::_cicle()
{
    for(int i = 0; i < _crtnsCount; i++)
    {
        if(!_crtns[i]) continue;
        _crtns[i]->Execute();
        if(_crtns[i]->IsAlive() == 0)
            delCrt(i, _crtns[i]->GetX(), _crtns[i]->GetY());
        
        if(_crtnsCount == MIN_COUNT_OF_CRT) 
            break; 

        if(_countOfCtrnsChenged)
        { 
            i--;
            _countOfCtrnsChenged = 0;   
            continue;
        }
    }
}
void AreaCLA::Start()
{
     random_device rd;   // non-deterministic generator  
     mt19937 gen(rd());  // to seed mersenne twister.  
     uniform_int_distribution<> rdH(1, _heigh - 2);
     uniform_int_distribution<> rdW(1, _width - 2);

     int g;
     vector<UNI> gens;
     /*cout << "Enter gens: ";
     for(int i = 0; i < 10; i++)
     {
         cin >> g;
         gens.push_back(g);
     }*/          

     uniform_int_distribution<> dist(0, 7);

     while(gens.size() < 64)
         gens.push_back(dist(gen));

     int x, y;
     while(_crtnsCount < _maxCrtnsCount)
     {
         y = rdH(gen);
         x = rdW(gen);

         if(_map[y][x]->GetType() == Void)
         {
             delete _map[y][x];

             _map[y][x] = new CreationC(gens, this, x, y);
             _crtns[_crtnsCount++] = ( CreationC*) _map[y][x];
         }
     }

         while(_crtnsCount > MIN_COUNT_OF_CRT)
         {
             _cicle();
             _refresh();
         }

      cout << _crtnsCount << endl;
}
