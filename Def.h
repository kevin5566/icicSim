#include<vector>
using namespace std;

double  c=299792458;        // Light Speed
double  carrierFreq=1.9*1000000000;    // (Hz)
double  d_0=100.0;          // ref. Distance(m)
int     n=4;                // Path Loss Exponent
double  N_0=-174.0;         // Noise Density(dBm/Hz)
double  BW=50*1000000;      // Total BW(Hz)
int     N_band=50;          // Subband num

double  pa_level[8]={-6,-4.77,-3,-1.77,0,1,2,3};

struct UE{
    double x;
    double y;
    bool   isEdge;
    double avgSINR;
    int pa;
    vector<double> subbandSINR;
    vector<bool> subbandMask;   //0: no use; 1: used
    UE(double a, double b, bool c, int d){
        x=a;
        y=b;
        isEdge=c;
        pa=d;
        avgSINR=1;
        for(int i=0;i<N_band;i++){
            subbandSINR.push_back(0);
            subbandMask.push_back(0);
        }
    }
};

struct baseStation{
    double x;
    double y;
    double power;
    int    subbandnum;
    vector<double> sub_P;
    vector<int> sub_alloc;  //-1: no use
    vector<int> RB_pa;
    vector<int> RB_pa_actual; //-1: no use
    vector<UE> UE_list;
    baseStation(double a, double b, double c){
        x=a;
        y=b;
        power=c;
        subbandnum=N_band;
        for(int i=0;i<subbandnum;i++){
            sub_P.push_back(power);
            sub_alloc.push_back(-1);
            RB_pa_actual.push_back(-1);
        }
    }
};

