
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
#include "G4ParticleDefinition.hh"
#include "G4Torus.hh"
#include "G4Tubs.hh"
#include "G4EllipticalTube.hh"
#include "G4SubtractionSolid.hh"
#include "G4EllipticalCone.hh"
#include "G4Para.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"


//local classes
#include "beampipe/BeamPipeExitWin.h"
#include "DetUtils.h"
using namespace std;

//_____________________________________________________________________________
BeamPipeExitWin::BeamPipeExitWin(G4String nam, G4double xpos, G4double ypos,G4double zpos, G4double Rpipe,G4double sizeZ,G4double pipeThick,G4double R_win, G4double angle,G4LogicalVolume *top):Detector(), G4VSensitiveDetector(nam), fNam(nam) {


	G4RotationMatrix* yRot = new G4RotationMatrix;
	yRot->rotateY(-1.0*angle*rad);


	G4Tubs *pipe_outer = new G4Tubs("fNam",0*cm,Rpipe+pipeThick,sizeZ/2.0, 0, 360*deg);
	G4Tubs *pipe_inter = new G4Tubs("fNam",0*cm,Rpipe,sizeZ/2.0, 0, 360*deg);

	G4RotationMatrix *rot_1 = new G4RotationMatrix;
	G4ThreeVector subtractPosition_s1(0*cm, 0*meter,-1.0*pipeThick);
	G4Transform3D subtractTransform_s1(*rot_1,subtractPosition_s1);

	G4SubtractionSolid *shape1  = new G4SubtractionSolid(fNam,pipe_outer,pipe_inter,subtractTransform_s1);

	G4Tubs *exit_win = new G4Tubs("fNam",0*cm,R_win,10*cm, 0, 360*deg);

	G4ThreeVector subtractPosition_s_win(-0.5*cm, 0*meter,1.0*sizeZ/2.0);
	G4Transform3D subtractTransform_s_win(*rot_1,subtractPosition_s_win);
	G4SubtractionSolid *shape2  = new G4SubtractionSolid(fNam,shape1,exit_win,subtractTransform_s_win);


	G4EllipticalTube * Cell_Outer = new G4EllipticalTube( "Cell_Outer", 0.5*0.07*meter+pipeThick, 0.5*0.049*meter+pipeThick,3.0*meter);

	G4RotationMatrix *Rot_Q5 = new G4RotationMatrix;;
        Rot_Q5->rotateY(0.014486713 * rad);
	G4ThreeVector subtractPosition_Q5(-0.47684484*meter+xpos, 0*meter, -65.88118730084092083*meter+zpos);
	G4Transform3D subtractTransform_Q5(*Rot_Q5,subtractPosition_Q5);

	G4SubtractionSolid *shape_vessel_1 = new G4SubtractionSolid(fNam+"_PipeEllipse",shape2, Cell_Outer,subtractTransform_Q5);
	


	//G4String nam_logic = fNam;


	G4Material *mat= G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
	G4LogicalVolume *vol_logic = new G4LogicalVolume(shape_vessel_1,mat,fNam);


	G4VisAttributes *vis_vessel = new G4VisAttributes();
	vis_vessel->SetColor(1.0, 0., 0.); // grey
	vis_vessel->SetLineWidth(2);
	vis_vessel->SetForceSolid(true);
	vol_logic->SetVisAttributes(vis_vessel);

	new G4PVPlacement(yRot, G4ThreeVector(xpos,ypos, zpos), vol_logic,fNam, top, false, 0);


}//BeamPipeExitWin

//_____________________________________________________________________________
G4bool BeamPipeExitWin::ProcessHits(G4Step *step, G4TouchableHistory*) {

	//remove the track
	G4Track *track = step->GetTrack();

  G4StepPoint* prePoint = step->GetPreStepPoint();
  G4double energy = prePoint->GetTotalEnergy();
  fEnPrim=energy;
  fEnAll=energy;


	//G4cout << track->GetParentID() << G4endl;
	//G4cout << "before: " << track->GetTotalEnergy() << G4endl;
	//G4cout << "before: " << track->GetKineticEnergy() << G4endl;

	track->SetTrackStatus(fKillTrackAndSecondaries);

	//G4cout << "after:  " << track->GetTotalEnergy() << G4endl;
	//G4cout << G4endl;


	const G4ParticleDefinition *partdef = track->GetParticleDefinition();




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
void BeamPipeExitWin::CreateOutput(TTree *tree) {

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
void BeamPipeExitWin::ClearEvent() {
	fIsHit = kFALSE;

	fEnPrim = -9999.;
	fEnAll = 0;

	fHx = 9999.;
	fHy = 9999.;
	fHz = 9999.;


}//ClearEvent














