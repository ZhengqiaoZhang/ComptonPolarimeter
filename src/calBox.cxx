
//_____________________________________________________________________________
//
// Simple calorimeter version 2 (V2) for tests,
// absorbs every particle in a single step with no secondaries.
//
// Historical note: such calorimeter was originally invented by Jara Cimrman.
//_____________________________________________________________________________

//C++

//ROOT
#include "TTree.h"

//Geant
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

//local classes
#include "calBox.h"
#include "DetUtils.h"
using namespace std;

//_____________________________________________________________________________
calBox::calBox(const G4String& nam, G4double x, G4double y, G4double z, G4double xysize, G4double zsize,G4double rotation,G4double color1,G4double color2,G4double color3,G4LogicalVolume *top): Detector(),
  G4VSensitiveDetector(nam), fNam(nam) {

  G4cout << "  calBox: " << fNam << G4endl;

  //detector shape
  //G4double xysize = 2000*mm;
  G4Box *shape = new G4Box(fNam, xysize/2., xysize/2., zsize/2.);

  //SILICON_DIOXIDE material
  G4Material *mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

  //logical volume
  G4LogicalVolume *vol = new G4LogicalVolume(shape, mat, fNam);

  //visibility
  G4VisAttributes *vis = new G4VisAttributes();
  vis->SetColor(color1, color2, color3); // 
  vis->SetForceSolid(true);
  vis->SetLineWidth(2.0);
  vol->SetVisAttributes(vis);

  //rotation in x-z plane by rotation along y
G4RotationMatrix* rot_y = new G4RotationMatrix;
        rot_y->rotateY(rotation*rad);


  //placement with rotation at a given position in x, y and z
  G4double xpos = x; // center position in x, mm

  G4double ypos = y; // position in y, mm

  G4double zpos = z; // position of the front face along z

  //put to the top volume
  new G4PVPlacement(rot_y,G4ThreeVector(xpos,ypos, zpos), vol, fNam, top, false, 0);

  //load flag for primary particles in ProcessHits if defined
  fSelectPrim = true;
  //G4cout << "  calBox, select_prim: " << fSelectPrim << G4endl;

  //clear all event variables
  ClearEvent();

}//calBox

//_____________________________________________________________________________
G4bool calBox::ProcessHits(G4Step *step, G4TouchableHistory*) {

  //remove the track
  G4Track *track = step->GetTrack();
  G4StepPoint* prePoint = step->GetPreStepPoint();
  G4double energy = prePoint->GetTotalEnergy();

   fEnAll  =energy;
   fEnPrim = energy;

  // G4cout<<"stauts="<<track->GetTrackStatus()<<G4endl;
  track->SetTrackStatus(fKillTrackAndSecondaries);
//  track->SetTrackStatus(fAlive);

// G4cout << "step->GetTotalEnergyDeposit(); step->GetTotalEnergyDeposit(); step->GetTotalEnergyDeposit();================="<<step->GetTotalEnergyDeposit() << G4endl;


  //G4cout << "after:  " << track->GetTotalEnergy() << G4endl;
  //G4cout << G4endl;

//  fEnAll += step->GetTotalEnergyDeposit();//track->GetTotalEnergy();

  //primary track only
  if( fSelectPrim == true && track->GetParentID() != 0 ) return true;

  //consider only first hit by the primary track
  if(fIsHit == kFALSE) {


    fIsHit = kTRUE;
  } else {
    return true;
  }

  //energy
  //cout<<fEnPrim<<endl;
  //if(fEnPrim<10)cout<<"E="<<fEnPrim<<endl;

  //hit position
  const G4ThreeVector hp = step->GetPostStepPoint()->GetPosition();
  fHx = hp.x();
  fHy = hp.y();
  fHz = hp.z();

//  G4cout<<" Hits X: "<<fHx<<" Hits Y: "<<fHy<<" Hits Z: "<<fHz<<G4endl;
  //G4cout << "calBox::ProcessHits: " << track->GetParentID() << G4endl;

  return true;

}//ProcessHits

//_____________________________________________________________________________
void calBox::CreateOutput(TTree *tree) {

  //output from calBox

  DetUtils u(fNam, tree);

  u.AddBranch("_IsHit", &fIsHit, "O");

  u.AddBranch("_EnPrim", &fEnPrim, "D");
  u.AddBranch("_en", &fEnAll, "D");

  u.AddBranch("_hx", &fHx, "D");
  u.AddBranch("_hy", &fHy, "D");
  u.AddBranch("_hz", &fHz, "D");

}//CreateOutput

//_____________________________________________________________________________
void calBox::ClearEvent() {

  fIsHit = kFALSE;

  fEnPrim = -9999.;
  fEnAll = 0;

  fHx = 9999.;
  fHy = 9999.;
  fHz = 9999.;

}//ClearEvent














