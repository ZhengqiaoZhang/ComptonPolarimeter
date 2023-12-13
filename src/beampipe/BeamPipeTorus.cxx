
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
#include "G4FieldManager.hh"
#include "G4UniformMagField.hh"
#include "G4EqMagElectricField.hh"
#include "G4TransportationManager.hh"
#include "G4ScaledSolid.hh"
#include <CLHEP/Geometry/Transform3D.h>
#include "G4SubtractionSolid.hh"


//local classes
#include "beampipe/BeamPipeTorus.h"
#include "DetUtils.h"
using namespace std;

//_____________________________________________________________________________
BeamPipeTorus::BeamPipeTorus(G4String nam, G4double xpos,G4double ypos,G4double zpos,G4double length, G4double pipeSizeX,G4double pipeSizeY, G4double pipeThick, G4double angle, G4double theta,G4double bfield,G4LogicalVolume *top):
	Detector(), G4VSensitiveDetector(nam), fNam(nam) {

		G4RotationMatrix* yRot = new G4RotationMatrix;
		yRot->rotateY(1.0*(M_PI-angle)*rad);
		yRot->rotateX(-1.0*M_PI*0.5*rad);


		G4double Rtor = length/theta;
		G4double z0 = zpos - Rtor*sin(angle);
		G4double x0 = xpos + Rtor*cos(angle);

		G4Torus *shape_1 = new G4Torus(fNam,pipeSizeX/2,pipeSizeX/2+pipeThick,Rtor,-0.5*theta,theta);





		G4Material *mat= G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");

		G4RotationMatrix cutRot;
                cutRot.rotateY(-1.0 * 0.0072433567 * rad);

                G4Tubs *cutoff = new G4Tubs("fNam",0*cm,1.056*meter,20.0*meter/2.0, 0, 360*deg);
                G4ThreeVector subtractBoxPosition(-0.46945144*meter+x0, 0*meter,-68.3811822708711361*meter+z0);
                G4Transform3D subtractBoxTransform(cutRot,subtractBoxPosition);
		G4SubtractionSolid* shape_final = new G4SubtractionSolid("subtractedSolid",shape_1,cutoff,subtractBoxTransform);


		 G4Scale3D scale(1.0, 1.0, pipeSizeY/pipeSizeX);

                G4ScaledSolid *shape = new G4ScaledSolid(nam+"scale",shape_final,scale);







		G4LogicalVolume *vol_logic = new G4LogicalVolume(shape_final,mat,fNam);
		//		G4LogicalVolume *vol_logic2 = new G4LogicalVolume(pipe2,mat,fNam);

		G4Torus *shape_Bfield_1 = new G4Torus ("Bfield_torus",0.*meter,pipeSizeX/2-0.001*meter,Rtor,-0.5*theta,theta);
		G4ScaledSolid *shape_Bfield = new G4ScaledSolid(nam+"scale",shape_Bfield_1,scale);
			
		G4Material *mat_inner = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
		G4LogicalVolume *vol_inner = new G4LogicalVolume(shape_Bfield, mat_inner, "Bfield_inner");

		G4UniformMagField *field = new G4UniformMagField(G4ThreeVector(0, bfield, 0));
		G4FieldManager *fman =  new G4FieldManager();
		fman->SetDetectorField(field);
		fman->CreateChordFinder(field);

		vol_inner->SetFieldManager(fman, true);
		vol_inner->SetVisAttributes(G4VisAttributes::GetInvisible());
		new G4PVPlacement(yRot, G4ThreeVector(x0,0*meter,z0), vol_inner,"Bfield_inner",top, false, 0);//add field;



		G4VisAttributes *vis_vessel = new G4VisAttributes();
		vis_vessel->SetColor(0.5, 0.5, 0.5); // blue
		vis_vessel->SetLineWidth(2);
		vis_vessel->SetForceSolid(true);
		vol_logic->SetVisAttributes(vis_vessel);
		//		vol_logic2->SetVisAttributes(vis_vessel);
		//


				new G4PVPlacement(yRot, G4ThreeVector(x0,0*meter, z0), vol_logic,fNam, top, false, 0);
		//		new G4PVPlacement(Rot, G4ThreeVector(35.547679*meter,0 ,70.599071*meter), vol_logic2,"pipe2", top, false, 0);


	}//BeamPipeTorus

//_____________________________________________________________________________
G4bool BeamPipeTorus::ProcessHits(G4Step *step, G4TouchableHistory*) {

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
	//G4cout << "BeamPipeTorus::ProcessHits: " << track->GetParentID() << G4endl;

	return true;

}//ProcessHits

//_____________________________________________________________________________
void BeamPipeTorus::CreateOutput(TTree *tree) {

	//output from BeamPipeTorus


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
void BeamPipeTorus::ClearEvent() {
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














