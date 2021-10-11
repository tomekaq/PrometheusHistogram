//#include "Detector.h"
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

enum ParticlesName{
    photon = 1,
    electron = 2,
    positron = 3,
    mionplus = 5,
    mionminus = 6
};

class Statistic
{
	public:
		int totalParticles = 0;
		double totalEnergy = 0;
		long int particles[200];
		vector<double> energy = vector<double>(200);
		friend class Particle;

	Statistic()
	{
		auto it = energy.begin();
		for (int i =0; i < 200; i++)
		{	//energy.insert(it,0);
			particles[i] = 0;
		}	
	}

	void addParticles(Particle part)
	{
		int ID = part.GetID();
		double ePart = part.GetE();
		totalParticles ++;			 
		
		particles[ID-1] ++;
		try{
			if( !isnan(ePart) && ePart < 100000){
				energy.at(ID-1) += ePart; 
				totalEnergy += ePart;		
			}
		}
		catch (std::out_of_range const& exc) {
		  	cout << ID << endl;
		        std::cout << exc.what() << '\n';
		}
		
	}
	float findMin()
	{
		return *min_element(std::begin(energy), std::end(energy)-1);
	}
	
	float findMax()
	{
		return *max_element(std::begin(energy), std::end(energy)-1);
	}
	
	void Show(ofstream &File)
	{	
		cout << "-----------------------------------------------------------------------------------------" << endl;
		File 	<< " Total number of particles: " << totalParticles << " total energy: " << totalEnergy << endl 
			<< " Number of photon: " << particles[0]   << " total energy: " << energy.at(0) << endl
			<< " Number of electron: " << particles[1] << " total energy: " << energy.at(1) << endl
			<< " Number of positron: " << particles[2]  << " total energy: " << energy.at(2) <<  endl
			<< " Number of mion plus: " << particles[4]  << " total energy: " << energy.at(4) <<  endl
			<< " Number of mion minus: " << particles[5]  << " total energy: " << energy.at(5) <<  endl
			<< " Number of pion neutral: " << particles[6]  << " total energy: " << energy.at(6) <<  endl
			<< " Number of pion plus: " << particles[7]  << " total energy: " << energy.at(7) <<  endl
			<< " Number of pion minus: " << particles[8]  << " total energy: " << energy.at(8) <<  endl
			<< " Number of kaon zero-Long: " << particles[9]  << " total energy: " << energy.at(9) <<  endl
			<< " Number of kaon plus: " << particles[10]  << " total energy: " << energy.at(10) <<  endl
			<< " Number of kaon minus:  " << particles[11]  << " total energy: " << energy.at(11) <<  endl
			<< " Number of neutron: " << particles[12]  << " total energy: " << energy.at(12) <<  endl
			<< " Number of proton: " << particles[13]  << " total energy: " << energy.at(13) <<  endl
			<< " Number of antiprotron: " << particles[14]  << " total energy: " << energy.at(14) <<  endl
			<< " Number of lambda: " << particles[17]  << " total energy: " << energy.at(17) <<  endl
			<< " Number of antineutron: " << particles[24]  << " total energy: " << energy.at(24) <<  endl
			<< " Number of neutrino electron: " << particles[65]  << " total energy: " << energy.at(65) <<  endl
			<< " Number of antineutrino tau: " << particles[66]  << " total energy: " << energy.at(66) <<  endl
			<< " Number of neutrino mion: " << particles[67]  << " total energy: " << energy.at(67) <<  endl
			<< " Number of antineutrino mion: " << particles[68]  << " total energy: " << energy.at(68) <<  endl
			<< " Number of neutrino tau: " << particles[132]  << " total energy: " << energy.at(132) <<  endl
			<< " Number of antineutrino tau: " << particles[133]  << " total energy: " << energy.at(133) <<  endl
			<< " Number of wrong value:" << partices[199] << << " total energy: " << energy.at(199) <<  endl
			<< " Min: " << findMin() << " Max: " << findMax() << endl;

	}
};
