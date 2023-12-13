
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

//local classes
#include "beampipe/BeamPipeEllipse.h"
#include "DetUtils.h"
using namespace std;

//_____________________________________________________________________________
BeamPipeEllipse::BeamPipeEllipse(G4String nam, G4double xpos,G4double ypos,G4double zpos,G4double length, G4double Dx,G4double Dy, G4double pipeThick, G4double angle,G4LogicalVolume *top):Detector(), G4VSensitiveDetector(nam), fNam(nam) {

	G4RotationMatrix* yRot = new G4RotationMatrix;
	yRot->rotateY(-1.0*angle*rad);
	G4EllipticalTube * Cell_Outer = new G4EllipticalTube( "Cell_Outer", 0.5*Dx+pipeThick, 0.5*Dy+pipeThick, 0.5*length);
	G4EllipticalTube * Cell_Inner = new G4EllipticalTube( "Cell_Inner", 0.5*Dx, Dy*0.5, 0.5*length+0.001*meter);

	G4SubtractionSolid *shape_vessel = new G4SubtractionSolid(fNam+"_PipeEllipse",Cell_Outer, Cell_Inner);


	G4String nam_logic = fNam+"_logic";


	G4Material *mat= G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
	G4LogicalVolume *vol_logic;
		if(nam!="Pipe_Q5_before")vol_logic = new G4LogicalVolume(shape_vessel,mat,fNam);

	G4VisAttributes *vis_vessel = new G4VisAttributes();
	vis_vessel->SetColor(0.5, 0.5, 0.5); // grey
	vis_vessel->SetLineWidth(2);
	vis_vessel->SetForceSolid(true);
	vol_logic->SetVisAttributes(vis_vessel);

	if(nam=="Pipe_Q5_before"){
		G4RotationMatrix cutRot;
		cutRot.rotateY(-1.0 * 0.014486713 * rad);

		G4Tubs *cutoff = new G4Tubs("fNam",0*cm,0.018*meter,3.9848620*meter/2.0, 0, 360*deg);
		G4ThreeVector subtractBoxPosition(-0.047285480*meter, 0, 1.8108000*meter);
		G4Transform3D subtractBoxTransform(cutRot,subtractBoxPosition);

		G4SubtractionSolid* shape_final = new G4SubtractionSolid("subtractedSolid",shape_vessel,cutoff,subtractBoxTransform);

		vol_logic = new G4LogicalVolume(shape_final,mat,fNam);
		vol_logic->SetVisAttributes(vis_vessel);
		new G4PVPlacement(yRot, G4ThreeVector(xpos,0*meter, zpos), vol_logic,fNam, top, false, 0);
	}





	if(nam!="Pipe_Q5_before")new G4PVPlacement(yRot, G4ThreeVector(xpos,0*meter, zpos), vol_logic,fNam, top, false, 0);


}//BeamPipeEllipse

//_____________________________________________________________________________
G4bool BeamPipeEllipse::ProcessHits(G4Step *step, G4TouchableHistory*) {

	//remove the track
	G4Track *track = step->GetTrack();

	//G4cout << track->GetParentID() << G4endl;
	//G4cout << "before: " << track->GetTotalEnergy() << G4endl;
	//G4cout << "before: " << track->GetKineticEnergy() << G4endl;

	track->SetTrackStatus(fKillTrackAndSecondaries);

	//G4cout << "after:  " << track->GetTotalEnergy() << G4endl;
	//G4cout << G4endl;

	fEnAll += track->GetTotalEnergy();

	const G4ParticleDefinition *partdef = track->GetParticleDefinition();
	fPDG[index]=partdef->GetPDGEncoding();


	//primary track only
	if( fSelectPrim == true && track->GetParentID() != 0 ) return true;

	//consider only first hit by the primary track
	if(fIsHit[index]== kFALSE) {

		fIsHit[index] = kTRUE;
	} else {
		return true;
	}

	//energy
	fEnPrim[index] = track->GetTotalEnergy();
	//cout<<fEnPrim<<endl;
	//if(fEnPrim<10)cout<<"E="<<fEnPrim<<endl;

	//hit position
	const G4ThreeVector hp = step->GetPostStepPoint()->GetPosition();
	fHx[index] = hp.x();
	fHy[index] = hp.y();
	fHz[index] = hp.z();
	index++;
	ntrack++;

	//  G4cout<<" Hits X: "<<fHx<<" Hits Y: "<<fHy<<" Hits Z: "<<fHz<<G4endl;
	//G4cout << "BeamPipeEllipse::ProcessHits: " << track->GetParentID() << G4endl;

	return true;

}//ProcessHits

//_____________________________________________________________________________
void BeamPipeEllipse::CreateOutput(TTree *tree) {

	//output from BeamPipeEllipse


	tree->Branch((fNam+"_ntrack").c_str(),&ntrack,(fNam+"_ntrack"+"/"+"I").c_str());
	tree->Branch((fNam+"_IsHit").c_str(),fIsHit,(fNam+"_IsHit"+"[10]"+"/"+"O").c_str());
	tree->Branch((fNam+"_EnPrim").c_str(), fEnPrim, (fNam+"_EnPrim[10]"+"/"+"D").c_str());
	tree->Branch((fNam+"_en").c_str(), &fEnAll, (fNam+"_en"+"/"+"D").c_str());
	tree->Branch((fNam+"_hx").c_str(),fHx, (fNam+"_hx[10]"+"/"+"D").c_str());
	tree->Branch((fNam+"_hy").c_str(),fHy, (fNam+"_hy[10]"+"/"+"D").c_str());
	tree->Branch((fNam+"_hz").c_str(),fHz, (fNam+"_hz[10]"+"/"+"D").c_str());
	tree->Branch((fNam+"_PDG").c_str(),fPDG,(fNam+"_PDG[10]"+"/"+"I").c_str());


}//CreateOutput

//_____________________________________________________________________________
void BeamPipeEllipse::ClearEvent() {
	ntrack = 0;
	for(int i=0;i<10;i++){
		fIsHit[i] = kFALSE;

		fEnPrim[i] = -9999.;

		fHx[i] = 9999.;
		fHy[i] = 9999.;
		fHz[i] = 9999.;
		fPDG[i]= 0;


	}
	index = 0;
	fEnAll= 0;

}//ClearEvent














