
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

//local classes
#include "beampipe/BeamPipeEllipseCone.h"
#include "DetUtils.h"
using namespace std;

//_____________________________________________________________________________
BeamPipeEllipseCone::BeamPipeEllipseCone(G4String nam, G4double xpos,G4double ypos,G4double zpos,G4double length, G4double Dx,G4double Dy,G4double Dx_top,G4double Dy_top, G4double pipeThick, G4double angle,G4LogicalVolume *top):Detector(), G4VSensitiveDetector(nam), fNam(nam) {

	G4RotationMatrix* yRot = new G4RotationMatrix;
	yRot->rotateY(-1.0*angle*rad);

	G4double zMax_Innter = (Dx_top/(Dx-Dx_top)+0.5)*length;
	G4double zMax_Outer = ((Dx_top+2.0*pipeThick)/(Dx-Dx_top)+0.5)*length;

	G4double pxSemiAxis_Outer = (0.5*Dx+pipeThick)/(zMax_Outer+0.5*length);
	G4double pySemiAxis_Outer = (0.5*Dy+pipeThick)/(zMax_Outer+0.5*length);
	G4double pxSemiAxis_Inner = (0.5*Dx)/(zMax_Outer+0.5*length);
        G4double pySemiAxis_Inner = (0.5*Dy)/(zMax_Outer+0.5*length);

	G4EllipticalCone * Cell_Outer = new G4EllipticalCone( "Cell_Outer", pxSemiAxis_Outer,pySemiAxis_Outer,zMax_Outer,0.5*length);
	G4EllipticalCone * Cell_Inner = new G4EllipticalCone( "Cell_Inner", pxSemiAxis_Inner,pySemiAxis_Inner,zMax_Innter,0.5*length+0.0001*meter);

	G4SubtractionSolid *shape_vessel = new G4SubtractionSolid(fNam+"_PipeEllipseCone",Cell_Outer, Cell_Inner);


	G4String nam_logic = fNam+"_logic";


	G4Material *mat= G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
	G4LogicalVolume *vol_logic = new G4LogicalVolume(shape_vessel,mat,fNam);


	G4VisAttributes *vis_vessel = new G4VisAttributes();
	vis_vessel->SetColor(0.5, 0.5, 0.5); // grey
	vis_vessel->SetLineWidth(2);
	vis_vessel->SetForceSolid(true);
	vol_logic->SetVisAttributes(vis_vessel);

	new G4PVPlacement(yRot, G4ThreeVector(xpos,0*meter, zpos), vol_logic,fNam, top, false, 0);


}//BeamPipeEllipseCone

//_____________________________________________________________________________
G4bool BeamPipeEllipseCone::ProcessHits(G4Step *step, G4TouchableHistory*) {

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
	//G4cout << "BeamPipeEllipseCone::ProcessHits: " << track->GetParentID() << G4endl;

	return true;

}//ProcessHits

//_____________________________________________________________________________
void BeamPipeEllipseCone::CreateOutput(TTree *tree) {

	//output from BeamPipeEllipseCone


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
void BeamPipeEllipseCone::ClearEvent() {
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














