// argv[1]:   input.txt
#include "Def.h"
using namespace std;

int main(int argc, char* argv[]){
    vector<baseStation> BS_list;
    
    // Read Input //
    if(readInput(argv[1],BS_list)==false)
        return 0;
    
    // Schedule //
    RBalloc(BS_list);
    
    // Calc Sub-band SINR of all UEs //
    calcsubSINR(BS_list);
    
    // Calc avg. SINR //
    calcavgSINR(BS_list);
    
    // Select UE CQI by SINR //
    for(int i=0;i<BS_list.size();i++)
        for(int j=0;j<BS_list[i].UE_list.size();j++)
            BS_list[i].UE_list[j].CQI=selectCQI(BS_list[i].UE_list[j].avgSINR);
    
    //showUEsinr(BS_list);
    showUEinfo(BS_list);
    showUEallocRB(BS_list);
    showBSinfo(BS_list);
    
    return 0;
}