#include <ZAI.hxx>
#include <TGraph.h>
#include <TGraph.h>

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <map>
#include <iostream>
#include <iomanip>
using namespace std;

string dtoa(double num)
{
	ostringstream os(ostringstream::out);
	os<<setprecision(3)<<num;
	return os.str();
}

string itoa(int num)
{
  ostringstream os(ostringstream::out);
  os << num;
  return os.str();
}
string itof(double num)
{
  ostringstream os(ostringstream::out);
  os << num;
  return os.str();
}

void FillInfo(string filename, map<int,double>& raw_infomap, int timecol = 0, int infocol = 1, int colnumb = 2);
void FormFCOmap(map<int,double>& raw_infomap, map<int,double>& FCO_infomap, bool flow = false);


int init_year = 2015;

void ReadEnergy();
void ReadReactorFlow();
void ReadSeparationFlow();
void ReadStorageInv();
void ReadCoolingInv();
void ReadEnrichFeed();
void ReadEnrichSWU();



map<int,double> raw_Energy_LWR_A;
map<int,double> raw_Energy_LWR_B;
map<int,double> raw_Energy_SFR_A;
map<int,double> raw_Energy_SFR_B;


map<int,double> raw_Flow_LWR_A;
map<int,double> raw_Flow_LWR_B;
map<int,double> raw_Flow_SFR_A;
map<int,double> raw_Flow_SFR_B;


map<int,double> raw_Separation_LWR;
map<int,double> raw_Separation_SFR_A;
map<int,double> raw_Separation_SFR_B;


map<int,double> raw_Storage_LWR_A;
map<int,double> raw_Storage_LWR_B;
map<int,double> raw_Storage_SFR_A;
map<int,double> raw_Storage_SFR_B;


map<int,double> raw_Cooling_LWR_A;
map<int,double> raw_Cooling_LWR_B;
map<int,double> raw_Cooling_SFR_A;
map<int,double> raw_Cooling_SFR_B;


map<int,double> raw_EnrichFeed;
