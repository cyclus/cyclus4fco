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

map<int,double> raw_Energy_LWR_A;
map<int,double> raw_Energy_LWR_B;
map<int,double> raw_Energy_SFR_A;
map<int,double> raw_Energy_SFR_B;