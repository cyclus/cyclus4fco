#include "Gene.hxx"

#include <string>
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <sstream>


int main(int argc, char ** argv){

  ReadEnergy();
  ReadReactorFlow();
  ReadSeparationFlow();
  ReadStorageInv();
  ReadCoolingInv();
  ReadEnrichFeed();
  ReadEnrichSWU();

  PrintoutFile();

}


void FormFCOmap(map<int,double>& raw_infomap, map<int,double>& FCO_infomap, bool flow = false){

  //make a copy of raw_map
  map<int,double> raw_infomap_copy = raw_infomap;
  map<int,double>::iterator it;

//sum the annual flow
  if (flow) {
    for( it = raw_infomap.begin; it != raw_infomap.end; it++){
      if(it.first%12 != 0)
        raw_infomap_copy[it.first] += it.second;
    }
  }

// push the annual flow in the FCO map
  for( it = raw_infomap_copy.begin; it != raw_infomap_copy.end; it++){
    if(it.first%12 == 0)
      FCO_infomap.insert(it);
  }

}

/* ------------------------------------------------------------------------------- */
/* -----------------------------  Generic Read & Fill ---------------------------- */
/* ------------------------------------------------------------------------------- */
void FillInfo(string filename, map<int,double>& raw_infomap, int timecol, int infocol, int colnumb){


  ifstream f_Info(filename.c_srt());

  if (!f_Info.is_open()){
    cout << "something wrong with cyan, no file: " << filename << endl:
    exit(1);
  }


  // get rid of the title
  string tmp;
  getline(f_Info,tmp);


  do{
    int timestep = 0;
    double info = 0;
    string dummy;

    for(int i = 0; i < colnumb; i++){
      if( i == timecol){
        f_Info >> timestep;
      }
      else if ( i == infocol){
        f_Info >> info;
      }
      else{
        f_Info >> dummy;
      }
    }


    pair<map<int,double>::iterator,bool> ret;

    ret = raw_infomap.insert( pair<int, double> (timestep, info));
    if (ret.second = false) {
      cout << "Pb reading the info file: " << filename << "...template should have change : 2 timestep are identical" << emdl;
      exit(1);
    }
  }while (!f_Info.eof());
  
  f_Info.close();
  
  
}




/* ------------------------------------------------------------------------------- */
/* ---------------------------------  Energy ------------------------------------- */
/* ------------------------------------------------------------------------------- */
void ReadEnergy(){


  //reactor LWR_A
  string Command = "cyan -db cyclus.sqlite power -proto=LWR_A > Energy_LWR_A.txt;"
  system(Command.c_str());

  FillInfo("Energy_LWR_A.txt", raw_Energy_LWR_A);

  Command = "rm -f Energy_LWR_A.txt";
  system(Command.c_str());


  //reactor LWR_B
  string Command = "cyan -db cyclus.sqlite power -proto=LWR_B > Energy_LWR_B.txt;"
  system(Command.c_str());

  FillInfo("Energy_LWR_B.txt", raw_Energy_LWR_B);

  Command = "rm -f Energy_LWR_B.txt";
  system(Command.c_str());


  //reactor SFR_A
  string Command = "cyan -db cyclus.sqlite power -proto=SFR_A > Energy_SFR_A.txt;"
  system(Command.c_str());

  FillInfo("Energy_SFR_A.txt", raw_Energy_SFR_A);

  Command = "rm -f Energy_SFR_A.txt";
  system(Command.c_str());


  //reactor SFR_B
  string Command = "cyan -db cyclus.sqlite power -proto=SFR_B > Energy_SFR_B.txt"
  system(Command.c_str());

  FillInfo("Energy_SFR_B.txt", raw_Energy_SFR_B);

  Command = "rm -f Energy_SFR_B.txt";
  system(Command.c_str());


}


/* ------------------------------------------------------------------------------- */
/* ---------------------------------  Reactor Flow ------------------------------- */
/* ------------------------------------------------------------------------------- */
void ReadReactorFlow(){


  //reactor LWR_A
  string Command = "cyan -db cyclus.sqlite flow -to lwr_A > LWR_A.txt"
  system(Command.c_str());

  FillInfo("LWR_A.txt", raw_Flow_LWR_A);

  Command = "rm -f LWR_A.txt";
  system(Command.c_str());


  //reactor LWR_B
  string Command = "cyan -db cyclus.sqlite flow -to lwr_B > LWR_B.txt"
  system(Command.c_str());

  FillInfo("LWR_B.txt", raw_Flow_LWR_B);

  Command = "rm -f LWR_B.txt";
  system(Command.c_str());


  //reactor SFR_A
  string Command = "cyan -db cyclus.sqlite flow -to SFR_A > SFR_A.txt"
  system(Command.c_str());

  FillInfo("SFR_A.txt", raw_Flow_SFR_A);

  Command = "rm -f SFR_A.txt";
  system(Command.c_str());


  //reactor SFR_B
  string Command = "cyan -db cyclus.sqlite flow -to SFR_B > SFR_B.txt"
  system(Command.c_str());

  FillInfo("SFR_B.txt", raw_Flow_SFR_B);

  Command = "rm -f SFR_B.txt";
  system(Command.c_str());


}


/* ------------------------------------------------------------------------------- */
/* ---------------------------------  Reactor Flow ------------------------------- */
/* ------------------------------------------------------------------------------- */
void ReadSeparationFlow(){


  //reactor LWR
  string Command = "cyan -db cyclus.sqlite flow -to LWR_separations > LWR_separations.txt"
  system(Command.c_str());

  FillInfo("LWR_separations.txt", raw_Separation_LWR);

  Command = "rm -f LWR_separations.txt";
  system(Command.c_str());


  //reactor SFR_A
  string Command = "cyan -db cyclus.sqlite flow -to SFR_A_separations > SFR_A_separations.txt"
  system(Command.c_str());

  FillInfo("SFR_A_separations.txt", raw_Separation_SFR_A);

  Command = "rm -f SFR_A_separations.txt";
  system(Command.c_str());


  //reactor SFR_B
  string Command = "cyan -db cyclus.sqlite flow -to SFR_B_separations > SFR_B_separations.txt"
  system(Command.c_str());

  FillInfo("SFR_B_separations.txt", raw_Separation_SFR_B);

  Command = "rm -f SFR_B_separations.txt";
  system(Command.c_str());
  
  
}


/* ------------------------------------------------------------------------------- */
/* ---------------------------------  Storage Inv -------------------------------- */
/* ------------------------------------------------------------------------------- */
void ReadStorageInv(){


  //reactor LWR_A
  string Command = "cyan -db cyclus.sqlite inv LWR_storage_A > LWR_storage_A.txt"
  system(Command.c_str());

  FillInfo("LWR_storage_A.txt", raw_Storage_LWR_A);

  Command = "rm -f LWR_storage_A.txt";
  system(Command.c_str());


  //reactor LWR_B
  string Command = "cyan -db cyclus.sqlite inv LWR_storage_B > LWR_storage_B.txt"
  system(Command.c_str());

  FillInfo("LWR_storage_B.txt", raw_Storage_LWR_B);

  Command = "rm -f LWR_storage_B.txt";
  system(Command.c_str());


  //reactor SFR_A
  string Command = "cyan -db cyclus.sqlite inv SFR_A_storage > SFR_A_storage.txt"
  system(Command.c_str());

  FillInfo("SFR_A_storage.txt", raw_Storage_SFR_A);

  Command = "rm -f SFR_A_storage.txt";
  system(Command.c_str());


  //reactor SFR_B
  string Command = " cyan -db cyclus.sqlite inv SFR_B_storage > SFR_B_storage.txt"
  system(Command.c_str());

  FillInfo("SFR_B_storage.txt", raw_Storage_SFR_B);

  Command = "rm -f SFR_B_storage.txt";
  system(Command.c_str());
  
  
}


/* ------------------------------------------------------------------------------- */
/* ---------------------------------  Cooling Inv -------------------------------- */
/* ------------------------------------------------------------------------------- */
void ReadCoolingInv(){


  //reactor LWR_A
  string Command = "cyan -db cyclus.sqlite inv LWR_Cooling_A > LWR_Cooling_A.txt"
  system(Command.c_str());

  FillInfo("LWR_Cooling_A.txt", raw_Cooling_LWR_A);

  Command = "rm -f LWR_Cooling_A.txt";
  system(Command.c_str());


  //reactor LWR_B
  string Command = "cyan -db cyclus.sqlite inv LWR_Cooling_B > LWR_Cooling_B.txt"
  system(Command.c_str());

  FillInfo("LWR_Cooling_B.txt", raw_Cooling_LWR_B);

  Command = "rm -f LWR_Cooling_B.txt";
  system(Command.c_str());


  //reactor SFR_A
  string Command = "cyan -db cyclus.sqlite inv SFR_A_Cooling > SFR_A_Cooling.txt"
  system(Command.c_str());

  FillInfo("SFR_A_Cooling.txt", raw_Cooling_SFR_A);

  Command = "rm -f SFR_A_Cooling.txt";
  system(Command.c_str());


  //reactor SFR_B
  string Command = " cyan -db cyclus.sqlite inv SFR_B_Cooling > SFR_B_Cooling.txt"
  system(Command.c_str());

  FillInfo("SFR_B_Cooling.txt", raw_Cooling_SFR_B);

  Command = "rm -f SFR_B_Cooling.txt";
  system(Command.c_str());
  
  
}

/* ------------------------------------------------------------------------------- */
/* -------------------------------  Enrichment feed ------------------------------ */
/* ------------------------------------------------------------------------------- */
void ReadEnrichFeed(){


  string Command = "cyan -db cyclus.sqlite table TimeSeriesEnrichmentFeed > TimeSeriesEnrichmentFeed.txt"
  system(Command.c_str());

  FillInfo("TimeSeriesEnrichmentFeed.txt", raw_EnrichFeed, 2, 3 ,4);

  Command = "rm -f TimeSeriesEnrichmentFeed.txt";
  system(Command.c_str());
  
  
}


/* ------------------------------------------------------------------------------- */
/* -------------------------------  Enrichment SWU ------------------------------ */
/* ------------------------------------------------------------------------------- */
void ReadEnrichSWU(){


  string Command = "cyan -db cyclus.sqlite table TimeSeriesEnrichmentSWU > TimeSeriesEnrichmentSWU.txt"
  system(Command.c_str());

  FillInfo("TimeSeriesEnrichmentSWU.txt", raw_EnrichSWU, 2, 3 ,4);

  Command = "rm -f TimeSeriesEnrichmentSWU.txt";
  system(Command.c_str());
  
  
}

/*
 
 g++ Gene.cxx `root-config --cflags` `root-config --libs` -I ./ PuEquiv.o
 
 */
