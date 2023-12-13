#include <TFile.h>
#include <TH1D.h>
#include <TMath.h>
#include <iostream>

const int reBin = 5;

void calAsy() {

	gStyle->SetOptStat(0);
	gStyle->SetOptDate(0);
	gStyle->SetOptTitle(0);

	TFile *result = new TFile("18GeV_result.root", "RECREATE");

	// Histograms initialization
	TH1D *xAsy_SQ6 = new TH1D("xAsy_SQ6", "xAsy_SQ6", 1000/reBin, -20.0, 20.0);
	TH1D *xAsy_SQ7 = new TH1D("xAsy_SQ7", "xAsy_SQ7", 1000/reBin, -20.0, 20.0);
	TH1D *EAsy_ori = new TH1D("EAsy_ori", "EAsy_ori", 2000/reBin, 0, 20);
	TH1D *EAsy_1 = new TH1D("EAsy_1", "EAsy_1", 2000/reBin, 0, 20);
	TH1D *EAsy_2 = new TH1D("EAsy_2", "EAsy_2", 2000/reBin, 0, 20);
	TH1D *EAsy_all = new TH1D("EAsy_all", "EAsy_all", 2000/reBin, 0, 20);

	// Opening files
	TFile *f3 = new TFile("18GeV_S3.root", "OPEN");
	TFile *f1 = new TFile("18GeV_S1.root", "OPEN");

	// Getting histograms from files
	TH1D *dis_S1_SQ6 = (TH1D*)f1->Get("xdis_after_SQ6");
	TH1D *dis_S3_SQ6 = (TH1D*)f3->Get("xdis_after_SQ6");

	TH1D *dis_S1_SQ7 = (TH1D*)f1->Get("xdis_after_SQ7");
	TH1D *dis_S3_SQ7 = (TH1D*)f3->Get("xdis_after_SQ7");

	TH1D *dis_S1_E_all = (TH1D*)f1->Get("energy_all");
	TH1D *dis_S3_E_all = (TH1D*)f3->Get("energy_all");

	TH1D *dis_S1_E_1 = (TH1D*)f1->Get("energy_acpt_1");
	TH1D *dis_S3_E_1 = (TH1D*)f3->Get("energy_acpt_1");

	TH1D *dis_S1_E_2 = (TH1D*)f1->Get("energy_acpt_2");
	TH1D *dis_S3_E_2 = (TH1D*)f3->Get("energy_acpt_2");


	dis_S1_SQ6->RebinX(reBin);
	dis_S3_SQ6->RebinX(reBin);
	dis_S1_SQ7->RebinX(reBin);
	dis_S3_SQ7->RebinX(reBin);
	dis_S1_E_all->RebinX(reBin);
	dis_S3_E_all->RebinX(reBin);
	dis_S1_E_1->RebinX(reBin);
	dis_S3_E_1->RebinX(reBin);
	dis_S1_E_2->RebinX(reBin);
	dis_S3_E_2->RebinX(reBin);


	// Calculation of asymmetry and errors
	for (int i = 1; i <= 1000/reBin; i++) {
		// xAsy_SQ6
		if(dis_S1_SQ6->GetBinContent(i) != 0 && dis_S3_SQ6->GetBinContent(i) != 0) {
			double N1 = dis_S1_SQ6->GetBinContent(i);
			double N2 = dis_S3_SQ6->GetBinContent(i);
			double asymmetry = (-1.0 * N1 + N2) / (N1 + N2);
			double error = sqrt((4 * N1 * N2) / pow(N1 + N2, 3));
			xAsy_SQ6->SetBinContent(i, asymmetry);
			xAsy_SQ6->SetBinError(i, error);
		}

		// xAsy_SQ7
		if(dis_S1_SQ7->GetBinContent(i) != 0 && dis_S3_SQ7->GetBinContent(i) != 0) {
			double N1 = dis_S1_SQ7->GetBinContent(i);
			double N2 = dis_S3_SQ7->GetBinContent(i);
			double asymmetry = (-1.0 * N1 + N2) / (N1 + N2);
			double error = sqrt((4 * N1 * N2) / pow(N1 + N2, 3));
			xAsy_SQ7->SetBinContent(i, asymmetry);
			xAsy_SQ7->SetBinError(i, error);
		}
	}

	for (int i = 1; i <= 2000/reBin; i++) {
		// EAsy_ori
		if(dis_S1_E_all->GetBinContent(i) != 0 && dis_S3_E_all->GetBinContent(i) != 0) {
			double N1 = dis_S1_E_all->GetBinContent(i);
			double N2 = dis_S3_E_all->GetBinContent(i);
			double asymmetry = (-1.0 * N1 + N2) / (N1 + N2);
			double error = sqrt((4 * N1 * N2) / pow(N1 + N2, 3));
			EAsy_ori->SetBinContent(i, asymmetry);
			EAsy_ori->SetBinError(i, error);
		}

		// EAsy_all
		double N1 = dis_S1_E_1->GetBinContent(i) + dis_S1_E_2->GetBinContent(i);
		double N2 = dis_S3_E_1->GetBinContent(i) + dis_S3_E_2->GetBinContent(i);
		if(N1 != 0 && N2 != 0) {
			double asymmetry = (-1.0 * N1 + N2) / (N1 + N2);
			double error = sqrt((4 * N1 * N2) / pow(N1 + N2, 3));
			EAsy_all->SetBinContent(i, asymmetry);
			EAsy_all->SetBinError(i, error);
		}

		// EAsy_1
		if(dis_S1_E_1->GetBinContent(i) != 0 && dis_S3_E_1->GetBinContent(i) != 0) {
			double N1 = dis_S1_E_1->GetBinContent(i);
			double N2 = dis_S3_E_1->GetBinContent(i);
			double asymmetry = (-1.0 * N1 + N2) / (N1 + N2);
			double error = sqrt((4 * N1 * N2) / pow(N1 + N2, 3));
			EAsy_1->SetBinContent(i, asymmetry);
			EAsy_1->SetBinError(i, error);
		}

		// EAsy_2
		if(dis_S1_E_2->GetBinContent(i) != 0 && dis_S3_E_2->GetBinContent(i) != 0) {
			double N1 = dis_S1_E_2->GetBinContent(i);
			double N2 = dis_S3_E_2->GetBinContent(i);
			double asymmetry = (-1.0 * N1 + N2) / (N1 + N2);
			double error = sqrt((4 * N1 * N2) / pow(N1 + N2, 3));
			EAsy_2->SetBinContent(i, asymmetry);
			EAsy_2->SetBinError(i, error);
		}
	}

	// Histogram properties
	xAsy_SQ6->SetLineWidth(2);
	xAsy_SQ7->SetLineWidth(2);
	EAsy_ori->SetLineWidth(2);
	EAsy_all->SetLineWidth(2);
	EAsy_1->SetLineWidth(2);
	EAsy_2->SetLineWidth(2);


	xAsy_SQ6->SetLineWidth(2.0);
	xAsy_SQ6->GetXaxis()->SetTitle("local X in electron detector (cm)");
	xAsy_SQ6->GetXaxis()->SetTitleSize(0.05);
	//xAsy_SQ6->GetXaxis()->SetRangeUser(-1.0,10);
	xAsy_SQ6->GetXaxis()->SetTitleOffset(1.00);
	xAsy_SQ6->GetYaxis()->SetTitle("Asymmetry");
	xAsy_SQ6->GetYaxis()->SetTitleSize(0.05);
	xAsy_SQ6->GetYaxis()->SetTitleOffset(1.00);


	xAsy_SQ7->SetLineWidth(2.0);
	xAsy_SQ7->GetXaxis()->SetTitle("local X in electron detector (cm)");
	xAsy_SQ7->GetXaxis()->SetTitleSize(0.05);
	//xAsy_SQ7->GetXaxis()->SetRangeUser(-1.0,10);
	xAsy_SQ7->GetXaxis()->SetTitleOffset(1.00);
	xAsy_SQ7->GetYaxis()->SetTitle("Asymmetry");
	xAsy_SQ7->GetYaxis()->SetTitleSize(0.05);
	xAsy_SQ7->GetYaxis()->SetTitleOffset(1.00);

	EAsy_ori->SetLineWidth(2.0);
	EAsy_ori->GetXaxis()->SetTitle("Energy (GeV)");
	EAsy_ori->GetXaxis()->SetTitleSize(0.05);
	//EAsy_ori->GetXaxis()->SetRangeUser(-1.0,10);
	EAsy_ori->GetXaxis()->SetTitleOffset(1.00);
	EAsy_ori->GetYaxis()->SetTitle("Asymmetry");
	EAsy_ori->GetYaxis()->SetTitleSize(0.05);
	EAsy_ori->GetYaxis()->SetTitleOffset(1.00);  EAsy_1->SetLineWidth(2.0);

	EAsy_all->SetLineWidth(2.0);
	EAsy_all->GetXaxis()->SetTitle("Energy (GeV)");
	EAsy_all->GetXaxis()->SetTitleSize(0.05);
	//EAsy_all->GetXaxis()->SetRangeUser(-1.0,10);
	EAsy_all->GetXaxis()->SetTitleOffset(1.00);
	EAsy_all->GetYaxis()->SetTitle("Asymmetry");
	EAsy_all->GetYaxis()->SetTitleSize(0.05);
	EAsy_all->GetYaxis()->SetTitleOffset(1.00);  EAsy_1->SetLineWidth(2.0);

	EAsy_1->GetXaxis()->SetTitle("Energy (GeV)");
	EAsy_1->GetXaxis()->SetTitleSize(0.05);
	//EAsy_1->GetXaxis()->SetRangeUser(-1.0,10);
	EAsy_1->GetXaxis()->SetTitleOffset(1.00);
	EAsy_1->GetYaxis()->SetTitle("Asymmetry");
	EAsy_1->GetYaxis()->SetTitleSize(0.05);
	EAsy_1->GetYaxis()->SetTitleOffset(1.00);

	EAsy_2->SetLineWidth(2.0);
	EAsy_2->GetXaxis()->SetTitle("Energy (GeV)");
	EAsy_2->GetXaxis()->SetTitleSize(0.05);
	//EAsy_2->GetXaxis()->SetRangeUser(-1.0,10);
	EAsy_2->GetXaxis()->SetTitleOffset(1.00);
	EAsy_2->GetYaxis()->SetTitle("Asymmetry");
	EAsy_2->GetYaxis()->SetTitleSize(0.05);
	EAsy_2->GetYaxis()->SetTitleOffset(1.00);
	// Set other properties like axis titles here...

	// Write histograms to file
	result->cd();
	xAsy_SQ6->Write();
	xAsy_SQ7->Write();
	EAsy_ori->Write();
	EAsy_all->Write();
	EAsy_1->Write();
	EAsy_2->Write();

	// Close files
	result->Close();
	f3->Close();
	f1->Close();
}

