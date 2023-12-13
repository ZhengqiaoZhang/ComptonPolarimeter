#include <fstream>
#include <iomanip>
#include <iostream>
#include "TMath.h"
#include <complex>
#include "TF1.h"
using namespace std;

void calAsy(){

 ifstream file1;
        file1.open("IP6_10GeV_S3_Ver8.0_L.dat");
ifstream file2;
        file2.open("IP6_10GeV_S1_Ver8.0_L.dat");


TFile *store = new TFile("result.root","recreate");

TH1D *dis_E_S3 = new TH1D("E_S3","E_S3",200,0,10);
TH1D *dis_E_S1 = new TH1D("E_S1","E_S1",200,0,10);

        string cc1,cc2,cc3;
        double d1,d2,d3,d4,d5,d6,d7,d8,d9;

  string line;

int i=0;


 while (!file1.eof()){

                 getline(file1,line);

  stringstream ss(line);
                           ss>>cc1;
		ss>>d1;
                ss>>d2;
                ss>>d3;
                ss>>d4;
                ss>>d5;

if(cc1=="TRACK:"&&d1==22){
dis_E_S3->Fill(sqrt(d2*d2+d3*d3+d4*d4));
i++;
if(i%100000==0)cout<<i<<endl;

}

if(i>50000000)break;

}

 while (!file2.eof()){

                 getline(file2,line);

  stringstream ss(line);
                           ss>>cc1;
                ss>>d1;
                ss>>d2;
                ss>>d3;
                ss>>d4;
                ss>>d5;

if(cc1=="TRACK:"&&d1==22){
dis_E_S1->Fill(sqrt(d2*d2+d3*d3+d4*d4));
i++;
if(i%100000==0)cout<<i<<endl;

}

//if(i>5000000)break;

}

dis_E_S3->Draw();

store->cd();
dis_E_S3->Write();
dis_E_S1->Write();

}
