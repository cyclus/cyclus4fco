#include "Gene.hxx"

#include <string>
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <sstream>


int main(int argc, char ** argv){


    if(argc < 1 ){
        std::cout <<" not arg" << endl;
        exit(1);
    }
    filename = argv[1];

    Read("flowto", "Reactor", reactorIn, -1, true);
    Read("flowfrom", "Reactor", reactorOut, -1, true);



    for(int i=0; i < 17; i++)
        Read("inv", "Sink", stored[i], nucleilist[i]);


    PrintoutFile();

}


string Form_cyanCommand(std::string info, std::string facility, int nuclei = -1){


    string command = "cyan -db ";
    command += filename;
    command += " ";

    if(info== "inv")
        command += "inv ";
    else if(info == "flowto")
        command += "flow -to ";
    else if( info == "flowfrom")
        command += "flow -from ";

    if (nuclei > 0) {
        command += "-nucs=";
        command += itoa(nuclei);
        command += " ";
    }

    command += facility;

    return command;
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
void FillInfo(string filename, map<int,double>& raw_infomap, int timecol, int infocol, int colnumb, bool cumulativ){


    ifstream f_Info(filename.c_str());

    if (!f_Info.is_open()){
        cout << "something wrong with cyan, no file: " << filename << endl;
        exit(1);
    }


    // get rid of the title
    string tmp;
    getline(f_Info,tmp);

    double previous = 0;

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
            ret = raw_infomap.insert( pair<int, double> (timestep, info + previous));

            if (cumulativ) previous += info;
            //      cout << timestep << " " << info << endl;
            if (ret.second == false) {
                ret.first->second += info;
            }
        }
    }while (!f_Info.eof());

    f_Info.close();


}




/* ------------------------------------------------------------------------------- */
/* ---------------------------------  Storage Inv -------------------------------- */
/* ------------------------------------------------------------------------------- */
void Read(string info, string facility, map<int, double>& map2fill,  int nuclei, bool cumulativ){

    string Command = Form_cyanCommand( info, facility, nuclei);

    Command += " > tmp.txt";
    system(Command.c_str());

    if (info != "TimeSeriesEnrichment") {
        FillInfo("tmp.txt", map2fill, 0, 1, 2, cumulativ);
    }
    Command = "rm -f tmp.txt";
    system(Command.c_str());




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



double get_val_at(int time, map<int,double> my_map){

    map<int,double>::iterator it =  my_map.find(time);

    if( it == my_map.end())
        return 0;
    else
        return it->second;
    
}

void   PrintoutFile(){

    ofstream Output("Output.txt");

    Output << "Time[month],";
    Output << "reactor_In[kg],Reactor_Out[kg],";
    Output << endl;

    for(int i = time_min; i < time_max+1; i++){
        Output << i+init_year << ",";
        Output << get_val_at(i, reactorIn) << ",";
        Output << get_val_at(i, reactorOut) << ",";
        Output << endl;
    }
    Output << endl << endl;

    Output << "Spent Fuel Inventories," << endl;
    Output << "Nucleus,Q[kg]" << endl;
        for(int i = 0; i<17; i++)
            Output << nucleilist[i] << "," << get_val_at(time_max -1, stored[i]) << ","<< endl ;



}

/*

 g++ Gene.cxx

 */
