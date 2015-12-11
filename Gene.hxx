
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

void FillInfo(string filename, map<int,double>& raw_infomap, int timecol = 0, int infocol = 1, int colnumb = 2, bool cumulativ = false);
void FormFCOmap(map<int,double>& raw_infomap, map<int,double>& FCO_infomap, bool cumulativ = false, bool flow = false, double scalefactor = 1000);
void Read(string info, string facility, map<int, double>& map2fill,  int nuclei =-1, bool cumulativ = false);


int init_year = 0;

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


int time_max = 1200;
int time_min = 0;

string filename;

map<int, double> reactorIn;
map<int, double> reactorOut;

map<int, double> stored[17];

int nucleilist[17] = {
    92234, 92235, 92236, 92238,
    94238, 94239, 94240, 94241, 94242,
    95241, 95242, 95243,
    96242, 96243, 96244, 96245,
    93237};