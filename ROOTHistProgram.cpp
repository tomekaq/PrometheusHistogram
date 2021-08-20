#include <stdio.h>
#include <ctime>
#include <filesystem>
#include <string>
#include <endian.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <cmath>
#include <bitset>
#include <deque>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h> 
#include "Detector.h"
#include "Statistic.h"

#define PI 3.14159265

typedef unsigned char BYTE;
using namespace std;

vector<string> vectorfileNames(int end, string path)
{
    vector<string> fileNames;
    for (int i = 1; i <= end;i+=1)
    {
        if (i < 10)
            fileNames.push_back(path + "DAT000001_merged.part.split000" + to_string(i));
        if(i >= 10 &&i < 100 )
            fileNames.push_back(path + "DAT000001_merged.part.split00" + to_string(i));
        if(i >= 100 &&i < 1000)
            fileNames.push_back(path + "DAT000001_merged.part.split0" + to_string(i));
        if(i >= 1000)
            fileNames.push_back(path + "DAT000001_merged.part.split" + to_string(i));
    }
    return fileNames;
}

int main (int argc, char **argv) 
{   
	//string path = "";
	string path = "../../CorsikaSims/unthinned_shower/";
	string resultsPath = "results2-10m/";
	ifstream myfile ("shifts.txt",ios::in);
	ofstream checkFile (resultsPath+"checkFile.txt");
	ofstream resultsFile (resultsPath+"results.txt");
	ofstream resultsParticles(resultsPath+"resultsParticles.txt");
	TFile *MyFile = new TFile("Event.root","RECREATE");
	 
	vector<string> fileNames = vectorfileNames(1200,path);

	float d = 100.0;
	
	// Set distance of detector to the center 

	int distanceX = 1000;
	int distanceY = 0;

	vector<Detector> vecDetectors;
	unordered_map<int,int> mapShifts;

	Detector detector(distanceX,distanceY,d);
	vecDetectors.push_back(detector);

	for (int i = 0; i< 1200;i++)
	{
	    pair<int,int> vecP;
	    myfile >> vecP.first;
	    myfile >> vecP.second;
	    mapShifts[vecP.first] = vecP.second;
	}
    vector<TH1F> histogramsEnergy;

	TObjArray HList(0);
	int gt10=0;	
	TH1F *histogramEnergy = new TH1F("Energy", "", 100, 0, 0.3);
	auto *histogramID = new TH1D("Type of particles by ID", "" ,100, 0,10.5);
	auto *hPositionX = new TH1D("X positions"     , "" ,100, -50+distanceX,100+distanceX);
	auto *hPositionY = new TH1D("Y positions"     , "", 100, -50,50);
	auto *hPositionXY = new TH2D("XY positions"	, "" ,100, -50+distanceX,50+distanceX,100, -50,  50);
	auto *hAngle = new TH1D("Particle angles"     , "" ,360, -185,185);

	TH1F *histogramEnergy1 = new TH1F("Energy1", "", 100, 0, 0.3);
	TH1F *histogramEnergy2 = new TH1F("Energy2", "", 100, 0, 0.3);
	TH1F *histogramEnergy3 = new TH1F("Energy3", "", 100, 0, 0.3);
	TH1F *histogramEnergy5 = new TH1F("Energy5", "", 100, 0, 1000);
	TH1F *histogramEnergy6 = new TH1F("Energy6", "", 100, 0, 1000);
	TH1F *histogramEnergy7 = new TH1F("Energy7", "", 100, 0, 1000);
	TH1F *histogramEnergy8 = new TH1F("Energy8", "", 100, 0, 1000);
	TH1F *histogramEnergy9 = new TH1F("Energy9", "", 100, 0, 1000);
	TH1F *histogramEnergy10 = new TH1F("Energy10", "", 100, 0, 1000);
	TH1F *histogramEnergy11 = new TH1F("Energy11", "", 100, 0, 1000);
	TH1F *histogramEnergy12 = new TH1F("Energy12", "", 100, 0, 1000);

	histogramsEnergy.push_back(*histogramEnergy1);
    histogramsEnergy[0].Draw();
	auto *hEnergyID = new TH2D("Dependence between energy and type of particle"	, "" ,100, 0,10,30,0,0.3);
	gROOT->Reset(); 

	//HList.Add(histogramEnergy);
	HList.Add(histogramEnergy1);
	HList.Add(histogramEnergy2);
    HList.Add(histogramEnergy3);
        HList.Add(histogramEnergy5);
        HList.Add(histogramEnergy6);
        HList.Add(histogramEnergy7);
        HList.Add(histogramEnergy8);
        HList.Add(histogramEnergy9);
        HList.Add(histogramEnergy10);
        HList.Add(histogramEnergy11);
        HList.Add(histogramEnergy12);

	HList.Add(histogramID);
	HList.Add(hPositionXY);
	HList.Add(hPositionX);
	HList.Add(hPositionY);
	HList.Add(hEnergyID);
	HList.Add(hAngle);
	
	int numberofHits = 0;
	int ShowerCounter = 0;
        double energy = 0;
	long long int totalNumberofHits = 0;
	Statistic stats;
	
	const string outputName;
        myfile.is_open();
        resultsFile.is_open();
	checkFile.is_open();
	std::time_t result = std::time(nullptr);
	resultsFile << std::asctime(std::localtime(&result)) << endl;
        resultsFile << " Results of hits particles in detector of size: " << d << " in x: " << distanceX << " y: " << distanceY << endl;
	resultsParticles << std::asctime(std::localtime(&result)) << endl;
	resultsParticles << " Results of hits particles in detector of size: " << d << " in x: " << distanceX << " y: " << distanceY << endl;


	for( int iFile = 0; iFile<fileNames.size(); ++iFile) 
	{
		const string fname = fileNames[iFile];
		std::ifstream input( fname, std::ios::binary );
		checkFile << "Read file: " << fname << endl;
		int shift = 0;
		int n = 0;
	        resultsParticles <<fname << endl;
		shift = mapShifts[(int)stoi(fname.substr(fname.size() - 4))];
		cout << shift << endl;
		if(input.is_open())
		{		
			if (iFile % 2 == 0 ){
                                input.seekg (0, input.end);
                                input.seekg (0, input.beg);
                        }
                        if (iFile % 2 == 1){
                                input.seekg (2, input.end);
                                input.seekg (2, input.beg);
                        }
		
			if (iFile == 1){
				input.seekg (2, input.end);
                                input.seekg (2, input.beg);	
			}
			float ival = 0.0;
			float k=0;
			while(!input.eof())  // loop over  binary file
	       		{ 	
       				double line [7] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0}; 
			   		for (int j = 0; j < 7; ++j)
                                	{
                                      	  	input.read(reinterpret_cast<char*>(&ival) , sizeof ival);
	                                        if ( k<= shift)
	                                        {
                                                	k ++;
                                                	j = 0;
                                                	continue;
                                                }
                                                if (ival != 0.0 || ival !=0)
                                                	line[j] = ival;
                                                else	
                                                	break;
                                        }	
					Particle* part = new Particle(line[0],line[1],line[2],line[3],line[4],line[5],line[6]);
					float radiusXY = radius(part->GetX(),part->GetY());

       					if(  hit(*part,vecDetectors[0]) && part->GetID() != 200 && part->GetTime() != 200)
       					{
       						numberofHits ++; 
			                	energy += part->GetE();						
						histogramEnergy -> Fill(part->GetE() );
						histogramID->Fill(part->GetID() );
						hPositionX->Fill( part->GetX() );
						hPositionY->Fill( part->GetY() );
						hPositionXY->SetFillColor( kBlue-2);
						hPositionXY->Fill( part->GetX(), part->GetY() );
						hEnergyID -> Fill ( part->GetID(),part->GetE() ) ;
						hAngle -> Fill(angle(part->GetX(),part->GetY() ));
						stats.addParticles(*part);	
						
						if (part->GetID() > 10)
							gt10 ++;
						if (part->GetID() == 1)
						{
							histogramEnergy1 -> Fill(part->GetE() );
						   	continue;
					        }

						if (part->GetID() == 2)
						    histogramEnergy2 -> Fill(part->GetE() );
						if (part->GetID() == 3)
						    histogramEnergy3 -> Fill(part->GetE() );
						if (part->GetID() == 5)
						    histogramEnergy5 -> Fill(part->GetE() );
						if (part->GetID() == 6)
						    histogramEnergy6 -> Fill(part->GetE() );
  						if (part->GetID() == 7)
  						    histogramEnergy7 -> Fill(part->GetE() );
						if (part->GetID() == 8)
						    histogramEnergy8 -> Fill(part->GetE() );
						if (part->GetID() == 9)
						    histogramEnergy9 -> Fill(part->GetE() );
						if (part->GetID() == 10)
						    histogramEnergy10 -> Fill(part->GetE() );
                                                if (part->GetID() == 11)
                                                   histogramEnergy11 -> Fill(part->GetE() );
                                                if (part->GetID() == 12)
                                                {
							histogramEnergy12 -> Fill(part->GetE() );
						}
					}
					delete part;
        		} 
			
			resultsFile << fname << endl;
			resultsFile << "Number of hits: " << numberofHits << endl;
			totalNumberofHits += numberofHits;
			resultsFile << "Total number of hits: " << totalNumberofHits << endl;
			input.close();	
		}
		numberofHits = 0;
	}
	

	auto c1  = new TCanvas("Energy20","Particle Energy",1200,800);
	histogramEnergy ->GetXaxis()->SetTitle("Energy in GeV ");
	histogramEnergy ->GetYaxis()->SetTitle("Amount");
	histogramEnergy->Draw();
	c1->Print("hEnergy.png");
	
	auto c2  = new TCanvas("ID","Particle ID",1200,800);
	histogramID->GetXaxis()->SetTitle("ID");
	histogramID->GetYaxis()->SetTitle("Amount");
	histogramID->Draw();
	gPad->SetLogy(); 
	c2->Print("hID.png");
	
	auto c3  = new TCanvas("XY","Particle position",1200,800);
	hPositionXY->Draw("colz");
	c3->Print("hXY.png");
	
	auto c4  = new TCanvas("Energy, type of particle","Particle position",1200,800);
	hEnergyID->Draw();
	c4->Print("hEnergyID.png");

	auto c5  = new TCanvas("Particle angles","Particle position",1200,800);
	hAngle->Draw();
	c5->Print("hAngle.png");

	auto c6 = new TCanvas("X","Particle position",1200,800);
	hPositionX->Draw();
	c6->Print("hX.png");

	auto c7  = new TCanvas("Y","Particle position",1200,800);
	hPositionY->Draw();
	c7->Print("hY.png");
	HList.Write();

	myfile.close();
	std::time_t result2 = std::time(nullptr);
	resultsFile << "End: " << std::asctime(std::localtime(&result2)) << endl;
        resultsFile << "Total energy: " << energy << endl;
	resultsFile << "Number of particles which have ID > 10: " << gt10 << endl;

	stats.Show(resultsFile);

	resultsFile.close();
	MyFile->Close();
	checkFile.close();
	return 0;
}		
