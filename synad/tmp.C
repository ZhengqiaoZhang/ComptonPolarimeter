void tmp(){
	TFile *file = new TFile("plots_onlyQ_beamtail.root", "OPEN");  // 打开文件以更新模式
		TFile *file1= new TFile("plots_onlyQ_beamtail_new.root","recreate");
TH1D *h = (TH1D*)file->Get("e");  // old_name 是旧的直方图名称
h->SetName("e_tail");  // new_name 是你想要的新名称
file1->cd();		       //
h->Write();  // 这将用新的名称保存直方图
file->Close();
}
