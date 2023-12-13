#include <fstream>
#include <iostream>
using namespace std;
const double millibarn_per_m2 = 1.e+31;   // conversion of m-barn <-> m^2
const double millibarn_to_barn = 1.e-3;
const double re = 2.817940285e-15;  // classical electron radius (m)
const double me = 0.510998902e-3;   // electron mass (GeV)
const double pi = 3.14159265359;
const double rotation = -3.14389 ; //rotation due to the beamline (Q6EF_6)
const double max_cross_spin1 = 0.00014745;//0.02*50000/eventno. multiplied by 0.1 for 500000
const double max_cross_spin3 = 0.00002;//0.02*50000/eventno.  1.74149e-05  9.94719e-05
double pL_x = -59008.307; //position X for laser IP  (unit cm)
double pL_y = 0;
double pL_z = 7484.6637;  
const double sigmaZ = 0.01;;
const double flaserXY = 0.0001;


//18GeV
const double fEmmX = 2.4e-8;
const double fEmmY = 2.0e-9;
const double AlphaX = -2.3997154;
const double AlphaY = 0.036733185;
const double BetaX = 2.6530932;
const double BetaY = 6.3699093;

void geat4events_IP6_eF_18GeV(){
	double com1 = 0;
	double com3 = 0;
	gRandom = new TRandom3(0); 
	TRandom3 *rand1 = new TRandom3(0);
	TRandom3 *rand2 = new TRandom3(0);
	ofstream outf;
	//outf.open("IP6_5GeV_S3_Ver8.0_L.dat");
	outf.open("18GeV_S3_Ver8.0_L.dat");

	ofstream outf1;
        //outf1.open("IP6_18GeV_S1_Ver8.0_L.dat");
        outf1.open("18GeV_S1_Ver8.0_L.dat");

	double ee = 18;
	double eph = 2.3305e-9;
	double x, theta_0,theta_g,y,theta_e,theta_xz;
	const double gamma = ee/me;        // defined as energy_electron/mass_electron
	double a = 1./(1. + 4.*eph*gamma/me);
	double kprimemax = 4.*a*eph*ee*ee/(me*me);
	//double pL_x = 25.6875;
	//double pL_z = -1027.5;//vertex of the compton scattering 

	TH1D *theat_g_dis = new TH1D("theta_1","theta_1",500,0,1);
	TH1D *theat_g_after = new TH1D("theta_after","theta_after",500,0,1);

	TH1D *e_electron_S1 = new TH1D("energy_e_S1","energy_e_S1",2000,0,20);
	TH1D *e_photon_S1 = new TH1D("energy_g_S1","energy_g_S1",2000,0,20);
	TH1D *e_electron_S3 = new TH1D("energy_e_S3","energy_e_S3",2000,0,20);
        TH1D *e_photon_S3 = new TH1D("energy_g_S3","energy_g_S3",2000,0,20);


	TFile *store = new TFile("18GeV_QA.root","recreate");

	TVector3 origin_e; //photon momentum from comrad.f
	TVector3 origin_g; //electronn momentum from comrad.f
	TVector3 origin_g_after;




	ifstream file1,file2;

	//file1.open("events_Z_1.dat"); ///z
	file1.open("../events_18GeV_large.dat");

	int eventno_S1 = 0;
	int eventno_S3 = 0;
	double px_g,py_g,pz_g,energy_ph,px_e,py_e,pz_e,energy_e,theta_xz_g,theta_xz_e;
	double px_g_r,py_g_r,pz_g_r,px_e_r,py_e_r,pz_e_r,px_g_ra,py_g_ra,pz_g_ra;//momomtum after rotation
	double px_g2,py_g2,pz_g2,energy_ph2,theta_xz_g2;
	double px_g_r2=0;
	double py_g_r2=0;
	double pz_g_r2=0;

	double d1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12;
	double dd1,dd2,dd3,dd4,dd5,dd6,dd7,dd8,dd9,dd10,dd11,dd12,dd13,dd14,dd15,dd16;


	string line;
	string line2;
	int i=0;
	int j=0;
	int count_twogamma=0;
	double csection = 0;

	double probSample,probRandom,SmearX,SmearXp,SmearY,SmearYp;

	while (!file1.eof()){
		getline(file1,line);
		stringstream ss(line);
		ss>>d1;
		ss>>d2;
		ss>>d3;
		ss>>d4;
		ss>>d5;
		ss>>d6;
		ss>>d7;
		ss>>d8;
		ss>>d9;
		ss>>d10;
		ss>>d11;
		ss>>d12;

		i++;
		if(i>3){
			if(i%5000==0)cout<<i<<endl;

			px_g = d1;
			py_g = d2;
			pz_g = d3;
			energy_ph = d4;

			origin_g.SetXYZ(d1,d2,d3);
			origin_g_after.SetXYZ(d1,d2,d3);


			//theat_g_dis->Fill(1000*atan(sqrt(d1*d1+d2*d2)/d3));




			px_e = d9;
			py_e = d10;
			pz_e = d11;
			energy_e = d12;

			origin_e.SetXYZ(d9,d10,d11);

			double randGausZ = rand2->Gaus(0,sigmaZ);
			//cout<<randGausX<<" "<<randGausY<<" "<<randGausZ<<endl;


			if(d5-d6+d7-d8>com1)com1=d5-d6+d7-d8;
			if(d5+d6+d7+d8>com3)com3=d5+d6+d7+d8;	
			//weights yield W1, W3  the order1􏰂 and order3􏰂 unpolarized cross sections, respectively. The sums of the W2 and W4 weights yield the order2 and order3 polarized cross sections


			double randN = rand1->Uniform(0,1)*max_cross_spin3;

//				theat_g_dis->Fill(1000*atan(sqrt(d1*d1+d2*d2)/d3));

				do{
					double x0 = rand2->Gaus(0,sqrt(fEmmX));
					double xp0 = rand2->Gaus(0,sqrt(fEmmX));
					SmearX = sqrt(BetaX)*x0*100.0;
					SmearXp = - AlphaX*sqrt(1/BetaX)*x0 + xp0/sqrt(BetaX);
					probSample = TMath::Gaus(SmearX,0,flaserXY,1);
					double xl = rand2->Gaus(0,flaserXY);
					probRandom = TMath::Gaus(xl,0,flaserXY,1);
				}while(probSample < probRandom);


				do{
					double y0 = rand2->Gaus(0,sqrt(fEmmY));
					double yp0 = rand2->Gaus(0,sqrt(fEmmY));
					SmearY = sqrt(BetaY)*y0*100.0;
					SmearYp = - AlphaY*sqrt(1/BetaY)*y0 + yp0/sqrt(BetaY);
					probSample = TMath::Gaus(SmearY,0,flaserXY,1);
					double yl = rand2->Gaus(0,flaserXY);
					probRandom = TMath::Gaus(yl,0,flaserXY,1);
				}while(probSample < probRandom);


				origin_g.RotateY(rotation-SmearXp);   
				origin_g.RotateX(SmearYp);

				px_g_r=origin_g.X(); //px of photon after rotation
				py_g_r=origin_g.Y(); //py of photon after rotation
				pz_g_r=origin_g.Z(); //pz of photon after rotation

				origin_g_after.RotateY(SmearXp);
				origin_g_after.RotateX(SmearYp);
				

				//origin_g_after.RotateY(-rotation);
				px_g_ra=origin_g_after.X(); //px of photon after rotation
                                py_g_ra=origin_g_after.Y(); //py of photon after rotation
                                pz_g_ra=origin_g_after.Z(); //pz of photon after rotation

				theat_g_after->Fill(1000*atan(sqrt(px_g_ra*px_g_ra+py_g_ra*py_g_ra)/pz_g_ra));
			//cout<<1000*atan(sqrt(px_g_ra*px_g_ra+py_g_ra*py_g_ra)/pz_g_ra)<<endl;

				origin_e.RotateY(rotation-SmearXp);
				origin_e.RotateX(SmearYp);

				px_e_r=origin_e.X();
				py_e_r=origin_e.Y();
				pz_e_r=origin_e.Z();

			if(d5+d6+d7+d8>randN){
				eventno_S3++;

				outf<<"EVENT:  "<<eventno_S3<<" "<<"2"<<" "<<std::setprecision(10)<<pL_x+SmearX<<"  "<<pL_y+SmearY<<"  "<<pL_z + randGausZ <<endl;
				outf<<"TRACK:  "<<"22"<<" "<<px_g_r<<"   "<<py_g_r<<"   "<<pz_g_r<<endl;
				outf<<"TRACK:  "<<"11"<<" "<<px_e_r<<"   "<<py_e_r<<"   "<<pz_e_r<<endl;
				outf<<endl;
				e_electron_S3->Fill(sqrt(px_e_r*px_e_r+py_e_r*py_e_r+pz_e_r*pz_e_r));
				e_photon_S3->Fill(sqrt(px_g_r*px_g_r+py_g_r*py_g_r+pz_g_r*pz_g_r));
				

			}

			 if(d5-d6+d7-d8>randN){
				eventno_S1++;
                                outf1<<"EVENT:  "<<eventno_S1<<" "<<"2"<<" "<<std::setprecision(10)<<pL_x+SmearX<<"  "<<pL_y+SmearY<<"  "<<pL_z + randGausZ <<endl;
                                outf1<<"TRACK:  "<<"22"<<" "<<px_g_r<<"   "<<py_g_r<<"   "<<pz_g_r<<endl;
                                outf1<<"TRACK:  "<<"11"<<" "<<px_e_r<<"   "<<py_e_r<<"   "<<pz_e_r<<endl;
                                outf1<<endl;
				 e_electron_S1->Fill(sqrt(px_e_r*px_e_r+py_e_r*py_e_r+pz_e_r*pz_e_r));
                                e_photon_S1->Fill(sqrt(px_g_r*px_g_r+py_g_r*py_g_r+pz_g_r*pz_g_r));

                        }


				//eventno++;
				//if(eventno%10000==0)cout<<"event ="<<eventno<<endl;
		}
		if(i>5000002)break;
		//		if(i>250010)break;




	}

	theat_g_dis->Draw();
	theat_g_after->SetLineColor(kRed);
	theat_g_after->Draw("same");
	store->cd();
	theat_g_dis->Write();
	theat_g_after->Write();
	e_electron_S1->Write();
	e_photon_S1->Write();
	e_electron_S3->Write();
        e_photon_S3->Write();

	cout<<"max_spin1(d5-d6+d7-d8_)="<<com1<<endl;
	cout<<"max_spin3(d5+d6+d7+d8_)="<<com3<<endl;



}
