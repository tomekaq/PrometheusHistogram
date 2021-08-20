#include "Particle.h"
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

#define PI 3.14159265

using namespace std;

class Detector{
	public:
		double Xcenter;	
		double Ycenter;
		double size[4];
		Detector (double x,double y,double d)
		{
			Xcenter = x;
			Ycenter = y;	
			size[0] = Xcenter - d/2.0;
			size[1] = Xcenter + d/2.0;
			size[2] = Ycenter - d/2.0;
			size[3] = Ycenter + d/2.0;
		};
		void PrintSize()
		{
		 	cout << size[0]<< " " << size[1]<<" " << size[2]<<" " << size[3]  << endl;
		}
		friend class Particle;
};

bool hit( Particle iPart, Detector detector ){
	
	//iPart.Show();
	if ( iPart.GetX() > detector.size[0] && iPart.GetX() < detector.size[1]
	&&   iPart.GetY() > detector.size[2] && iPart.GetY() < detector.size[3])
	{	
		return true;
	}	
	return false;
}

float radius(float x, float y)
{
	return sqrt(pow(x,2) + pow(y,2));
}

float angle(float x, float y)
{
        return atan2(y,x) * 180/PI;
}

bool hitRadius( Particle iPart, Detector detector ){

        if ( radius( iPart.GetX(), iPart.GetY() ) < radius(detector.size[0], detector.size[1]) )
        {
                return true;
        }
	return false;
}

bool hitRadius( Particle iPart, float radiusA ){

        if ( radius( iPart.GetX(), iPart.GetY() ) < radiusA )
        {
                return true;
        }
	return false;
}
