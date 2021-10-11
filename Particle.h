#include <ctime>
#include <filesystem>
#include <fstream>
#include <string>
#include <endian.h>
#include <iostream>
#include <sstream>
#include <map>
#include <cmath>
#include <bitset>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

class Particle{
	public:
		int _ID; 
		int _GenID;
		double _x;
		double _y;
		double _time;
		double _px;
		double _py;
		double _pz;
		
	Particle(int ID,double px,double py, double pz, double x, double y,double time)
	{
		_ID = SetParticleID (ID);
		_GenID = ID;	
		_px = px;
		_py = py;
		_pz = pz;	
		_x = x;
		_y = y;
		_time = SetParticleTime(time); //
	}

	bool acceptID(int ID)
	{
		ID = ID / 1000;
        	if (ID > 0 and ID != 4 and (ID < 33 or ID > 49) and (ID < 70 or ID > 115  ) and(ID < 200))
                	return true;
        	else 
			return false;
	}
	bool acceptTime(double time)
	{
		if (time < 6000 || time > 750000)
			return false;
		return true;
	}
	inline int SetParticleID (int ID) 
	{
		if (acceptID(ID) == 1)
			return (int) ID /1000;
		else
			return 200;
	}
	inline double SetParticleTime (double time)
        {
                if (acceptTime(time))
                        return (double) time;
                else
                    	return 200;
        }

	inline int GetID() const 
	{
		return _ID;
	}
	inline int GetGenID() const
        {
                return _GenID;
        }

	inline double GetX() const
	{
		return _x;
	}
	inline double GetY() const
	{
		return _y;
	}
	inline double GetTime() const
	{
		return _time;
	}
	double GetPSquared() const
	{
		double P;
		P = _px *_px +  _py *_py + _pz * _pz;
    		//P = std::sqrt (P);
		return P;
	}
	double GetE() const {
		if (_ID != 200 ) 
			return std::sqrt(GetPSquared() + std::pow(GetMass(_ID-1),2)); 
		else	
			return 0;
	}
	double GetMass(int ID) const
	{
	  static const float gParticleMass [200] = {
     0.e0	,.51099892e-3,.51099892e-3,  0.e0     ,.105658369e0,
     .105658369e0, .1349766e0, .13957018e0,.13957018e0, 0.497648e0 ,//10
     0.493677e0 , 0.493677e0 ,.93956536e0 ,.93827203e0,.93827203e0 ,
     0.497648e0 , 0.54775e0  , 1.115683e0 , 1.18937e0 , 1.192642e0 ,//20
     1.197449e0 , 1.31483e0  , 1.32131e0  , 1.67245e0 ,.93956536e0 ,
     1.115683e0 , 1.18937e0  , 1.192642e0 , 1.197449e0, 1.31483e0  ,//30
     1.32131e0  , 1.67245e0  , 0.e0	  , 0.e0      , 0.e0	   ,
     0.e0	, 0.e0       , 0.e0	  , 0.e0      , 0.e0	   ,//40
     1.e9	, 580.e0     , 1.e5	  , 0.e0      , 0.e0	   ,
     0.e0	, 0.e0       , 0.e0	  , 0.e0      , 0.78259e0  ,//50
     0.7690e0	, 0.7665e0   , 0.7665e0   , 1.2305e0  , 1.2318e0   ,
     1.2331e0	, 1.2344e0   , 1.2309e0   , 1.2323e0  , 1.2336e0   ,//60
     1.2349e0	, 0.89610e0  , 0.89166e0  , 0.89166e0 , 0.89610e0  ,
     0.e0	, 0.e0       , 0.e0	  , 0.e0      , 0.e0	   ,//70
     0.54775e0  , 0.54775e0  , 0.54775e0  , 0.54775e0 ,.105658369e0,
     .105658369e0 , 0.e0       , 0.e0	  , 0.e0      , 0.e0	   ,//80
     0.e0	, 0.e0       , 0.e0	  , 0.e0      ,.105658369e0,
     .105658369e0, 0.e0       , 0.e0	  , 0.e0      , 0.e0	   ,//90
     0.e0	, 0.e0       , 0.e0	  , 0.e0      ,.105658369e0,
     .105658369e0, 0.e0       , 0.e0	  , 0.e0      , 0.e0	   ,//100
     0.e0	, 0.e0       , 0.e0	  , 0.e0      , 0.e0	   ,
     0.e0	, 0.e0       , 0.e0	  , 0.e0      , 0.e0	   ,//110
     0.e0	, 0.e0       , 0.e0	  , 0.e0      , 0.e0	   ,
     1.8645e0	, 1.8697e0   , 1.8697e0   , 1.8645e0   , 1.9682e0   ,//120
     1.9682e0	, 2.9804e0   , 2.0067e0   , 2.0100e0   , 2.0100e0   ,
     2.0067e0	, 2.1121e0   , 2.1121e0   , 0.0e0      , 3.096916e0 ,//130
     1.77699e0  , 1.77699e0  , 0.e0	  , 0.e0       , 0.e0	    ,
     0.e0	, 2.28646e0  , 2.4679e0   , 2.4710e0   , 2.45402e0  ,//140
     2.4529e0	, 2.45376e0  , 2.5757e0   , 2.5780e0   , 2.6975e0   ,
     0.e0	, 0.e0       , 0.e0	  , 2.28646e0  , 2.4679e0   ,//150
     2.4710e0	, 2.45402e0  , 2.4529e0   , 2.45376e0  , 2.5757e0   ,
     2.5780e0	, 2.6975e0   , 0.e0	  , 0.e0       , 0.e0	    ,//160
     2.5184e0	, 2.5175e0   , 2.5180e0   , 0.e0       , 0.e0	    ,
     0.e0	, 0.e0       , 0.e0	  , 0.e0       , 0.e0	    ,//170
     2.5184e0	, 2.5175e0   , 2.5180e0   , 0.e0       , 0.e0	    ,
     5.27958e0  , 5.27925e0  , 5.27925e0  , 5.27958e0  , 5.36677e0  ,//180
     5.36677e0  , 6.277e0    , 6.277e0    , 5.6194e0   , 5.8155e0   ,
     5.8113e0	, 5.788e0    , 5.7911e0   , 6.071e0    , 5.6194e0   ,//190
     5.8155e0	, 5.8113e0   , 5.788e0    , 5.7911e0   , 6.071e0    ,
     0.e0	, 0.e0       , 0.e0	  , 0.e0       , 0.e0	    }; //200

		if (ID < 200 && ID > 0)
     			return gParticleMass[ID-1]; 
		return 0;
	}
	
	void Show()
	{
		cout	<< setprecision(8);
		cout 	<< "Particle ID: " << setw(3)  <<_ID 
			<< "   x: " << setw(18) 	<< _x 
			<< "   y: "<< 	setw(18) 	<< _y 
			<< "   energy: "<< setw(18) 	<< GetE()
			<< "   time: "<< setw(18) 	<< _time 
			<< endl;
 	}
};
