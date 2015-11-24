#include "Gene.hxx"

#include <string>
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <sstream>

static const size_t npos = -1;
class PuEquiv;
using namespace ROOT::Minuit2;


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

int main(int argc, char ** argv){


}

void ReadEnergy(){

  string Command = "cyan -db cyclus.sqlite power -proto=LWR_A > Energy_LWR_A.txt;"
  system(Command.c_str());

  FillEnergy("Energy_LWR_A.txt", raw_Energy_LWR_A);

  Command = "rm -f Energy_LWR_A.txt";
  system(Command.c_str());


  string Command = "cyan -db cyclus.sqlite power -proto=LWR_B > Energy_LWR_B.txt;"
  system(Command.c_str());

  FillEnergy("Energy_LWR_B.txt", raw_Energy_LWR_B);

  Command = "rm -f Energy_LWR_B.txt";
  system(Command.c_str());


  string Command = "cyan -db cyclus.sqlite power -proto=SFR_A > Energy_SFR_A.txt;"
  system(Command.c_str());

  FillEnergy("Energy_SFR_A.txt", raw_Energy_SFR_A);

  Command = "rm -f Energy_SFR_A.txt";
  system(Command.c_str());


  string Command = "cyan -db cyclus.sqlite power -proto=SFR_B > Energy_SFR_B.txt"
  system(Command.c_str());

  FillEnergy("Energy_SFR_B.txt", raw_Energy_SFR_B);

  Command = "rm -f Energy_SFR_B.txt";
  system(Command.c_str());


}

void FillEnergy(string filename, map<int,double> raw_emergymap){


  ifstream f_Energy(filename.c_srt());

  if (!f_Energy.is_open()){
    cout << "something wrong with cyan, no file: " << filename << endl:
    exit(1);
  }

  string tmp;
  getline(f_Energy,tmp);


  do{
    int timestep = 0;
    double Energy = 0;
    f_Energy >> timestep >> Energy;

    pair<map<int,double>::iterator,bool> ret;

    ret = raw_emergymap.insert( pair<int, double> (timestep, Energy));
    if (ret.second = false) {
      cout << "Pb reading the energy file: " << filename << "...template should have change : 2 timestep are identical" << emdl;
      exit(1);
    }
  }while (!f_Energy.eof());

  JOB.close();

		// Remove temporary files...

}

/*
 
 g++ Gene.cxx `root-config --cflags` `root-config --libs` -I ./ PuEquiv.o
 
 */
