#include <fstream>
#include <iomanip>
#include <iostream>
#include "TMath.h"
#include <complex>   
#include "TF1.h"
using namespace std;

double BX(double x){
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
	x_p = x;
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
	}
	return betaX;


}
double BY(double x){
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
	x_p = x;
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

		//              cout<<line_tmp<<endl;
		line_tmp = line2;
		if((x_p>d1_2)||(x_p<=d1))continue;
		//              cout<<"dd="<<x_p<<" "<<d1_2<<" "<<d1<<endl;

		double s = x_p-d1;

		beta0x = d3;
		alpha0x = d4;
		beta0y = d5;
		alpha0y = d6;
		Kx = d7_2;
		Ky = -1.0*d7_2;
		//              cout<<beta0x<<" "<<alpha0x<<" "<<Kx<<endl;


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
	}
	return betaY;


}

double fBX(double *x,double *par){
double x_p = (x[0]+1333.4)*0.01;
	return BX(x_p);
}
double fBY(double *x,double *par){
double x_p = (x[0]+1333.4)*0.01;
        return BY(x_p);
}






void cal_eff_DB(){
	gStyle->SetOptStat(0);
	gStyle->SetOptDate(0);
	gStyle->SetOptTitle(0);
	TFile *store = new TFile("result_DB1_S3.root","OPEN");
	TH2D *xz = (TH2D*)store->Get("x_z");
	TFile *store_S1 = new TFile("result_DB1_S1.root","OPEN");
	TH2D *xz_S1 = (TH2D*)store_S1->Get("x_z");
	double eff_S1[199], eff_S3[199],z_gap[199];
	xz->RebinX(10);
	xz_S1->RebinX(10);

/*	 TF1 *f1 = new TF1("myfunc",fBX,0,2000,1);
        f1->SetLineColor(kRed);
        f1->GetXaxis()->SetTitle("z (cm)");
	f1->GetYaxis()->SetTitle("beta function (m)");
        f1->Draw();
        TF1 *f2 = new TF1("myfunc2",fBY,0,2000,1);
        f2->SetLineColor(kBlue);
        f2->Draw("same");
        f2->GetXaxis()->SetTitle("z (cm)");
        TLegend *l1 = new TLegend(0.1,0.80,0.15,0.99);
        l1->AddEntry(f1,"#beta_{x}","l");
        l1->AddEntry(f2,"#beta_{y}","l");
        l1->Draw("same");*/

	for(int i=1;i<200;i++){
		double zvalue = i*10.0;
		double xvalue = 0;
		double binwidth = 30.0/2000.0;
		double x_accept = xvalue - sqrt(BX(13.334+zvalue*0.01)*24.0*1.0e-9)*13.0*100;
		int Xbin = xz->GetYaxis()->FindFixBin(x_accept);//floor((x_accept)/binwidth);
		int eAccept = 0;
		for(int j=1;j<Xbin+1;j++){
			eAccept=eAccept+xz->GetBinContent(i,j);
		}
		int totale = 0;
		for(int j=1;j<2001;j++){
			totale=totale+xz->GetBinContent(i,j);
		}
				cout<<zvalue<<" "<<eAccept<<" "<<totale<<" "<<x_accept<<" "<<Xbin<<endl;
		z_gap[i-1]=zvalue;
		eff_S3[i-1]=eAccept*1.0/(totale*1.0);
	}

	for(int i=1;i<200;i++){
		double zvalue = i*10.0;
		double xvalue = 0;
		double binwidth = 30.0/2000.0;
		double x_accept = xvalue - sqrt(BX(13.334+zvalue*0.01)*24.0*1.0e-9)*13.0*100;
		int Xbin = xz->GetYaxis()->FindFixBin(x_accept);
		int eAccept = 0;
		for(int j=1;j<Xbin+1;j++){
			eAccept=eAccept+xz_S1->GetBinContent(i,j);
		}
		int totale = 0;
		for(int j=1;j<2001;j++){
			totale=totale+xz_S1->GetBinContent(i,j);
		}
		//		cout<<zvalue<<" "<<eAccept<<" "<<totale<<" "<<BX(13.334+zvalue*0.01)<<" "<<BY(13.334+zvalue*0.01)<<endl;
		eff_S1[i-1]=eAccept*1.0/(totale*1.0);
		//		cout<<eff_S3[i-1]<<" "<<eff_S1[i-1]<<endl;
	}

	TGraph* g_S1 = new TGraph(199,z_gap,eff_S1);
	TGraph* g_S3 = new TGraph(199,z_gap,eff_S3);
	g_S1->GetXaxis()->SetTitle("z (cm)");
	g_S3->GetXaxis()->SetTitle("z (cm)");
	g_S3->GetYaxis()->SetTitle("Maximum Acceptance");
	g_S1->GetYaxis()->SetTitle("Maximum Acceptance");


	// TCanvas *c1 = new TCanvas("c1","A Simple Graph Example",1500,600);
	g_S3->SetLineColor(kRed);
	g_S3->SetLineWidth(4);
	//	g_S3->Draw("AL");
	g_S1->SetLineColor(kBlue);
	g_S1->SetLineWidth(4);
	g_S1->Draw("AL");
	g_S3->Draw("SAMEL");
	TLegend *l1 = new TLegend(0.75,0.25,1,0.05);
	l1->AddEntry(g_S3,"Spins aligned","l");
	l1->AddEntry(g_S1,"Anti-aligned","l");
	l1->Draw("same");
	// eff->Draw();

}



