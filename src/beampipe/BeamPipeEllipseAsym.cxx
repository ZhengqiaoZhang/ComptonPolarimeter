
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
#include "G4SubtractionSolid.hh"
#include "G4TessellatedSolid.hh"
#include "G4TriangularFacet.hh"

//local classes
#include "beampipe/BeamPipeEllipseAsym.h"
#include "G4EllipticalTube_new.h"
#include "DetUtils.h"
using namespace std;

//_____________________________________________________________________________
BeamPipeEllipseAsym::BeamPipeEllipseAsym(G4String nam, G4String LorR,G4double xpos,G4double ypos,G4double zpos,G4double length, G4double Dx_t,G4double Dy_t,G4double Dx_b,G4double Dy_b, G4double pipeThick, G4double angle,G4LogicalVolume *top):Detector(), G4VSensitiveDetector(nam), fNam(nam) {

	G4RotationMatrix* yRot = new G4RotationMatrix;
	yRot->rotateY(-1.0*angle*rad);

	G4int numFacets = 500;



	G4TessellatedSolid* ellipticalTube = new G4TessellatedSolid("EllipticalTube");

	for (G4int i = 0; i < numFacets; ++i) {
		G4double angle1 = 2 * M_PI * i / numFacets;
		G4double angle2 = 2 * M_PI * (i + 1) / numFacets;

		G4ThreeVector v1(0.5*Dx_t*cos(angle1), 0.5*Dy_t*sin(angle1), 0.5*length);
		G4ThreeVector v2(0.5*Dx_t*cos(angle2), 0.5*Dy_t*sin(angle2), 0.5*length);
		G4ThreeVector v3(0.5*(Dx_t+2.0*pipeThick)*cos(angle1), 0.5*(Dy_t+2.0*pipeThick)*sin(angle1), 0.5*length);
		G4ThreeVector v4(0.5*(Dx_t+2.0*pipeThick)*cos(angle2), 0.5*(Dy_t+2.0*pipeThick)*sin(angle2), 0.5*length);


		G4TriangularFacet* facet1 = new G4TriangularFacet(v1, v4, v2, ABSOLUTE);
		ellipticalTube->AddFacet((G4VFacet*)facet1);
		G4TriangularFacet* facet2 = new G4TriangularFacet(v1, v3, v4, ABSOLUTE);
		ellipticalTube->AddFacet((G4VFacet*)facet2);

		G4ThreeVector v5(0.5*Dx_b*cos(angle1), 0.5*Dy_b*sin(angle1), -0.5*length);
		G4ThreeVector v6(0.5*Dx_b*cos(angle2), 0.5*Dy_b*sin(angle2), -0.5*length);
		G4ThreeVector v7(0.5*(Dx_b+2.0*pipeThick)*cos(angle1), 0.5*(Dy_b+2.0*pipeThick)*sin(angle1), -0.5*length);
		G4ThreeVector v8(0.5*(Dx_b+2.0*pipeThick)*cos(angle2), 0.5*(Dy_b+2.0*pipeThick)*sin(angle2), -0.5*length);

		G4TriangularFacet* facet3 = new G4TriangularFacet(v5, v6, v8, ABSOLUTE);
		ellipticalTube->AddFacet((G4VFacet*)facet3);
		G4TriangularFacet* facet4 = new G4TriangularFacet(v5, v8, v7, ABSOLUTE);
		ellipticalTube->AddFacet((G4VFacet*)facet4);
	}

	for (G4int i = 0; i < numFacets; ++i) {
		G4double angle1 = 2 * M_PI * i / numFacets;
		G4double angle2 = 2 * M_PI * (i + 1) / numFacets;

		G4ThreeVector v1(0.5*(Dx_t+2.0*pipeThick)*cos(angle1), 0.5*(Dy_t+2.0*pipeThick)*sin(angle1), 0.5*length);
		G4ThreeVector v2(0.5*(Dx_t+2.0*pipeThick)*cos(angle2), 0.5*(Dy_t+2.0*pipeThick)*sin(angle2), 0.5*length);
		G4ThreeVector v3(0.5*(Dx_b+2.0*pipeThick)*cos(angle1), 0.5*(Dy_b+2.0*pipeThick)*sin(angle1), -0.5*length);
		G4ThreeVector v4(0.5*(Dx_b+2.0*pipeThick)*cos(angle2), 0.5*(Dy_b+2.0*pipeThick)*sin(angle2), -0.5*length);

		G4TriangularFacet* facet5 = new G4TriangularFacet(v1, v3, v2, ABSOLUTE);
		ellipticalTube->AddFacet((G4VFacet*)facet5);

		G4TriangularFacet* facet6 = new G4TriangularFacet(v2, v3, v4, ABSOLUTE);
		ellipticalTube->AddFacet((G4VFacet*)facet6);

		G4ThreeVector v5(0.5*Dx_t*cos(angle1), 0.5*Dy_t*sin(angle1), 0.5*length);
		G4ThreeVector v6(0.5*Dx_t*cos(angle2), 0.5*Dy_t*sin(angle2), 0.5*length);
		G4ThreeVector v7(0.5*Dx_b*cos(angle1), 0.5*Dy_b*sin(angle1), -0.5*length);
		G4ThreeVector v8(0.5*Dx_b*cos(angle2), 0.5*Dy_b*sin(angle2), -0.5*length);

		G4TriangularFacet* facet7 = new G4TriangularFacet(v5, v8, v7, ABSOLUTE);
		ellipticalTube->AddFacet((G4VFacet*)facet7);

		G4TriangularFacet* facet8 = new G4TriangularFacet(v5, v6, v8, ABSOLUTE);
		ellipticalTube->AddFacet((G4VFacet*)facet8);



	}

	ellipticalTube->SetSolidClosed(true);

	G4String nam_logic = fNam+"_logic";

	G4VisAttributes *vis_vessel = new G4VisAttributes();
	vis_vessel->SetColor(0.5, 0.5, 0.5); // grey
	vis_vessel->SetLineWidth(2);
	vis_vessel->SetForceSolid(true);


	G4Material *mat= G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");


	if(LorR!="All"){

		G4double lrno;
		if(LorR=="Right")lrno=1.0;
		if(LorR=="Left")lrno=-1.0;



		G4Box *cutoff = new G4Box(fNam, 2*meter/2., 2*meter/2., 10*meter/2.);
		G4ThreeVector subtractBoxPosition(lrno*1*meter, 0, 0);
		G4Transform3D subtractBoxTransform(G4RotationMatrix(), subtractBoxPosition);

		G4SubtractionSolid* shape_vessel = new G4SubtractionSolid("subtractedSolid", ellipticalTube,cutoff, subtractBoxTransform);

		G4LogicalVolume *vol_logic = new G4LogicalVolume(shape_vessel,mat,fNam);

		vol_logic->SetVisAttributes(vis_vessel);
		new G4PVPlacement(yRot, G4ThreeVector(xpos,0*meter, zpos), vol_logic,fNam, top, false, 0);

	}

	if(LorR=="All"){
		G4LogicalVolume *vol_logic = new G4LogicalVolume(ellipticalTube,mat,fNam);

		vol_logic->SetVisAttributes(vis_vessel);

		new G4PVPlacement(yRot, G4ThreeVector(xpos,0*meter, zpos), vol_logic,fNam, top, false, 0);
	}



}//BeamPipeEllipseAsym

//_____________________________________________________________________________
G4bool BeamPipeEllipseAsym::ProcessHits(G4Step *step, G4TouchableHistory*) {

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
	//G4cout << "BeamPipeEllipseAsym::ProcessHits: " << track->GetParentID() << G4endl;

	return true;

}//ProcessHits

//_____________________________________________________________________________
void BeamPipeEllipseAsym::CreateOutput(TTree *tree) {

	//output from BeamPipeEllipseAsym


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
void BeamPipeEllipseAsym::ClearEvent() {
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














