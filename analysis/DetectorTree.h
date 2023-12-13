//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Aug 29 13:04:40 2023 by ROOT version 6.21/01
// from TTree DetectorTree/DetectorTree
// found on file: compton.root
//////////////////////////////////////////////////////////

#ifndef DetectorTree_h
#define DetectorTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class DetectorTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        phot_gen;
   Double_t        phot_theta;
   Double_t        phot_phi;
   Double_t        vtx_x;
   Double_t        vtx_y;
   Double_t        vtx_z;
   Double_t        el_gen;
   Double_t        el_theta;
   Double_t        el_phi;
   Bool_t          ExitWin_IsHit;
   Double_t        ExitWin_EnPrim;
   Double_t        ExitWin_en;
   Double_t        ExitWin_hx;
   Double_t        ExitWin_hy;
   Double_t        ExitWin_hz;

   // List of branches
   TBranch        *b_phot_gen;   //!
   TBranch        *b_phot_theta;   //!
   TBranch        *b_phot_phi;   //!
   TBranch        *b_vtx_x;   //!
   TBranch        *b_vtx_y;   //!
   TBranch        *b_vtx_z;   //!
   TBranch        *b_el_gen;   //!
   TBranch        *b_el_theta;   //!
   TBranch        *b_el_phi;   //!
   TBranch        *b_ExitWin_IsHit;   //!
   TBranch        *b_ExitWin_EnPrim;   //!
   TBranch        *b_ExitWin_en;   //!
   TBranch        *b_ExitWin_hx;   //!
   TBranch        *b_ExitWin_hy;   //!
   TBranch        *b_ExitWin_hz;   //!

   DetectorTree(TTree *tree=0);
   virtual ~DetectorTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef DetectorTree_cxx
DetectorTree::DetectorTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("compton.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("compton.root");
      }
      f->GetObject("DetectorTree",tree);

   }
   Init(tree);
}

DetectorTree::~DetectorTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t DetectorTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t DetectorTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void DetectorTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("phot_gen", &phot_gen, &b_phot_gen);
   fChain->SetBranchAddress("phot_theta", &phot_theta, &b_phot_theta);
   fChain->SetBranchAddress("phot_phi", &phot_phi, &b_phot_phi);
   fChain->SetBranchAddress("vtx_x", &vtx_x, &b_vtx_x);
   fChain->SetBranchAddress("vtx_y", &vtx_y, &b_vtx_y);
   fChain->SetBranchAddress("vtx_z", &vtx_z, &b_vtx_z);
   fChain->SetBranchAddress("el_gen", &el_gen, &b_el_gen);
   fChain->SetBranchAddress("el_theta", &el_theta, &b_el_theta);
   fChain->SetBranchAddress("el_phi", &el_phi, &b_el_phi);
   fChain->SetBranchAddress("ExitWin_IsHit", &ExitWin_IsHit, &b_ExitWin_IsHit);
   fChain->SetBranchAddress("ExitWin_EnPrim", &ExitWin_EnPrim, &b_ExitWin_EnPrim);
   fChain->SetBranchAddress("ExitWin_en", &ExitWin_en, &b_ExitWin_en);
   fChain->SetBranchAddress("ExitWin_hx", &ExitWin_hx, &b_ExitWin_hx);
   fChain->SetBranchAddress("ExitWin_hy", &ExitWin_hy, &b_ExitWin_hy);
   fChain->SetBranchAddress("ExitWin_hz", &ExitWin_hz, &b_ExitWin_hz);
   Notify();
}

Bool_t DetectorTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void DetectorTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t DetectorTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef DetectorTree_cxx
