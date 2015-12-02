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
  ReadDeployed();
  Readbuilt();
  ReadReactorFlow();
  ReadSeparationFlow();
  ReadCoolingInv();
  ReadStorageInv();
  ReadEnrichFeed();
  ReadEnrichSWU();
  ReadStorage();

  ComputeRetiredFacility(raw_Deployed_LWR_A, raw_built_LWR_A, raw_Retired_LWR_A);
  ComputeRetiredFacility(raw_Deployed_LWR_B, raw_built_LWR_B, raw_Retired_LWR_B);
  ComputeRetiredFacility(raw_Deployed_SFR_A, raw_built_SFR_A, raw_Retired_SFR_A);
  ComputeRetiredFacility(raw_Deployed_SFR_B, raw_built_SFR_B, raw_Retired_SFR_B);

  FormFCOmap(raw_Retired_LWR_A, FCO_Retired_LWR_A, false, true, 1);
  FormFCOmap(raw_Retired_LWR_B, FCO_Retired_LWR_B, false, true, 1);
  FormFCOmap(raw_Retired_SFR_A, FCO_Retired_SFR_A, false, true, 1./0.4);
  FormFCOmap(raw_Retired_SFR_B, FCO_Retired_SFR_B, false, true, 1./0.4);



  PrintoutFile();

}


void FormFCOmap(map<int,double>& raw_infomap, map<int,double>& FCO_infomap, bool cumulativ, bool flow, double scalefactor){

  //make a copy of raw_map
  map<int,double> raw_infomap_copy = raw_infomap;
  map<int,double>::iterator it;
  map<int,double>::iterator it2;

  //sum the annual flow
  if (flow) {
    for( it = raw_infomap.begin(); it != raw_infomap.end(); it++){
      if(it->first%12 != 0){

        pair<map<int,double>::iterator,bool> ret;

        int year = it->first /12;


        ret = raw_infomap_copy.insert( pair<int, double>( 12*year, it->second));
        if(!(ret.second))
          ret.first->second += it->second;

      }
    }
  }

  // push the annual flow in the FCO map
  double prev_val_info = 0;

  for( it2 = raw_infomap_copy.begin(); it2 != raw_infomap_copy.end(); it2++){

    if(it2->first%12 == 0){

      double val_info = it2->second;

      if (cumulativ)
        val_info += prev_val_info;

      FCO_infomap.insert(pair<int, double>(it2->first/12, val_info/scalefactor));

      prev_val_info = val_info;
    }
    if (it2->first < time_min)
      time_min = it2->first;

    if (it2->first > time_max)
      time_max = it2->first;

  }

}

/* ------------------------------------------------------------------------------- */
/* -----------------------------  Generic Read & Fill ---------------------------- */
/* ------------------------------------------------------------------------------- */
void FillInfo(string filename, map<int,double>& raw_infomap, int timecol, int infocol, int colnumb){


  ifstream f_Info(filename.c_str());

  if (!f_Info.is_open()){
    cout << "something wrong with cyan, no file: " << filename << endl;
    exit(1);
  }


  // get rid of the title
  string tmp;
  getline(f_Info,tmp);


  do{
    int timestep = -1;
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


    if(timestep != -1){
      pair<map<int,double>::iterator,bool> ret;
      ret = raw_infomap.insert( pair<int, double> (timestep, info));

      //      cout << timestep << " " << info << endl;
      if (ret.second == false) {
        ret.first->second += info;
      }
    }
  }while (!f_Info.eof());

  f_Info.close();


}

/* ------------------------------------------------------------------------------- */
/* ---------------------------------  Energy ------------------------------------- */
/* ------------------------------------------------------------------------------- */
void ReadEnergy(){


  //reactor LWR_A
  string Command = "cyan -db cyclus.sqlite power -proto=LWR_A > Energy_LWR_A.txt;";
  system(Command.c_str());

  FillInfo("Energy_LWR_A.txt", raw_Energy_LWR_A);
  FormFCOmap(raw_Energy_LWR_A, FCO_Energy_LWR_A);
  FormFCOmap(raw_Energy_LWR_A, FCO_Capacity_LWR_A, false, false, 0.9*1000);
  Command = "rm -f Energy_LWR_A.txt";
  system(Command.c_str());


  //reactor LWR_B
  Command = "cyan -db cyclus.sqlite power -proto=LWR_B > Energy_LWR_B.txt;";
  system(Command.c_str());

  FillInfo("Energy_LWR_B.txt", raw_Energy_LWR_B);
  FormFCOmap(raw_Energy_LWR_B, FCO_Energy_LWR_B);
  FormFCOmap(raw_Energy_LWR_B, FCO_Capacity_LWR_B, false, false, 0.9*1000);
  Command = "rm -f Energy_LWR_B.txt";
  system(Command.c_str());


  //reactor SFR_A
  Command = "cyan -db cyclus.sqlite power -proto=SFR_A > Energy_SFR_A.txt;";
  system(Command.c_str());

  FillInfo("Energy_SFR_A.txt", raw_Energy_SFR_A);
  FormFCOmap(raw_Energy_SFR_A, FCO_Energy_SFR_A);
  FormFCOmap(raw_Energy_SFR_A, FCO_Capacity_SFR_A, false, false, 0.9*1000);
  Command = "rm -f Energy_SFR_A.txt";
  system(Command.c_str());


  //reactor SFR_B
  Command = "cyan -db cyclus.sqlite power -proto=SFR_B > Energy_SFR_B.txt";
  system(Command.c_str());

  FillInfo("Energy_SFR_B.txt", raw_Energy_SFR_B);
  FormFCOmap(raw_Energy_SFR_B, FCO_Energy_SFR_B);
  FormFCOmap(raw_Energy_SFR_B, FCO_Capacity_SFR_B, false, false, 0.9*1000);
  Command = "rm -f Energy_SFR_B.txt";
  system(Command.c_str());


}



/* ------------------------------------------------------------------------------- */
/* ---------------------------------  Reactor ------------------------------------- */
/* ------------------------------------------------------------------------------- */
void Readbuilt(){


  //reactor LWR_A
  string Command = "cyan -db cyan -db cyclus.sqlite built LWR_A > LWR_A.txt";
  system(Command.c_str());

  FillInfo("LWR_A.txt", raw_built_LWR_A);
  FormFCOmap(raw_built_LWR_A, FCO_built_LWR_A, false, true, 1);
  Command = "rm -f LWR_A.txt";
  system(Command.c_str());


  //reactor LWR_B
  Command = "cyan -db cyan -db cyclus.sqlite built LWR_B > LWR_B.txt";
  system(Command.c_str());

  FillInfo("LWR_B.txt", raw_built_LWR_B);
  FormFCOmap(raw_built_LWR_B, FCO_built_LWR_B, false, true, 1);
  Command = "rm -f LWR_B.txt";
  system(Command.c_str());


  //reactor SFR_A
  Command = "cyan -db cyan -db cyclus.sqlite built SFR_A > SFR_A.txt";
  system(Command.c_str());

  FillInfo("SFR_A.txt", raw_built_SFR_A);
  FormFCOmap(raw_built_SFR_A, FCO_built_SFR_A, false, true, 1./0.4);
  Command = "rm -f SFR_A.txt";
  system(Command.c_str());


  //reactor SFR_B
  Command = "cyan -db cyan -db cyclus.sqlite built SFR_B > SFR_B.txt";
  system(Command.c_str());

  FillInfo("SFR_B.txt", raw_built_SFR_B);
  FormFCOmap(raw_built_SFR_B, FCO_built_SFR_B, false, true, 1./0.4);
  Command = "rm -f SFR_B.txt";
  system(Command.c_str());

}



/* ------------------------------------------------------------------------------- */
/* ---------------------------------  Reactor ------------------------------------- */
/* ------------------------------------------------------------------------------- */
void ReadDeployed(){


  //reactor LWR_A
  string Command = "cyan -db cyan -db cyclus.sqlite deployed LWR_A > LWR_A.txt";
  system(Command.c_str());

  FillInfo("LWR_A.txt", raw_Deployed_LWR_A);
  FormFCOmap(raw_Deployed_LWR_A, FCO_Deployed_LWR_A, false, false, 1);
  Command = "rm -f LWR_A.txt";
  system(Command.c_str());


  //reactor LWR_B
  Command = "cyan -db cyan -db cyclus.sqlite deployed LWR_B > LWR_B.txt";
  system(Command.c_str());

  FillInfo("LWR_B.txt", raw_Deployed_LWR_B);
  FormFCOmap(raw_Deployed_LWR_B, FCO_Deployed_LWR_B, false, false, 1);
  Command = "rm -f LWR_B.txt";
  system(Command.c_str());


  //reactor SFR_A
  Command = "cyan -db cyan -db cyclus.sqlite deployed SFR_A > SFR_A.txt";
  system(Command.c_str());

  FillInfo("SFR_A.txt", raw_Deployed_SFR_A);
  FormFCOmap(raw_Deployed_SFR_A, FCO_Deployed_SFR_A, false, false, 1./0.4);
  Command = "rm -f SFR_A.txt";
  system(Command.c_str());


  //reactor SFR_B
  Command = "cyan -db cyan -db cyclus.sqlite deployed SFR_B > SFR_B.txt";
  system(Command.c_str());

  FillInfo("SFR_B.txt", raw_Deployed_SFR_B);
  FormFCOmap(raw_Deployed_SFR_B, FCO_Deployed_SFR_B, false, false, 1./0.4);
  Command = "rm -f SFR_B.txt";
  system(Command.c_str());

}



/* ------------------------------------------------------------------------------- */
/* ---------------------------------  Reactor Flow ------------------------------- */
/* ------------------------------------------------------------------------------- */
void ReadReactorFlow(){


  //reactor LWR_A
  string Command = "cyan -db cyclus.sqlite flow -to LWR_A > LWR_A.txt";
  system(Command.c_str());

  FillInfo("LWR_A.txt", raw_Flow_LWR_A);
  FormFCOmap(raw_Flow_LWR_A, FCO_Flow_LWR_A,false, true);
  Command = "rm -f LWR_A.txt";
  system(Command.c_str());


  //reactor LWR_B
  Command = "cyan -db cyclus.sqlite flow -to LWR_B > LWR_B.txt";
  system(Command.c_str());

  FillInfo("LWR_B.txt", raw_Flow_LWR_B);
  FormFCOmap(raw_Flow_LWR_B, FCO_Flow_LWR_B,false, true);
  Command = "rm -f LWR_B.txt";
  system(Command.c_str());


  //reactor SFR_A
  Command = "cyan -db cyclus.sqlite flow -to SFR_A > SFR_A.txt";
  system(Command.c_str());

  FillInfo("SFR_A.txt", raw_Flow_SFR_A);
  FormFCOmap(raw_Flow_SFR_A, FCO_Flow_SFR_A,false, true);
  Command = "rm -f SFR_A.txt";
  system(Command.c_str());


  //reactor SFR_B
  Command = "cyan -db cyclus.sqlite flow -to SFR_B > SFR_B.txt";
  system(Command.c_str());

  FillInfo("SFR_B.txt", raw_Flow_SFR_B);
  FormFCOmap(raw_Flow_SFR_B, FCO_Flow_SFR_B,false, true);
  Command = "rm -f SFR_B.txt";
  system(Command.c_str());


}


/* ------------------------------------------------------------------------------- */
/* ---------------------------------  Separation Flow ------------------------------- */
/* ------------------------------------------------------------------------------- */
void ReadSeparationFlow(){

  //reactor LWR_A
  string Command = "cyan -db cyclus.sqlite flow -to LWR_separation -commod=UOX_A_spent_stored > LWR_A_separations.txt";
  system(Command.c_str());

  FillInfo("LWR_A_separations.txt", raw_Separation_LWR_A);
  FormFCOmap(raw_Separation_LWR_A, FCO_Separation_LWR_A,false, true);
  Command = "rm -f LWR_A_separations.txt";
  system(Command.c_str());

  //reactor LWR_B
  Command = "cyan -db cyclus.sqlite flow -to LWR_separation -commod=UOX_B_spent_stored > LWR_B_separations.txt";
  system(Command.c_str());

  FillInfo("LWR_B_separations.txt", raw_Separation_LWR_B);
  FormFCOmap(raw_Separation_LWR_B, FCO_Separation_LWR_B,false, true);
  Command = "rm -f LWR_B_separations.txt";
  system(Command.c_str());


  //reactor SFR_A
  Command = "cyan -db cyclus.sqlite flow -to SFR_A_separations > SFR_A_separations.txt";
  system(Command.c_str());

  FillInfo("SFR_A_separations.txt", raw_Separation_SFR_A);
  FormFCOmap(raw_Separation_SFR_A, FCO_Separation_SFR_A,false, true);
  Command = "rm -f SFR_A_separations.txt";
  system(Command.c_str());


  //reactor SFR_B
  Command = "cyan -db cyclus.sqlite flow -to SFR_B_separations > SFR_B_separations.txt";
  system(Command.c_str());

  FillInfo("SFR_B_separations.txt", raw_Separation_SFR_B);
  FormFCOmap(raw_Separation_SFR_B, FCO_Separation_SFR_B,false, true);
  Command = "rm -f SFR_B_separations.txt";
  system(Command.c_str());


}


/* ------------------------------------------------------------------------------- */
/* ---------------------------------  Storage Inv -------------------------------- */
/* ------------------------------------------------------------------------------- */
void ReadStorageInv(){


  //reactor LWR_A
  string Command = "cyan -db cyclus.sqlite inv LWR_A_storage > LWR_storage_A.txt";
  system(Command.c_str());

  FillInfo("LWR_storage_A.txt", raw_Storage_LWR_A);
  FormFCOmap(raw_Storage_LWR_A, FCO_Storage_LWR_A);
  Command = "rm -f LWR_storage_A.txt";
  system(Command.c_str());


  //reactor LWR_B
  Command = "cyan -db cyclus.sqlite inv LWR_B_storage > LWR_storage_B.txt";
  system(Command.c_str());

  FillInfo("LWR_storage_B.txt", raw_Storage_LWR_B);
  FormFCOmap(raw_Storage_LWR_B, FCO_Storage_LWR_B);
  Command = "rm -f LWR_storage_B.txt";
  system(Command.c_str());


  //reactor SFR_A
  Command = "cyan -db cyclus.sqlite inv SFR_A_storage > SFR_A_storage.txt";
  system(Command.c_str());

  FillInfo("SFR_A_storage.txt", raw_Storage_SFR_A);
  FormFCOmap(raw_Storage_SFR_A, FCO_Storage_SFR_A);
  Command = "rm -f SFR_A_storage.txt";
  system(Command.c_str());


  //reactor SFR_B
  Command = " cyan -db cyclus.sqlite inv SFR_B_storage > SFR_B_storage.txt";
  system(Command.c_str());

  FillInfo("SFR_B_storage.txt", raw_Storage_SFR_B);
  FormFCOmap(raw_Storage_SFR_B, FCO_Storage_SFR_B);
  Command = "rm -f SFR_B_storage.txt";
  system(Command.c_str());
  
  //RU Storage
  Command = " cyan -db cyclus.sqlite inv RU_Storage > RU_storage.txt";
  system(Command.c_str());

  FillInfo("RU_storage.txt", raw_Storage_RU);
  FormFCOmap(raw_Storage_RU, FCO_Storage_RU);
  Command = "rm -f RU_storage.txt";
  system(Command.c_str());
  
  //DU Storage
  Command = " cyan -db cyclus.sqlite inv DU_Storage > DU_storage.txt";
  system(Command.c_str());

  FillInfo("DU_storage.txt", raw_Storage_DU);
  FormFCOmap(raw_Storage_DU, FCO_Storage_DU);
  Command = "rm -f DU_storage.txt";
  system(Command.c_str());


}


/* ------------------------------------------------------------------------------- */
/* ---------------------------------  Cooling Inv -------------------------------- */
/* ------------------------------------------------------------------------------- */
void ReadCoolingInv(){


  //reactor LWR_A
  string Command = "cyan -db cyclus.sqlite inv LWR_A_cooling > LWR_cooling_A.txt";
  system(Command.c_str());

  FillInfo("LWR_cooling_A.txt", raw_cooling_LWR_A);
  FormFCOmap(raw_cooling_LWR_A, FCO_cooling_LWR_A);
  Command = "rm -f LWR_cooling_A.txt";
  system(Command.c_str());


  //reactor LWR_B
  Command = "cyan -db cyclus.sqlite inv LWR_B_cooling > LWR_cooling_B.txt";
  system(Command.c_str());

  FillInfo("LWR_cooling_B.txt", raw_cooling_LWR_B);
  FormFCOmap(raw_cooling_LWR_B, FCO_cooling_LWR_B);
  Command = "rm -f LWR_cooling_B.txt";
  system(Command.c_str());


  //reactor SFR_A
  Command = "cyan -db cyclus.sqlite inv SFR_A_cooling > SFR_A_cooling.txt";
  system(Command.c_str());

  FillInfo("SFR_A_cooling.txt", raw_cooling_SFR_A);
  FormFCOmap(raw_cooling_SFR_A, FCO_cooling_SFR_A);
  Command = "rm -f SFR_A_cooling.txt";
  system(Command.c_str());


  //reactor SFR_B
  Command = " cyan -db cyclus.sqlite inv SFR_B_cooling > SFR_B_cooling.txt";
  system(Command.c_str());

  FillInfo("SFR_B_cooling.txt", raw_cooling_SFR_B);
  FormFCOmap(raw_cooling_SFR_B, FCO_cooling_SFR_B);
  Command = "rm -f SFR_B_cooling.txt";
  system(Command.c_str());


}

/* ------------------------------------------------------------------------------- */
/* -------------------------------  Enrichment feed ------------------------------ */
/* ------------------------------------------------------------------------------- */
void ReadEnrichFeed(){


  string Command = "cyan -db cyclus.sqlite table TimeSeriesEnrichmentFeed > TimeSeriesEnrichmentFeed.txt";
  system(Command.c_str());

  FillInfo("TimeSeriesEnrichmentFeed.txt", raw_EnrichFeed, 2, 3 ,4);
  FormFCOmap(raw_EnrichFeed, FCO_EnrichFeed, true, true);
  Command = "rm -f TimeSeriesEnrichmentFeed.txt";
  system(Command.c_str());


}


/* ------------------------------------------------------------------------------- */
/* -------------------------------  Enrichment SWU ------------------------------ */
/* ------------------------------------------------------------------------------- */
void ReadEnrichSWU(){


  string Command = "cyan -db cyclus.sqlite table TimeSeriesEnrichmentSWU > TimeSeriesEnrichmentSWU.txt";
  system(Command.c_str());

  FillInfo("TimeSeriesEnrichmentSWU.txt", raw_EnrichSWU, 2, 3 ,4);
  FormFCOmap(raw_EnrichSWU, FCO_EnrichSWU, false, true);
  Command = "rm -f TimeSeriesEnrichmentSWU.txt";
  system(Command.c_str());


}

double get_val_at(int time, map<int,double> my_map){

  map<int,double>::iterator it =  my_map.find(time);

  if( it == my_map.end())
    return 0;
  else
    return it->second;

}


void ComputeRetiredFacility(map<int,double> deployed_map, map<int,double> built_map, map<int, double>& retired_map){

  map<int,double>::iterator it;
  double previous_val = deployed_map.begin()->first;

  for( it = deployed_map.begin(); it != deployed_map.end(); it++){

    int built = 0;
    map<int,double>::iterator it2 = built_map.find(it->first);
    if (it2 != built_map.end())
      built = it2->second;

    double retired = it->second - previous_val - built;

    retired_map.insert(pair<int, double>(it->first, (-1)*retired));

    previous_val = it->second;

  }

}



/* ------------------------------------------------------------------------------- */
/* ---------------------------------  Storage Inv -------------------------------- */
/* ------------------------------------------------------------------------------- */
void ReadStorage(){

  {
  //reactor LWR_A
  string Command = "cyan -db cyclus.sqlite inv -nucs=94239 LWR_A_storage > storage_pu.txt";
  system(Command.c_str());

  FillInfo("storage_pu.txt", raw_Storage_pu);
  Command = "rm -f storage_pu.txt";
  system(Command.c_str());


  //reactor LWR_B
  Command = "cyan -db cyclus.sqlite inv -nucs=94239 LWR_B_storage > storage_pu.txt";
  system(Command.c_str());

  FillInfo("storage_pu.txt", raw_Storage_pu);
  Command = "rm -f storage_pu.txt";
  system(Command.c_str());


  //reactor SFR_A
  Command = "cyan -db cyclus.sqlite inv -nucs=94239 SFR_A_storage > storage_pu.txt";
  system(Command.c_str());

  FillInfo("storage_pu.txt", raw_Storage_pu);
  Command = "rm -f storage_pu.txt";
  system(Command.c_str());


  //reactor SFR_B
  Command = " cyan -db cyclus.sqlite inv -nucs=94239 SFR_B_storage > storage_pu.txt";
  system(Command.c_str());

  FillInfo("storage_pu.txt", raw_Storage_pu);
  Command = "rm -f storage_pu.txt";
  system(Command.c_str());

  FormFCOmap(raw_Storage_pu, FCO_Storage_pu);
  }

  //reactor LWR_A
  string Command = "cyan -db cyclus.sqlite inv -nucs=95241 LWR_A_storage > storage_MA.txt";
  system(Command.c_str());

  FillInfo("storage_MA.txt", raw_Storage_MA);
  Command = "rm -f storage_MA.txt";
  system(Command.c_str());


  //reactor LWR_B
  Command = "cyan -db cyclus.sqlite inv -nucs=95241 LWR_B_storage > storage_MA.txt";
  system(Command.c_str());

  FillInfo("storage_MA.txt", raw_Storage_MA);
  Command = "rm -f storage_MA.txt";
  system(Command.c_str());


  //reactor SFR_A
  Command = "cyan -db cyclus.sqlite inv -nucs=95241 SFR_A_storage > storage_MA.txt";
  system(Command.c_str());

  FillInfo("storage_MA.txt", raw_Storage_MA);
  Command = "rm -f storage_MA.txt";
  system(Command.c_str());


  //reactor SFR_B
  Command = "cyan -db cyclus.sqlite inv -nucs=95241 SFR_B_storage > storage_MA.txt";
  system(Command.c_str());

  FillInfo("storage_MA.txt", raw_Storage_MA);
  Command = "rm -f storage_MA.txt";
  system(Command.c_str());

  FormFCOmap(raw_Storage_MA, FCO_Storage_MA);

}


void   PrintoutFile(){

  ofstream Output("FCO_Output.txt");

  Output << "Time ";
  Output << "Energy_LWR_A\tEnergy_LWR_B\tEnergy_SFR_A\tEnergy_SFR_B\t";
  Output << "Capacity_LWR_A\tCapacity_LWR_B\tCapacity_SFR_A\tCapacity_SFR_B\t";
  Output << "built_LWR_A\tbuilt_LWR_B\tbuilt_SFR_A\tbuilt_SFR_B\t";
  Output << "FreshFuel_LWR_A\tFreshFuel_LWR_B\tFreshFuel_SFR_A\tFreshFuel_SFR_B\t";
  Output << "Separation_LWR_A\tSeparation_LWR_B\tSeparation_SFR_A\tSeparation_SFR_B\t";
  Output << "Cooling_LWR_A\tCooling_LWR_B\tCooling_SFR_A\tCooling_SFR_B\t";
  Output << "Storage_LWR_A\tStorage_LWR_B\tStorage_SFR_A\tStorage_SFR_B\t";
  Output << "EnrichFeed\tEnrichSWU\t";
  Output << "retired_LWR_A\tretired_LWR_B\tretired_SFR_A\tretired_SFR_B\t";
  Output << "stored_Pu\tstored_MA\t";
  Output << "depleted_U\Recycled_U\t";
  Output << endl;

  for(int i = time_min/12; i < time_max/12+1; i++){
    Output << i+init_year << " ";
    Output << get_val_at(i, FCO_Energy_LWR_A) << " ";
    Output << get_val_at(i, FCO_Energy_LWR_B) << " ";
    Output << get_val_at(i, FCO_Energy_SFR_A) << " ";
    Output << get_val_at(i, FCO_Energy_SFR_B) << " ";
    Output << get_val_at(i, FCO_Capacity_LWR_A) << " ";
    Output << get_val_at(i, FCO_Capacity_LWR_B) << " ";
    Output << get_val_at(i, FCO_Capacity_SFR_A) << " ";
    Output << get_val_at(i, FCO_Capacity_SFR_B) << " ";
    Output << get_val_at(i, FCO_built_LWR_A) << " ";
    Output << get_val_at(i, FCO_built_LWR_B) << " ";
    Output << get_val_at(i, FCO_built_SFR_A) << " ";
    Output << get_val_at(i, FCO_built_SFR_B) << " ";
    Output << get_val_at(i, FCO_Flow_LWR_A) << " ";
    Output << get_val_at(i, FCO_Flow_LWR_B) << " ";
    Output << get_val_at(i, FCO_Flow_SFR_A) << " ";
    Output << get_val_at(i, FCO_Flow_SFR_B) << " ";
    Output << get_val_at(i, FCO_Separation_LWR_A) << " ";
    Output << get_val_at(i, FCO_Separation_LWR_B) << " ";
    Output << get_val_at(i, FCO_Separation_SFR_A) << " ";
    Output << get_val_at(i, FCO_Separation_SFR_B) << " ";
    Output << get_val_at(i, FCO_cooling_LWR_A) << " ";
    Output << get_val_at(i, FCO_cooling_LWR_B) << " ";
    Output << get_val_at(i, FCO_cooling_SFR_A) << " ";
    Output << get_val_at(i, FCO_cooling_SFR_B) << " ";
    Output << get_val_at(i, FCO_Storage_LWR_A) << " ";
    Output << get_val_at(i, FCO_Storage_LWR_B) << " ";
    Output << get_val_at(i, FCO_Storage_SFR_A) << " ";
    Output << get_val_at(i, FCO_Storage_SFR_B) << " ";
    Output << get_val_at(i, FCO_EnrichFeed) << " ";
    Output << get_val_at(i, FCO_EnrichSWU) << " ";
    Output << get_val_at(i, FCO_Retired_LWR_A) << " ";
    Output << get_val_at(i, FCO_Retired_LWR_B) << " ";
    Output << get_val_at(i, FCO_Retired_SFR_A) << " ";
    Output << get_val_at(i, FCO_Retired_SFR_B) << " ";
    Output << get_val_at(i, FCO_Storage_pu) << " ";
    Output << get_val_at(i, FCO_Storage_MA) << " ";
    Output << get_val_at(i, FCO_Storage_DU) << " ";
    Output << get_val_at(i, FCO_Storage_RU) << " ";
    Output << endl;
    
    
  }
  
}

/*
 
 g++ Gene.cxx
 
 */
