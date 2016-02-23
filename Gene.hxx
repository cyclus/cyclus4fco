
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
void FormFCOmap(map<int,double>& raw_infomap, map<int,double>& FCO_infomap, bool cumulativ = false, bool flow = false, double scalefactor = 1000);


int init_year = 2015;

void ReadEnergy();
void ReadDeployed();
void Readbuilt();
void ReadReactorFlow();
void ReadSeparationFlow();
void ReadStorageInv();
void ReadCoolingInv();
void ReadEnrichFeed();
void ReadEnrichSWU();
void ReadStorage();
void ComputeRetiredFacility(map<int,double> deployed , map<int,double> built, map<int, double>& retired_map);
void PrintoutFile();


double get_val_at(int i, map<int,double> my_map);


int time_max = 0;
int time_min = 9999999;


map<int,double> raw_Energy_LWR_A;
map<int,double> raw_Energy_LWR_B;
map<int,double> raw_Energy_SFR_A;
map<int,double> raw_Energy_SFR_B;

map<int,double> raw_built_LWR_A;
map<int,double> raw_built_LWR_B;
map<int,double> raw_built_SFR_A;
map<int,double> raw_built_SFR_B;

map<int,double> raw_Deployed_LWR_A;
map<int,double> raw_Deployed_LWR_B;
map<int,double> raw_Deployed_SFR_A;
map<int,double> raw_Deployed_SFR_B;

map<int,double> raw_Retired_LWR_A;
map<int,double> raw_Retired_LWR_B;
map<int,double> raw_Retired_SFR_A;
map<int,double> raw_Retired_SFR_B;

map<int,double> raw_Flow_LWR_A;
map<int,double> raw_Flow_LWR_B;
map<int,double> raw_Flow_SFR_A;
map<int,double> raw_Flow_SFR_B;

map<int,double> raw_Separation_LWR_A;
map<int,double> raw_Separation_LWR_B;
map<int,double> raw_Separation_SFR_A;
map<int,double> raw_Separation_SFR_B;

map<int,double> raw_Storage_LWR_A;
map<int,double> raw_Storage_LWR_B;
map<int,double> raw_Storage_SFR_A;
map<int,double> raw_Storage_SFR_B;

map<int,double> raw_cooling_LWR_A;
map<int,double> raw_cooling_LWR_B;
map<int,double> raw_cooling_SFR_A;
map<int,double> raw_cooling_SFR_B;

map<int,double> raw_EnrichFeed;
map<int,double> raw_EnrichSWU;


map<int,double> raw_HLW_U;
map<int,double> raw_HLW_Pu;
map<int,double> raw_HLW_MA;
map<int,double> raw_HLW_FP;


map<int,double> raw_Storage_pu;
map<int,double> raw_Storage_MA;

map<int,double> raw_Storage_RU;
map<int,double> raw_Storage_DU;

map<int,double> FCO_Energy_LWR_A;
map<int,double> FCO_Energy_LWR_B;
map<int,double> FCO_Energy_SFR_A;
map<int,double> FCO_Energy_SFR_B;

map<int,double> FCO_built_LWR_A;
map<int,double> FCO_built_LWR_B;
map<int,double> FCO_built_SFR_A;
map<int,double> FCO_built_SFR_B;

map<int,double> FCO_Deployed_LWR_A;
map<int,double> FCO_Deployed_LWR_B;
map<int,double> FCO_Deployed_SFR_A;
map<int,double> FCO_Deployed_SFR_B;

map<int,double> FCO_Retired_LWR_A;
map<int,double> FCO_Retired_LWR_B;
map<int,double> FCO_Retired_SFR_A;
map<int,double> FCO_Retired_SFR_B;

map<int,double> FCO_Capacity_LWR_A;
map<int,double> FCO_Capacity_LWR_B;
map<int,double> FCO_Capacity_SFR_A;
map<int,double> FCO_Capacity_SFR_B;

map<int,double> FCO_Flow_LWR_A;
map<int,double> FCO_Flow_LWR_B;
map<int,double> FCO_Flow_SFR_A;
map<int,double> FCO_Flow_SFR_B;

map<int,double> FCO_Separation_LWR_A;
map<int,double> FCO_Separation_LWR_B;
map<int,double> FCO_Separation_SFR_A;
map<int,double> FCO_Separation_SFR_B;

map<int,double> FCO_Storage_LWR_A;
map<int,double> FCO_Storage_LWR_B;
map<int,double> FCO_Storage_SFR_A;
map<int,double> FCO_Storage_SFR_B;

map<int,double> FCO_cooling_LWR_A;
map<int,double> FCO_cooling_LWR_B;
map<int,double> FCO_cooling_SFR_A;
map<int,double> FCO_cooling_SFR_B;

map<int,double> FCO_EnrichFeed;
map<int,double> FCO_EnrichSWU;

map<int,double> FCO_HLW_U;
map<int,double> FCO_HLW_Pu;
map<int,double> FCO_HLW_MA;
map<int,double> FCO_HLW_FP;


map<int,double> FCO_Storage_pu;
map<int,double> FCO_Storage_MA;

map<int,double> FCO_Storage_RU;
map<int,double> FCO_Storage_DU;