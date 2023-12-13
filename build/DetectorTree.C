#define DetectorTree_cxx
#include "DetectorTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void DetectorTree::Loop()
{
//   In a ROOT session, you can do:
//      root> .L DetectorTree.C
//      root> DetectorTree t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   TFile *file1 = new TFile("18GeV_S1.root","recreate");

   TH1D *en_dis = new TH1D("edis","edis",200,5,10);

   TH1D *xdis_3 = new TH1D("xdis_after_SQ6","xdis_after_SQ6",1000,-20,20);
   TH1D *ydis_3 = new TH1D("ydis_after_SQ6","ydis_after_SQ6",1000,-1,1);
   TH1D *xdis_4 = new TH1D("xdis_after_SQ7","xdis_after_SQ7",1000,-20,20);
   TH1D *ydis_4 = new TH1D("ydis_after_SQ7","ydis_after_SQ7",1000,-1,1);
   TH1D *energy_all = new TH1D("energy_all","energy_all",2000,0,20);
   TH1D *energy_acpt_1 = new TH1D("energy_acpt_1","energy_acpt_1",2000,0,20);
   TH1D *energy_acpt_2 = new TH1D("energy_acpt_2","energy_acpt_2",2000,0,20);


   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
	if(Det_after_SQ6_hx!=9999)xdis_3->Fill(Det_after_SQ6_hx*0.1+58986.133260796);
	if(Det_after_SQ6_hy!=9999)ydis_3->Fill(Det_after_SQ6_hy*0.1);
	if(Det_after_SQ7_hx!=9999)xdis_4->Fill(Det_after_SQ7_hx*0.1+58994.745);
	if(Det_after_SQ7_hy!=9999)ydis_4->Fill(Det_after_SQ7_hy*0.1);
	if(Det_after_SQ7_hx!=9999)energy_acpt_1->Fill(Det_after_SQ7_EnPrim*0.001);
	if(Det_after_SQ6_hx!=9999)energy_acpt_2->Fill(Det_after_SQ6_EnPrim*0.001);
	energy_all->Fill(el_gen);

      // if (Cut(ientry) < 0) continue;
   }
   file1->cd();
   xdis_3->Write();
   ydis_3->Write();
   xdis_4->Write();
   ydis_4->Write();
   energy_all->Write();
   energy_acpt_1->Write();
   energy_acpt_2->Write();

   xdis_3->Draw();
}
