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
      if(it.first%12 != 0){

        pair<map<int,double>::iterator,bool> ret;

        ret = raw_infomap.insert( it);
        if(!(ret.second))
          ret.first.second += it.second;

      }
    }
  }

  // push the annual flow in the FCO map
  for( it = raw_infomap_copy.begin; it != raw_infomap_copy.end; it++){

    if(it.first%12 == 0)
      FCO_infomap.insert(it);

    if (it.first < time_min)
      time_min = it.first;
    if (it.first > time_max)
      time_max = it.first;

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
  FormFCOmap(raw_Energy_LWR_A, FCO_Energy_LWR_A);
  Command = "rm -f Energy_LWR_A.txt";
  system(Command.c_str());


  //reactor LWR_B
  string Command = "cyan -db cyclus.sqlite power -proto=LWR_B > Energy_LWR_B.txt;"
  system(Command.c_str());

  FillInfo("Energy_LWR_B.txt", raw_Energy_LWR_B);
  FormFCOmap(raw_Energy_LWR_B, FCO_Energy_LWR_B);
  Command = "rm -f Energy_LWR_B.txt";
  system(Command.c_str());


  //reactor SFR_A
  string Command = "cyan -db cyclus.sqlite power -proto=SFR_A > Energy_SFR_A.txt;"
  system(Command.c_str());

  FillInfo("Energy_SFR_A.txt", raw_Energy_SFR_A);
  FormFCOmap(raw_Energy_SFR_A, FCO_Energy_SFR_A);
  Command = "rm -f Energy_SFR_A.txt";
  system(Command.c_str());


  //reactor SFR_B
  string Command = "cyan -db cyclus.sqlite power -proto=SFR_B > Energy_SFR_B.txt"
  system(Command.c_str());

  FillInfo("Energy_SFR_B.txt", raw_Energy_SFR_B);
  FormFCOmap(raw_Energy_SFR_B, FCO_Energy_SFR_B);
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
  FormFCOmap(raw_Flow_LWR_A, FCO_Flow_LWR_A);
  Command = "rm -f LWR_A.txt";
  system(Command.c_str());


  //reactor LWR_B
  string Command = "cyan -db cyclus.sqlite flow -to lwr_B > LWR_B.txt"
  system(Command.c_str());

  FillInfo("LWR_B.txt", raw_Flow_LWR_B);
  FormFCOmap(raw_Flow_LWR_B, FCO_Flow_LWR_B);
  Command = "rm -f LWR_B.txt";
  system(Command.c_str());


  //reactor SFR_A
  string Command = "cyan -db cyclus.sqlite flow -to SFR_A > SFR_A.txt"
  system(Command.c_str());

  FillInfo("SFR_A.txt", raw_Flow_SFR_A);
  FormFCOmap(raw_Flow_SFR_A, FCO_Flow_SFR_A);
  Command = "rm -f SFR_A.txt";
  system(Command.c_str());


  //reactor SFR_B
  string Command = "cyan -db cyclus.sqlite flow -to SFR_B > SFR_B.txt"
  system(Command.c_str());

  FillInfo("SFR_B.txt", raw_Flow_SFR_B);
  FormFCOmap(raw_Flow_SFR_B, FCO_Flow_SFR_B);
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
  FormFCOmap(raw_Separation_LWR, FCO_Separation_LWR);
  Command = "rm -f LWR_separations.txt";
  system(Command.c_str());


  //reactor SFR_A
  string Command = "cyan -db cyclus.sqlite flow -to SFR_A_separations > SFR_A_separations.txt"
  system(Command.c_str());

  FillInfo("SFR_A_separations.txt", raw_Separation_SFR_A);
  FormFCOmap(raw_Separation_SFR_A, FCO_Separation_SFR_A);
  Command = "rm -f SFR_A_separations.txt";
  system(Command.c_str());


  //reactor SFR_B
  string Command = "cyan -db cyclus.sqlite flow -to SFR_B_separations > SFR_B_separations.txt"
  system(Command.c_str());

  FillInfo("SFR_B_separations.txt", raw_Separation_SFR_B);
  FormFCOmap(raw_Separation_SFR_B, FCO_Separation_SFR_B);
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
  FormFCOmap(raw_Storage_LWR_A, FCO_Storage_LWR_A);
  Command = "rm -f LWR_storage_A.txt";
  system(Command.c_str());


  //reactor LWR_B
  string Command = "cyan -db cyclus.sqlite inv LWR_storage_B > LWR_storage_B.txt"
  system(Command.c_str());

  FillInfo("LWR_storage_B.txt", raw_Storage_LWR_B);
  FormFCOmap(raw_Storage_LWR_B, FCO_Storage_LWR_B);
  Command = "rm -f LWR_storage_B.txt";
  system(Command.c_str());


  //reactor SFR_A
  string Command = "cyan -db cyclus.sqlite inv SFR_A_storage > SFR_A_storage.txt"
  system(Command.c_str());

  FillInfo("SFR_A_storage.txt", raw_Storage_SFR_A);
  FormFCOmap(raw_Storage_SFR_A, FCO_Storage_SFR_A);
  Command = "rm -f SFR_A_storage.txt";
  system(Command.c_str());


  //reactor SFR_B
  string Command = " cyan -db cyclus.sqlite inv SFR_B_storage > SFR_B_storage.txt"
  system(Command.c_str());

  FillInfo("SFR_B_storage.txt", raw_Storage_SFR_B);
  FormFCOmap(raw_Storage_SFR_B, FCO_Storage_SFR_B);
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
  FormFCOmap(raw_Cooling_LWR_A, FCO_Cooling_LWR_A);
  Command = "rm -f LWR_Cooling_A.txt";
  system(Command.c_str());


  //reactor LWR_B
  string Command = "cyan -db cyclus.sqlite inv LWR_Cooling_B > LWR_Cooling_B.txt"
  system(Command.c_str());

  FillInfo("LWR_Cooling_B.txt", raw_Cooling_LWR_B);
  FormFCOmap(raw_Cooling_LWR_B, FCO_Cooling_LWR_B);
  Command = "rm -f LWR_Cooling_B.txt";
  system(Command.c_str());


  //reactor SFR_A
  string Command = "cyan -db cyclus.sqlite inv SFR_A_Cooling > SFR_A_Cooling.txt"
  system(Command.c_str());

  FillInfo("SFR_A_Cooling.txt", raw_Cooling_SFR_A);
  FormFCOmap(raw_Cooling_SFR_A, FCO_Cooling_SFR_A);
  Command = "rm -f SFR_A_Cooling.txt";
  system(Command.c_str());


  //reactor SFR_B
  string Command = " cyan -db cyclus.sqlite inv SFR_B_Cooling > SFR_B_Cooling.txt"
  system(Command.c_str());

  FillInfo("SFR_B_Cooling.txt", raw_Cooling_SFR_B);
  FormFCOmap(raw_Cooling_SFR_B, FCO_Cooling_SFR_B);
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
  FormFCOmap(raw_EnrichFeed, FCO_EnrichFeed);
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
  FormFCOmap(raw_EnrichSWU, FCO_EnrichSWU);
  Command = "rm -f TimeSeriesEnrichmentSWU.txt";
  system(Command.c_str());


}

double get_val_at(int i, map<int,double> my_map){

  map<int,double>::iterator it =  my_map.find(i);
  if( it== my_map.end)
    return 0;
  else
    return it.second;

}


void   PrintoutFile(){

  ofstream Output(FCO_Output.txt);

  Output << "Time ";
  Output << "Energy_LWR_A Energy_LWR_B Energy_SFR_A Energy_SFR_B ";
  Output << "FreshFuel_LWR_A FreshFuel_LWR_B FreshFuel_SFR_A FreshFuel_SFR_B ";
  Output << "Separation_LWR Separation_SFR_A Separation_SFR_B ";
  Output << "Cooling_LWR_A Cooling_LWR_B Cooling_SFR_A Cooling_SFR_B ";
  Output << "Separation_LWR_A Separation_LWR_B Separation_SFR_A Separation_SFR_B ";
  Output << "EnrichFeed EnrichSWU";
  Output << endl;

  for(int i = time_min; i < time_max+1; i++){

    Output << get_val_at(i, FCO_Energy_LWR_A) << " ";
    Output << get_val_at(i, FCO_Energy_LWR_B) << " ";
    Output << get_val_at(i, FCO_Energy_SFR_A) << " ";
    Output << get_val_at(i, FCO_Energy_SFR_B) << " ";
    Output << get_val_at(i, FCO_Flow_LWR_A) << " ";
    Output << get_val_at(i, FCO_Flow_LWR_B) << " ";
    Output << get_val_at(i, FCO_Flow_SFR_A) << " ";
    Output << get_val_at(i, FCO_Flow_SFR_B) << " ";
    Output << get_val_at(i, FCO_Separation_LWR) << " ";
    Output << get_val_at(i, FCO_Separation_SFR_A) << " ";
    Output << get_val_at(i, FCO_Separation_SFR_B) << " ";
    Output << get_val_at(i, FCO_Cooling_LWR_A) << " ";
    Output << get_val_at(i, FCO_Cooling_LWR_B) << " ";
    Output << get_val_at(i, FCO_Cooling_SFR_A) << " ";
    Output << get_val_at(i, FCO_Cooling_SFR_B) << " ";
    Output << get_val_at(i, FCO_Storage_LWR_A) << " ";
    Output << get_val_at(i, FCO_Storage_LWR_B) << " ";
    Output << get_val_at(i, FCO_Storage_SFR_A) << " ";
    Output << get_val_at(i, FCO_Storage_SFR_B) << " ";
    Output << get_val_at(i, FCO_EnrichFeed) << " ";
    Output << get_val_at(i, FCO_EnrichSWU);
    Output << endl;
    
    
  }
  
}

/*
 
 g++ Gene.cxx `root-config --cflags` `root-config --libs` -I ./ PuEquiv.o
 
 */
