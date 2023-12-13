
//_____________________________________________________________________________
//
// absorbs every particle in a single step with no secondaries.
//
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
#include "G4QuadrupoleMagField.hh"
#include "G4FieldManager.hh"



//local classes
#include "beampipe/BeamPipeEllipse_Quad.h"
#include "DetUtils.h"
using namespace std;

//_____________________________________________________________________________
BeamPipeEllipse_Quad::BeamPipeEllipse_Quad(G4String nam, G4double xpos,G4double ypos,G4double zpos,G4double length, G4double Dx,G4double Dy, G4double pipeThick, G4double angle,G4double gradient,G4LogicalVolume *top):Detector(), G4VSensitiveDetector(nam), fNam(nam) {

	G4RotationMatrix* yRot = new G4RotationMatrix;
	yRot->rotateY(-1.0*angle*rad);
	G4EllipticalTube * Cell_Outer = new G4EllipticalTube( "Cell_Outer", 0.5*Dx+pipeThick, 0.5*Dy+pipeThick, 0.5*length);
	G4EllipticalTube * Cell_Inner = new G4EllipticalTube( "Cell_Inner", 0.5*Dx, 0.5*Dy, 0.5*length+1e-4*meter);

	G4EllipticalTube * shape_Bfield = new G4EllipticalTube( "shape_Bfield", 0.5*Dx-0.001*meter, 0.5*Dy-0.001*meter, 0.5*length);

	G4Material *mat_inner = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
	G4LogicalVolume *vol_Bfield = new G4LogicalVolume(shape_Bfield, mat_inner, "Bfield_inner");


	G4QuadrupoleMagField *field = new G4QuadrupoleMagField(gradient, G4ThreeVector(xpos, ypos, zpos),yRot);
	G4FieldManager *fman = new G4FieldManager();
	fman->SetDetectorField(field);
	fman->CreateChordFinder(field);

	vol_Bfield->SetFieldManager(fman, true);
	vol_Bfield->SetVisAttributes(G4VisAttributes::GetInvisible());
	new G4PVPlacement(yRot, G4ThreeVector(xpos,0*meter, zpos), vol_Bfield,"Bfield_inner", top, false, 0);




	G4SubtractionSolid *shape_vessel = new G4SubtractionSolid(fNam+"_PipeEllipse",Cell_Outer, Cell_Inner);


	G4String nam_logic = fNam+"_logic";


	G4Material *mat= G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
	G4LogicalVolume *vol_logic = new G4LogicalVolume(shape_vessel,mat,fNam);


	G4VisAttributes *vis_vessel = new G4VisAttributes();
	vis_vessel->SetColor(0.5, 0.5, 0.5); // grey
	vis_vessel->SetLineWidth(2);
	vis_vessel->SetForceSolid(true);
	vol_logic->SetVisAttributes(vis_vessel);

	new G4PVPlacement(yRot, G4ThreeVector(xpos,0*meter, zpos), vol_logic,fNam, top, false, 0);


}//BeamPipeEllipse_Quad

//_____________________________________________________________________________
G4bool BeamPipeEllipse_Quad::ProcessHits(G4Step *step, G4TouchableHistory*) {

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
	//G4cout << "BeamPipeEllipse_Quad::ProcessHits: " << track->GetParentID() << G4endl;

	return true;

}//ProcessHits

//_____________________________________________________________________________
void BeamPipeEllipse_Quad::CreateOutput(TTree *tree) {

	//output from BeamPipeEllipse_Quad


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
void BeamPipeEllipse_Quad::ClearEvent() {
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














