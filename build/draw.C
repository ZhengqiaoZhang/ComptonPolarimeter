void draw(){

	TFile *s1 = new TFile("S1.root","OPEN");
	TFile *s3 = new TFile("S3.root","OPEN");

	TH1D *dis_s1 = (TH1D*)s1->Get("dis_S1");
	TH1D *dis_s3 = (TH1D*)s3->Get("dis_S3");

	TH1D *dis_ays = new TH1D("dis_ays","dis_ays",1000,-50,50);

	for(int i=1;i<1001;i++){
		if((dis_s3->GetBinContent(i)+dis_s1->GetBinContent(i))!=0)
		dis_ays->SetBinContent(i,(dis_s3->GetBinContent(i)-dis_s1->GetBinContent(i))/(dis_s3->GetBinContent(i)+dis_s1->GetBinContent(i)));
	}


	dis_ays->Draw();


}
