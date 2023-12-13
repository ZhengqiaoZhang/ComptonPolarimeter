void calAsy_photon(){

	 TH1D *xdis = new TH1D("xdis","xdis",500,-10.0,10.0);


	TFile *f3 = new TFile("10GeV_photon_S3.root","OPEN");
	TFile *f1 = new TFile("10GeV_photon_S1.root","OPEN");

	TH1D *dis_S1 = (TH1D*)f1->Get("xdis_photon");
	TH1D *dis_S3 = (TH1D*)f3->Get("xdis_photon");

	for(int i=1;i<501;i++){

		if(dis_S1->GetBinContent(i)==0)continue;
		if(dis_S3->GetBinContent(i)==0)continue;



		xdis->SetBinContent(i,(-1.0*dis_S1->GetBinContent(i)+dis_S3->GetBinContent(i))/(dis_S1->GetBinContent(i)+dis_S3->GetBinContent(i)));

	}

	for(int i=1;i<501;i++){
		cout<<i<<" "<<xdis->GetBinContent(i)<<endl;
	}

	xdis->Draw();

}
