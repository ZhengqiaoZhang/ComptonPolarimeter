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

   TFile *file1 = new TFile("5GeV_test.root","recreate");

   TH1D *en_dis = new TH1D("edis","edis",200,5,10);

   TH1D *xdis_3 = new TH1D("xdis_after_SQ6","xdis_after_SQ6",1000,-10,10);
   TH1D *ydis_3 = new TH1D("ydis_after_SQ6","ydis_after_SQ6",1000,-1,1);

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
	if(Det_after_SQ6_hx!=9999)xdis_3->Fill(Det_after_SQ6_hx*0.1+58986.133260796);
	if(Det_after_SQ6_hy!=9999)ydis_3->Fill(Det_after_SQ6_hy*0.1);

      // if (Cut(ientry) < 0) continue;
   }
   file1->cd();
   xdis_3->Write();
   ydis_3->Write();

   xdis_3->Draw();
}
