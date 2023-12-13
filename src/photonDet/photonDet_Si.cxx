//Geant headers
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVReplica.hh"
#include "G4Cons.hh"
#include "G4Box.hh"
#include "G4SystemOfUnits.hh"
#include "G4PVPlacement.hh"
#include "G4FieldManager.hh"
#include "G4UniformMagField.hh"
#include "G4VisAttributes.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4RotationMatrix.hh"
#include "G4QuadrupoleMagField.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4OpticalPhoton.hh"

//local headers
#include "DetUtils.h"
#include "photonDet/photonDet_Si.h"

//_____________________________________________________________________________
photonDet_Si::photonDet_Si(G4String nam, G4double xpos,G4double ypos,G4double zpos, G4double angle, G4double xysiz ,G4double zsiz_Si ,G4double deltaD,G4LogicalVolume *top):
	Detector(), G4VSensitiveDetector(nam), fNam(nam) {

		//
		G4double density;
		G4double z,a;
		G4NistManager* man = G4NistManager::Instance();

		auto defaultMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");


		G4RotationMatrix* yRot = new G4RotationMatrix;
		yRot->rotateY(-1.0*angle*rad);


		///silicon

		G4Box *shape_Si = new G4Box(fNam, xysiz/2., xysiz/2., zsiz_Si/2.);


		//SILICON_DIOXIDE material
		G4Material *mat_Si = G4NistManager::Instance()->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

		//logical volume
		G4LogicalVolume *vol_Si = new G4LogicalVolume(shape_Si, mat_Si, fNam);

		//visibility
		G4VisAttributes *vis_Si = new G4VisAttributes();
		vis_Si->SetColor(0.45,0.25,0.0); // brown
						 //vis_Si->SetLineWidth(2.0);
		vis_Si->SetForceSolid(true);
		vol_Si->SetVisAttributes(vis_Si);

		//rotation in x-z plane by rotation along y

		//placement with rotation at a given position in x, y and z

		//put to the top volume

		new G4PVPlacement(
				yRot,                // rotation
				G4ThreeVector(xpos, ypos, zpos+deltaD),
				vol_Si,          // its logical volume
				"SiStrip",    // its name
				top,          // its mother  volume
				false,            // no boolean operation
				0,                // copy number
				true);  // checking overlaps
					//


					//clear all event variables
		ClearEvent();


	}//photonDet_Si

//_____________________________________________________________________________
G4bool photonDet_Si::ProcessHits(G4Step *step, G4TouchableHistory*) {

	//remove the track entering the magnet vessel
	//
	G4Track *track = step->GetTrack();
	G4StepPoint* prePoint = step->GetPreStepPoint();
	G4double energy = prePoint->GetTotalEnergy();
	if(fEnPrim==-9999)fEnPrim=energy;

	//increment energy deposit in the detector in the event
	//G4cout << "step->GetTotalEnergyDeposit(); step->GetTotalEnergyDeposit(); step->GetTotalEnergyDeposit();================="<<step->GetTotalEnergyDeposit() << G4endl;

	fEdep += step->GetTotalEnergyDeposit();


	//first point in the detector in the event
	if(fZ > 9998.) {

		const G4ThreeVector point = step->GetPreStepPoint()->GetPosition();

		fX = point.x();
		fY = point.y();
		fZ = point.z();
	}



	return true;

}//ProcessHits


void photonDet_Si::ClearEvent() {

	//G4cout << "BoxCal::ClearEvent" << G4endl;

	//clear event variables
	fEdep = 0;
	fEnPrim = -9999;
	fX = 9999.;
	fY = 9999.;
	fZ = 9999.;

}//ClearEvent

void photonDet_Si::CreateOutput(TTree *tree) {

	DetUtils u(fNam, tree);

	u.AddBranch("_EnPrim", &fEnPrim, "D");
	u.AddBranch("_en", &fEdep, "D");
	u.AddBranch("_x", &fX, "D");
	u.AddBranch("_y", &fY, "D");
	u.AddBranch("_z", &fZ, "D");


}//CreateOutput


































