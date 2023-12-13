#include <fstream>
#include <iomanip>
#include <iostream>
#include "TMath.h"
#include <complex>
#include "TF1.h"
using namespace std;

void transForm(){

	  ifstream file1;
        file1.open("ir6-survey-doug.tfs");

	 ofstream outf;
        outf.open("test.dat");

	        int i=0;
        string line;
        string line2;
        string line_tmp;
        bool firstline = true;

        string cc1,cc2,cc3;
        double d1,d2,d3,d4,d5,d6,d7,d8,d9;

	  outf<<"Name                 Center X                  Center Y                  Center Z           rin(z-in) rin(z-out)  dout         Length                      Azimuth                     B Field                    B Gradient"<<endl;
                         outf<<"##                      [m]                       [m]                       [m]                [m]     [m]     [m]              [m]                         [rad]                        [T]                         [T/m]"<<endl;





	 while (!file1.eof()){

		 getline(file1,line);
		 i++;
		 if(i>9){

			  stringstream ss(line);
			   ss>>cc1;
                ss>>cc2;
		 ss>>d1;
                ss>>d2;
                ss>>d3;
                ss>>d4;
                ss>>d5;
                ss>>d6;
                ss>>d7;
		ss>>d8;
		ss>>d9;


			if(cc1=="\"O_CRAB_EF_6\""||cc2!="\"DRIFT\""){
			if(cc1.substr(0, 2) == "\"Q")cc3="QD";
			else if(cc1.substr(0, 2) == "\"D")cc3="DB";
			else if(cc1=="\"O_CRAB_EF_6\"")cc3="CRAB";
			else if(cc1=="\"HSOL20_6\"")cc3="SOLENOID";
			else if(cc1.substr(0,5) == "\"HQLS")cc3="QD";
			else cc3="NONE";
			outf<<cc3<<"  "<<cc1<<" 	"<<std::setprecision(10)<<d4-d2*0.5*sin(d7)<<"			0		"<<d6-d2*0.5*cos(d7)<<"		"<<"0.06    0.06    0.5"<<"	"<<d2<<"	"<<d7<<"	"<<"0  0"<<endl;
			}

		 }

	 }

}
