void produceSR(){

	ifstream file1;
	file1.open("5M_ALL.dat");

	ofstream outf;
	outf.open("SR_all_nobeamtail.dat");

	TH1D *energy = new TH1D("energy","energy",1000,0,1000);

	TFile *file = new TFile("plots_all.dat","recreate");


	// Create a new .root file
	TFile *f = new TFile("weight_all_beamtail.root", "RECREATE");

	// Create a new TTree
	TTree *tree = new TTree("Weight", "weight");

	// Create variables to hold data
	float w1,e1;

	// Create branches in the tree
	tree->Branch("flux", &w1, "flux/F");
	tree->Branch("energy", &e1, "energy/F");

	double tFlux = 8.83e18;

	double criticalE,averE;
	long double totalE = 0;



	int i=0;
	string line;
	string cc1,cc2;
	double d1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12,d13,d14;
	double total_e = 0;
	double photon_n = 0;
	double power= 0;
	int eventNO = 1;

	TH1D *xdis = new TH1D ("xdis","xdis",3000,-600,-580);
	TH1D *xdir = new TH1D("xdir","xdir",5000,-1,1);
	TH1D *ydir = new TH1D("ydir","ydir",5000,-0.1,0.1);
	TH1D *X = new TH1D("X","X",1000,45,50);
	TH2D *X_DIR = new TH2D("X_DIR","X_DIR",1000,-5,5,500,-0.05,0.05);

	TH1D *en = new TH1D("e_tail","e_tail",4000,0,2000);

	while (!file1.eof()){
		getline(file1,line);
		if(line=="")continue;
		stringstream ss(line);
		i++;
		if(i==1)continue;

		//		if(eventNO>500000)break;

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
		ss>>d13;
		ss>>d14;

		if(d12!=0)energy->Fill(d12*0.001,d13*1.0e-8);


		outf<<"EVENT:  "<<eventNO<<" 1 "<<d1<<" "<<d2<<" "<<d3<<endl;
		outf<<"TRACK:  22 "<<d12*1.0e-9*d6<<" "<<d12*1.0e-9*d7<<" "<<d12*1.0e-9*d8<<endl;
		outf<<endl;
		eventNO++;

		w1=d13;
		e1=d12;
		tree->Fill();


		xdis->Fill(d1*0.01);
		xdir->Fill(d6);
		ydir->Fill(d7);
		X->Fill(d1);
		X_DIR->Fill(d1,d6,d14);

		en->Fill(d12*0.001,d13);

		total_e=total_e+d11*d12*1.0e-9;
		photon_n=photon_n+d13;
		total_e=total_e+d12*d13*d11*1.0e-9;
		power=power+d14*d11;

	}
	cout<<total_e*1.602176487e-10<<endl;
	X_DIR->Scale(tFlux/photon_n);
	cout<<"scale="<<tFlux/photon_n<<endl;
	cout<<"power="<<power*tFlux/photon_n<<endl;
	cout<<photon_n<<endl;

	f->cd();
	tree->Write();

file->cd();
en->Write();
X_DIR->Write();


	//energy->Draw();
//	xdir->Draw();
	X_DIR->Draw("colz");
}
