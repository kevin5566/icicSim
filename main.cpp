// argv[1]:   input.txt

#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<sstream>
#include<random>
#include<math.h>
#include<iomanip>
#include<numeric>

#include "Def.h"
using namespace std;

// Shadow Effect //
default_random_engine seed1;
//lognormal_distribution<double> Shadow(0.0,3.16);
normal_distribution<double> Shadow(0.0,3.16);

// Return strg value (i.e. Power-PathLoss, unit: dBm) //
double getStrg(vector<baseStation> BS_list, int i, int j, int k, int l){
    // i: BS idx
    // j: UE idx
    // k: Subband idx
    double d=sqrt(pow(BS_list[i].UE_list[j].x-BS_list[l].x,2.0)+
                  pow(BS_list[i].UE_list[j].y-BS_list[l].y,2.0));
    double result=20.0*log10(4.0*3.14159*d_0*carrierFreq/c);
    result=result+10*n*log10(d/d_0);
    double S=Shadow(seed1);
    while(S-result>-18){
        S=Shadow(seed1);
    }
    result=result+S;
    result=-1*result;
    result=result+BS_list[i].sub_P[k];
    
    return result;
}

void calcavgSINR(vector<baseStation> &BS_list, double &SINR_max, double &SINR_min){
    double avg=0;
    for(int i=0;i<BS_list.size();i++){
        for(int j=0;j<BS_list[i].UE_list.size();j++){
            for(int k=0;k<BS_list[i].UE_list[j].subbandSINR.size();k++){
                avg=avg+BS_list[i].UE_list[j].subbandSINR[k];
            }
            avg=avg/BS_list[i].UE_list[j].subbandSINR.size();
            BS_list[i].UE_list[j].avgSINR=avg;
            if(avg>SINR_max)
                SINR_max=avg;
            if(avg<SINR_min)
                SINR_min=avg;
            avg=0;
        }
    }
}

void showUEinfo(vector<baseStation> BS_list){
    for(int i=0;i<BS_list.size();i++){
        cout<<setw(6)<<"BS idx"<<"|"<<setw(8)<<"x"<<"|"<<setw(8)<<"y"<<endl;
        cout<<setw(6)<<i<<"|"<<setw(8)<<BS_list[i].x<<"|"<<setw(8)<<BS_list[i].y<<endl;
        cout<<setw(6)<<"UE idx"<<"|"<<setw(8)<<"x"<<"|"<<setw(8)<<"y"<<"|"<<setw(8)<<"avgSINR"<<"|"<<setw(5)<<"Pa"<<"|"<<setw(7)<<"RBnum"<<endl;
        for(int j=0;j<BS_list[i].UE_list.size();j++){
            cout<<setw(6)<<j<<"|"<<setw(8)<<BS_list[i].UE_list[j].x<<"|"<<setw(8)<<BS_list[i].UE_list[j].y<<"|"<<setw(8)<<BS_list[i].UE_list[j].avgSINR<<"|"<<setw(5)<<BS_list[i].UE_list[j].pa<<"|"<<setw(7)<<accumulate(BS_list[i].UE_list[j].subbandMask.begin(),BS_list[i].UE_list[j].subbandMask.end(),0)<<endl;
        }
        cout<<endl;
    }
}

void showUEallocRB(vector<baseStation> BS_list){
    cout<<"////////////////////////// UE RB used //////////////////////////"<<endl;
    for(int i=0;i<BS_list.size();i++){
        cout<<setw(6)<<"BS idx"<<":"<<setw(3)<<i<<endl;
        cout<<setw(6)<<"UE idx"<<"|"<<setw(9)<<"RB used"<<"(1: Used, 0: No Use)"<<endl;
        for(int j=0;j<BS_list[i].UE_list.size();j++){
            cout<<setw(6)<<j<<"|  ";
            for(int k=0;k<N_band;k++){
                cout<<BS_list[i].UE_list[j].subbandMask[k];
            }
            cout<<" ("<<setw(2)<<accumulate(BS_list[i].UE_list[j].subbandMask.begin(),BS_list[i].UE_list[j].subbandMask.end(),0)<<")";
            cout<<endl;
        }
        cout<<endl;
    }
}

void showBSinfo(vector<baseStation> BS_list){
    cout<<"/////////////////////// Base Station Info. //////////////////////"<<endl;
    for(int i=0;i<BS_list.size();i++){
        cout<<setw(12)<<"BS idx"<<":"<<setw(3)<<i<<endl;
        cout<<setw(12)<<"RB_Pa_origin"<<":  ";
        for(int j=0;j<N_band;j++){
            cout<<BS_list[i].RB_pa[j];
        }
        cout<<endl;
        cout<<setw(12)<<"RB_Pa_actual"<<":  ";
        for(int j=0;j<N_band;j++){
            if(BS_list[i].RB_pa_actual[j]==-1)
                cout<<"X";
            else
                cout<<BS_list[i].RB_pa_actual[j];
        }
        cout<<endl;
        cout<<setw(12)<<"RB_alloc_to_"<<":  ";
        for(int j=0;j<N_band;j++){
            if(BS_list[i].sub_alloc[j]==-1)
                cout<<"X";
            else
                cout<<BS_list[i].sub_alloc[j];
        }
        cout<<endl<<endl;
    }
}

void showUEsinr(vector<baseStation> BS_list){
    for(int i=0;i<BS_list.size();i++){
        cout<<"BS idx: "<<i<<endl;
        for(int j=0;j<BS_list[i].UE_list.size();j++){
            cout<<"UE idx: "<<j<<endl;
            cout<<"avg. SINR: "<<BS_list[i].UE_list[j].avgSINR<<endl;
            for(int k=0;k<BS_list[i].UE_list[j].subbandSINR.size();k++){
                cout<<"SINR of subband "<<k<<": "
                    <<BS_list[i].UE_list[j].subbandSINR[k]<<endl;
            }
        }
        cout<<endl;
    }
}

int main(int argc, char* argv[]){
    vector<baseStation> BS_list;
    
    // Read BS Input //
    ifstream infile;
    infile.open(argv[1],ifstream::in);
    
    string tmpline;
    // First line: BSnum //
    getline(infile,tmpline);
    int BSnum=atoi(tmpline.c_str());
    
    // Next BSnum lines: BS info. //
    vector<double> tmp;
    string field;
    
    for(int i=0;i<BSnum;i++){
        getline(infile,tmpline);
        stringstream element(tmpline);
        
        while(getline(element,field,',')){
            tmp.push_back(atof(field.c_str()));
        }
        
        BS_list.push_back(baseStation(tmp[0],tmp[1],tmp[2]));
        tmp.clear();
    }
    
    // Read Pa command //
    for(int i=0;i<BSnum;i++){
        getline(infile,tmpline);
        for(int j=0;j<tmpline.length();j++){
            //BS_list[i].sub_P[j]=BS_list[i].sub_P[j]+pa_level[tmpline[j]-'0'];
            BS_list[i].RB_pa.push_back(tmpline[j]-'0');
        }
    }
    
    // Read UE Input //
    int UEnum=0;
    for(int i=0;i<BSnum;i++){
        // Read each BS's UE num //
        getline(infile,tmpline);
        UEnum=atoi(tmpline.c_str());
        // Push UE info to UE_list //
        for(int j=0;j<UEnum;j++){
            getline(infile,tmpline);
            stringstream element(tmpline);
            
            while(getline(element,field,',')){
                tmp.push_back(atof(field.c_str()));
            }
            
            BS_list[i].UE_list.push_back(UE(BS_list[i].x+tmp[0],BS_list[i].y+tmp[1],tmp[2],tmp[3]));
            tmp.clear();
        }
    }
    // Read Input End //
    
    // Schedule //
    int UE_pa_num[8]={0,0,0,0,0,0,0,0};
    int RB_pa_num[8]={0,0,0,0,0,0,0,0};
    int RB_num=0;
    int maxRB_num_UE_get=0;
    int nowRB_num_UE_get=0;
    vector<int> sched_UE_list;
    
    // each iteration schedule one BS //
    for(int i=0;i<BSnum;i++){
        for(int j=0;j<BS_list[i].UE_list.size();j++){
            UE_pa_num[BS_list[i].UE_list[j].pa]=UE_pa_num[BS_list[i].UE_list[j].pa]+1;
        }
        for(int j=0;j<N_band;j++){
            RB_pa_num[BS_list[i].RB_pa[j]]=RB_pa_num[BS_list[i].RB_pa[j]]+1;
        }
        // Schedule Algorithm //
        // higher pa UE first //
        for(int j=7;j>-1;j--){
            // j: pa_level value
            // skip the pa_level w/o UE //
            if(UE_pa_num[j]==0)
                continue;
            
            // calc the max num of RB that a UE can get //
            for(int k=j;k<8;k++){
                RB_num=RB_num+RB_pa_num[k];
                RB_pa_num[k]=0;
            }
            maxRB_num_UE_get=RB_num/UE_pa_num[j];
            
            // fill now schedule list //
            for(int k=0;k<BS_list[i].UE_list.size();k++){
                if(BS_list[i].UE_list[k].pa==j)
                    sched_UE_list.push_back(k);
            }
            // alloc RB //
            for(int k=0;k<sched_UE_list.size()-1;k++){
                for(int l=0;l<N_band;l++){
                    if(nowRB_num_UE_get==maxRB_num_UE_get){
                        nowRB_num_UE_get=0;
                        break;
                    }
                    if(BS_list[i].RB_pa[l]<j)
                        continue;
                    if(BS_list[i].sub_alloc[l]!=-1)
                        continue;
                    nowRB_num_UE_get++;
                    // Specify BS RB to which UE //
                    BS_list[i].sub_alloc[l]=sched_UE_list[k];
                    // Modify Power level of BS RB //
                    BS_list[i].sub_P[l]=BS_list[i].sub_P[l]+pa_level[BS_list[i].UE_list[sched_UE_list[k]].pa];
                    // Record Actual Pa used of BS RB //
                    BS_list[i].RB_pa_actual[l]=BS_list[i].UE_list[sched_UE_list[k]].pa;
                    // Update UE RB used(mask) list //
                    BS_list[i].UE_list[sched_UE_list[k]].subbandMask[l]=1;
                }
            }
            // last one UE get bonus RB//
            for(int l=0;l<N_band;l++){
                if(BS_list[i].RB_pa[l]<j)
                    continue;
                if(BS_list[i].sub_alloc[l]!=-1)
                    continue;
                BS_list[i].sub_alloc[l]=sched_UE_list[sched_UE_list.size()-1];
                BS_list[i].sub_P[l]=BS_list[i].sub_P[l]+pa_level[BS_list[i].UE_list[sched_UE_list[sched_UE_list.size()-1]].pa];
                BS_list[i].RB_pa_actual[l]=BS_list[i].UE_list[sched_UE_list[sched_UE_list.size()-1]].pa;
                BS_list[i].UE_list[sched_UE_list[sched_UE_list.size()-1]].subbandMask[l]=1;
            }
            
            RB_num=0;
            sched_UE_list.clear();
        }
        // Schedule done //
        
        for(int j=0;j<8;j++){
            UE_pa_num[j]=0;
            RB_pa_num[j]=0;
        }
    }
    
    showUEinfo(BS_list);
    showUEallocRB(BS_list);
    showBSinfo(BS_list);
    return 0;
    
    // Calc Sub-band SINR of all UEs //
    double sinr_tmp=0;
    double i_tmp=0;
    for(int i=0;i<BSnum;i++){
        for(int j=0;j<BS_list[i].UE_list.size();j++){
            // i: BS idx
            // j: UE idx
            // k: Subband idx
            for(int k=0;k<N_band;k++){
                i_tmp=pow(10,N_0*(BW/N_band)/10);
                for(int l=0;l<BSnum;l++){
                    if(l==i)
                        sinr_tmp=getStrg(BS_list,i,j,k,l);
                    else
                        i_tmp=i_tmp+pow(10,getStrg(BS_list,i,j,k,l)/10);
                    
                }
                //cout<<"S: "<<sinr_tmp<<endl;
                //cout<<"I+N: "<<10*log10(i_tmp)<<endl;
                sinr_tmp=sinr_tmp-10*log10(i_tmp);
                //cout<<"SINR: "<<sinr_tmp<<endl<<endl;
                BS_list[i].UE_list[j].subbandSINR[k]=sinr_tmp;
            }
        }
    }
    
    // Calc avg. SINR //
    double SINR_max=0;
    double SINR_min=1000;
    calcavgSINR(BS_list,SINR_max,SINR_min);
    
    // Determine threshold //
    /*
    vector<int> threshold;
    vector<double> accuracy;
    bool isEdgebySINR;
    int min=int(SINR_min);
    int max=int(SINR_max);
    for(int i=min;i<max;i++){
        threshold.push_back(i);
        accuracy.push_back(0);
        for(int j=0;j<BSnum;j++){
            for(int k=0;k<UEnum;k++){
                if(BS_list[j].UE_list[k].avgSINR<i)
                    isEdgebySINR=1;
                else
                    isEdgebySINR=0;
                if(isEdgebySINR==BS_list[j].UE_list[k].isEdge)
                    accuracy[i-min]=accuracy[i-min]+1;
            }
        }
        accuracy[i-min]=accuracy[i-min]/(BSnum*UEnum);
    }
    
    double bestAccuracy=0;
    int bestidx=0;
    for(int i=0;i<accuracy.size();i++){
        if(accuracy[i]>bestAccuracy){
            bestAccuracy=accuracy[i];
            bestidx=i;
        }
    }
    cout<<"Best SINR threshold value:\t"<<threshold[bestidx]<<"dB"<<endl;
    cout<<"Classification accuracy:\t"<<accuracy[bestidx]*100.0<<"%\n"<<endl;
    //for(int i=0;i<threshold.size();i++)
    //    cout<<threshold[i]<<","<<accuracy[i]<<endl;
    */
    
    return 0;
}