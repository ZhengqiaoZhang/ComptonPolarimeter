#include <fstream>
#include <iomanip>
#include <iostream>
#include "TMath.h"
#include <complex>   
#include "TF1.h"
using namespace std;

double func(double *x,double *par){
	cout.precision(17);
	cout.width(20);
	std::setprecision(13);
	using std::setw;

	ifstream file1;
	file1.open("eF_twiss_DB.dat");

	int i=0;
	string line;
	string line2;
	string line_tmp;
	bool firstline = true;

	string cc1,cc2;
	double d1,d2,d3,d4,d5,d6,d7,d8;
	string cc1_2,cc2_2;
	double d1_2,d2_2,d3_2,d4_2,d5_2,d6_2,d7_2,d8_2;

	double x_p=65.328;
	x_p = x[0];
	double betaX, alphaX, betaY, alphaY, gammaX, gammaY;
	double beta0x, alpha0x, gamma0x, Kx, Cx, SSx, C_dx, SS_dx;
	double beta0y, alpha0y, gamma0y, Ky, Cy, SSy, C_dy, SS_dy;

	while (!file1.eof()){
		if(firstline == true){getline(file1,line);firstline=false;line_tmp=line;}
		getline(file1,line2);
		stringstream ss(line_tmp);
		stringstream ss2(line2);
		i++;

		ss>>cc1;
		ss>>cc2;
		ss>>d1;
		ss>>d2;
		ss>>d3;
		ss>>d4;
		ss>>d5;
		ss>>d6;
		ss>>d7;

		ss2>>cc1_2;
		ss2>>cc2_2;
		ss2>>d1_2;
		ss2>>d2_2;
		ss2>>d3_2;
		ss2>>d4_2;
		ss2>>d5_2;
		ss2>>d6_2;
		ss2>>d7_2;

		//		cout<<line_tmp<<endl;
		line_tmp = line2;
		if((x_p>d1_2)||(x_p<=d1))continue;
		//		cout<<"dd="<<x_p<<" "<<d1_2<<" "<<d1<<endl;

		double s = x_p-d1;

		beta0x = d3;
		alpha0x = d4;
		beta0y = d5;
		alpha0y = d6;
		Kx = d7_2;
		Ky = -1.0*d7_2;
		//		cout<<beta0x<<" "<<alpha0x<<" "<<Kx<<endl;


		if(cc2_2=="Quadrupole"){
			if(Kx>0){
				gamma0x=(1.0+alpha0x*alpha0x)/beta0x;
				Cx = cos(sqrt(Kx)*s);
				SSx = (1.0/sqrt(Kx))*sin(sqrt(Kx)*s);
				C_dx = -1.0*sqrt(Kx)*sin(sqrt(Kx)*s);
				SS_dx = (1.0/sqrt(Kx))*sqrt(Kx)*cos(sqrt(Kx)*s);
				betaX = Cx*Cx*beta0x-2.0*Cx*SSx*alpha0x+SSx*SSx*gamma0x;
				alphaX = -1.0*Cx*C_dx*beta0x+(SSx*C_dx+SS_dx*Cx)*alpha0x-SSx*SS_dx*gamma0x;
				gammaX = (1.0+alphaX*alphaX)/betaX;


			}
			if(Kx<0){
				gamma0x=(1.0+alpha0x*alpha0x)/beta0x;
				Cx = cosh(sqrt(-1.0*Kx)*s);
				SSx = sinh(sqrt(-1.0*Kx)*s)*1.0/sqrt(-1.0*Kx);
				C_dx = sqrt(-1.0*Kx)*sinh(sqrt(-1.0*Kx)*s);
				SS_dx = cosh(sqrt(-1.0*Kx)*s);
				betaX = Cx*Cx*beta0x-2.0*Cx*SSx*alpha0x+SSx*SSx*gamma0x;
				alphaX = -1.0*Cx*C_dx*beta0x+(SSx*C_dx+SS_dx*Cx)*alpha0x-SSx*SS_dx*gamma0x;
				gammaX = (1.0+alphaX*alphaX)/betaX;


			}

			if(Ky>0){
				gamma0y=(1.0+alpha0y*alpha0y)/beta0y;
				Cy = cos(sqrt(Ky)*s);
				SSy = (1.0/sqrt(Ky))*sin(sqrt(Ky)*s);
				C_dy = -1.0*sqrt(Ky)*sin(sqrt(Ky)*s);
				SS_dy = (1.0/sqrt(Ky))*sqrt(Ky)*cos(sqrt(Ky)*s);
				betaY = Cy*Cy*beta0y-2.0*Cy*SSy*alpha0y+SSy*SSy*gamma0y;
				alphaY = -1.0*Cy*C_dy*beta0y+(SSy*C_dy+SS_dy*Cy)*alpha0y-SSy*SS_dy*gamma0y;
				gammaY = (1.0+alphaY*alphaY)/betaY;

			}
			if(Ky<0){
				gamma0y=(1.0+alpha0y*alpha0y)/beta0y;
				Cy = cosh(sqrt(-1.0*Ky)*s);
				SSy = sinh(sqrt(-1.0*Ky)*s)*1.0/sqrt(-1.0*Ky);
				C_dy = sqrt(-1.0*Ky)*sinh(sqrt(-1.0*Ky)*s);
				SS_dy = cosh(sqrt(-1.0*Ky)*s);
				betaY = Cy*Cy*beta0y-2.0*Cy*SSy*alpha0y+SSy*SSy*gamma0y;
				alphaY = -1.0*Cy*C_dy*beta0y+(SSy*C_dy+SS_dy*Cy)*alpha0y-SSy*SS_dy*gamma0y;
				gammaY = (1.0+alphaY*alphaY)/betaY;

			}

		}

		if(cc2_2!="Quadrupole"){
			gamma0x=(1.0+alpha0x*alpha0x)/beta0x;
			betaX = beta0x-2.0*alpha0x*s+gamma0x*s*s;
			alphaX = alpha0x - gamma0x*s;
			gammaX =gamma0x;

			gamma0y=(1.0+alpha0y*alpha0y)/beta0y;
			betaY = beta0y-2.0*alpha0y*s+gamma0y*s*s;
			alphaY = alpha0y - gamma0y*s;
			gammaY = gamma0y;

		}






		//		line_tmp = line2;

	}
	if(par[0]==1.0)return betaX;
	if(par[0]==2.0)return betaY;


}

void cal_twiss_eF(){
	gStyle->SetOptStat(0);
	gStyle->SetOptDate(0);
	gStyle->SetOptTitle(0);
	cout.precision(17);
	cout.width(20);
	std::setprecision(13);
	using std::setw;
TCanvas *canvas = new TCanvas("c1","c1",900,450);
canvas->cd();
	//double test1=func(65.328);
	TF1 *f1 = new TF1("myfunc",func,13.35,66.0,1);
	f1->SetParameter(0,1.0);
	f1->SetLineColor(kRed);
	f1->GetXaxis()->SetTitle("s (m)");
	f1->Draw();
	TF1 *f2 = new TF1("myfunc2",func,13.35,66.0,1);
	f2->SetParameter(0,2.0);
	f2->SetLineColor(kBlue);
	f2->Draw("same");
	f2->GetXaxis()->SetTitle("s (m)");
	TLegend *l1 = new TLegend(0.1,0.80,0.15,0.99);
	l1->AddEntry(f1,"#beta_{x}","l");
	l1->AddEntry(f2,"#beta_{y}","l");
	l1->Draw("same");
}


