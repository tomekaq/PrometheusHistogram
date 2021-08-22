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
	for (int i = 1; i <= end; i += 1)
	{
		if (i < 10)
			fileNames.push_back(path + "DAT000001_merged.part.split000" + to_string(i));
		if (i >= 10 && i < 100)
			fileNames.push_back(path + "DAT000001_merged.part.split00" + to_string(i));
		if (i >= 100 && i < 1000)
			fileNames.push_back(path + "DAT000001_merged.part.split0" + to_string(i));
		if (i >= 1000)
			fileNames.push_back(path + "DAT000001_merged.part.split" + to_string(i));
	}
	return fileNames;
}

int main(int argc, char **argv)
{
	int lFile = 0;
	string path = "";
	int distanceX = 0;
	int distanceY = 0;
	//string path = "../../CorsikaSims/unthinned_shower/";
	//string path = "/home/tomekaq/Pulpit/corsika-77100/src/utils/coast/CorsikaRead/UnThining/";
	string resultsPath = "";
	ifstream shiftsFile("shifts.txt", ios::in);
	ifstream inputFile("input",ios::in);

	inputFile.is_open();
	inputFile >> path;
	inputFile >> resultsPath;
	inputFile >> lFile;
	inputFile >> distanceX;
	inputFile >> distanceY;

	TString nameRootFile("New");
	nameRootFile = resultsPath + "Results"+ distanceX +"m.root";
	ofstream checkFile(resultsPath + "checkFile.txt");
	ofstream resultsFile(resultsPath + "results.txt");
	ofstream resultsParticles(resultsPath + "resultsParticles.txt");
	TFile *rootFile = new TFile(nameRootFile, "RECREATE");
	vector<string> fileNames = vectorfileNames(lFile, path);

	float d = 100.0;

	// Set distance of detector to the center

	cout << path << " "<< lFile << " " << distanceX << " " << distanceY << endl;
	vector<Detector> vecDetectors;
	unordered_map<int, int> mapShifts;

	Detector detector(distanceX, distanceY, d);
	vecDetectors.push_back(detector);
	for (int i = 0; i < 1200; i++)
	{
		pair<int, int> vecP;
		shiftsFile >> vecP.first;
		shiftsFile >> vecP.second;
		mapShifts[vecP.first] = vecP.second;
	}
	vector<TH1F> histogramsEnergy;

	TObjArray HList(0);
	TObjArray HListEnergy(0);
	TObjArray HListPosX(0);
	TObjArray HListPosY(0);
	TObjArray HListPosXY(0);

	int gt10 = 0;
	TH1F *histogramEnergy = new TH1F("Energy all particles", "", 100, 0, 0.3);
	auto *histogramID = new TH1D("Type of particles by ID", "", 100, 0, 10.5);
	auto *hPositionX = new TH1D("X positions", "", 100, -50 + distanceX, 50 + distanceX);
	auto *hPositionY = new TH1D("Y positions", "", 100, -50, 50);
	auto *hPositionXY = new TH2D("XY positions", "", 100, -50 + distanceX, 50 + distanceX, 100, -50, 50);
	auto *hAngle = new TH1D("Particle angles", "", 360, -185, 185);

	for (int i = 1; i <= 15; i++)
	{

		TH1F *hEnergyPart = new TH1F("Energy", "", 1000, 0, 1000);

		if (i < 4)
			hEnergyPart->GetXaxis()->SetLimits(0., 0.3);

		string nameH = "Energy of particle ";
		nameH = nameH + i;
		hEnergyPart->SetName(nameH.c_str());
		hEnergyPart->GetXaxis()->SetTitle("Energy in GeV ");
		hEnergyPart->GetYaxis()->SetTitle("Amount");
		HListEnergy.Add(hEnergyPart);

		TH1F *hPartX = new TH1F("Position X", "", 100, -50 + distanceX, 50 + distanceX);
		string nameHX = "Position X of particle  ";
		nameHX = nameHX + i;
		hPartX->SetName(nameHX.c_str());
		hPartX->GetXaxis()->SetTitle("Position on X axis");
		hPartX->GetYaxis()->SetTitle("Amount");
		HListPosX.Add(hPartX);

		TH1F *hPartY = new TH1F("Position Y", "", 100, -50 + distanceY, 50 + distanceY);
		string nameHY = "Position Y of particle ";
		nameHY = nameHY + i;
		hPartY->SetName(nameHY.c_str());
		hPartY->GetXaxis()->SetTitle("Position on Y axis");
		hPartY->GetYaxis()->SetTitle("Amount");
		HListPosY.Add(hPartY);

		TH2F *hPartXY = new TH2F("Position X & Y", "", 100, -50 + distanceX, 50 + distanceX,100, -50 + distanceY, 50 + distanceY);
		string nameHXY = "Position X and Y of particle ";
		nameHXY = nameHXY + i;
		hPartXY->SetName(nameHXY.c_str());
		hPartXY->GetXaxis()->SetTitle("Position on X axis");
		hPartXY->GetYaxis()->SetTitle("Position on Y axis");
		HListPosXY.Add(hPartXY);
	}

	auto *hEnergyID = new TH2D("Dependence between energy and type of particle", "", 100, 0, 10, 30, 0, 0.3);
	gROOT->Reset();

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
	shiftsFile.is_open();
	resultsFile.is_open();
	checkFile.is_open();
	std::time_t result = std::time(nullptr);
	resultsFile << std::asctime(std::localtime(&result)) << endl;
	resultsFile << " Results of hits particles in detector of size: " << d << " in x: " << distanceX << " y: " << distanceY << endl;
	resultsParticles << std::asctime(std::localtime(&result)) << endl;
	resultsParticles << " Results of hits particles in detector of size: " << d << " in x: " << distanceX << " y: " << distanceY << endl;

	for (int iFile = 0; iFile < fileNames.size(); ++iFile)
	{
		const string fname = fileNames[iFile];
		std::ifstream input(fname, std::ios::binary);
		checkFile << "Read file: " << fname << endl;
		int shift = 0;
		int n = 0;
		resultsParticles << fname << endl;
		shift = mapShifts[(int)stoi(fname.substr(fname.size() - 4))];

		if (input.is_open())
		{
			if (iFile % 2 == 0)
			{
				input.seekg(0, input.end);
				input.seekg(0, input.beg);
			}
			if (iFile % 2 == 1)
			{
				input.seekg(2, input.end);
				input.seekg(2, input.beg);
			}

			if (iFile == 1)
			{
				input.seekg(2, input.end);
				input.seekg(2, input.beg);
			}
			float ival = 0.0;
			float k = 0;
			//for (int i = 0; i < 300000; i++)
			while (!input.eof()) // loop over  binary file
			{
				double line[7] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
				for (int j = 0; j < 7; ++j)
				{
					input.read(reinterpret_cast<char *>(&ival), sizeof ival);
					if (k <= shift)
					{
						k++;
						j = 0;
						continue;
					}
					if (ival != 0.0 || ival != 0)
						line[j] = ival;
					else
						break;
				}
				Particle *part = new Particle(line[0], line[1], line[2], line[3], line[4], line[5], line[6]);
				float radiusXY = radius(part->GetX(), part->GetY());

				if (hit(*part, vecDetectors[0]) && part->GetID() != 200 && part->GetTime() != 200)
				{
					numberofHits++;
					int partID = part->GetID();
					double partEnergy = part->GetE();
					double partX = part->GetX();
					double partY = part->GetY();
					energy += partEnergy;
					histogramEnergy->Fill(partEnergy);
					histogramID->Fill(partID);
					hPositionX->Fill(partX);
					hPositionY->Fill(partY);
					hPositionXY->SetFillColor(kBlue - 2);
					hPositionXY->Fill(partX, partY);
					hEnergyID->Fill(partID, partEnergy);
					hAngle->Fill(angle(partX, partY));
					stats.addParticles(*part);

					if (partID > 10)
						gt10++;
					if (partID < 15)
					{
						TH1D *hEnergy = (TH1D *)HListEnergy[partID - 1];
						hEnergy->Fill(partEnergy);

						TH1D *hX = (TH1D *)HListPosX[partID - 1];
						hX->Fill(partX);

						TH1D *hY = (TH1D *)HListPosY[partID - 1];
						hY->Fill(partY);

						TH2D *hXY = (TH2D *)HListPosXY[partID - 1];
						hXY->Fill(partX,partY);
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

	auto c1 = new TCanvas("Energy20", "Particle Energy", 1200, 800);
	histogramEnergy->GetXaxis()->SetTitle("Energy in GeV ");
	histogramEnergy->GetYaxis()->SetTitle("Amount");
	histogramEnergy->Draw();
	c1->Print("hEnergy.png");

	auto c2 = new TCanvas("ID", "Particle ID", 1200, 800);
	histogramID->GetXaxis()->SetTitle("ID");
	histogramID->GetYaxis()->SetTitle("Amount");
	histogramID->Draw();
	gPad->SetLogy();
	c2->Print("hID.png");

	auto c3 = new TCanvas("XY", "Particle position", 1200, 800);
	hPositionXY->Draw("colz");
	c3->Print("hXY.png");

	auto c4 = new TCanvas("Energy, type of particle", "Particle position", 1200, 800);
	hEnergyID->Draw();
	c4->Print("hEnergyID.png");

	auto c5 = new TCanvas("Particle angles", "Particle position", 1200, 800);
	hAngle->Draw();
	c5->Print("hAngle.png");

	auto c6 = new TCanvas("X", "Particle position", 1200, 800);
	hPositionX->Draw();
	c6->Print("hX.png");

	auto c7 = new TCanvas("Y", "Particle position", 1200, 800);
	hPositionY->Draw();
	c7->Print("hY.png");
	HList.Write();

	HListEnergy.Write();
	HListPosX.Write();
	HListPosY.Write();
	for (int i = 0; i < 15; i++)
	{
		TH2D* hXY = (TH2D*)HListPosXY[i];
		hXY->SetOption("colz");
	}
	HListPosXY.Write();

	shiftsFile.close();
	std::time_t result2 = std::time(nullptr);
	resultsFile << "End: " << std::asctime(std::localtime(&result2)) << endl;
	resultsFile << "Total energy: " << energy << endl;
	resultsFile << "Number of particles which have ID > 10: " << gt10 << endl;

	stats.Show(resultsFile);

	resultsFile.close();
	rootFile->Close();
	checkFile.close();
	return 0;
}
