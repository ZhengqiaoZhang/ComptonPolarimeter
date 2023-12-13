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
#include "Shield.h"

//_____________________________________________________________________________
Shield::Shield(G4String nam,G4String mat, G4double xpos,G4double ypos,G4double zpos, G4double angle,G4double xysiz_shield,G4double zsiz_shield, G4LogicalVolume *top):
	Detector(), G4VSensitiveDetector(nam), fNam(nam) {

		//
		G4double density;
		G4double z,a;
		G4NistManager* man = G4NistManager::Instance();


		auto defaultMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");


		G4RotationMatrix* yRot = new G4RotationMatrix;
		yRot->rotateY(-1.0*angle*rad);



		//shield layer
		G4Box *shape_shield = new G4Box("shield_layer", xysiz_shield/2., xysiz_shield/2., zsiz_shield/2.);


		G4Material *mat_shield = G4NistManager::Instance()->FindOrBuildMaterial(mat);

		//logical volume
		G4LogicalVolume *vol_shield = new G4LogicalVolume(shape_shield, mat_shield, fNam);

		//visibility
		G4VisAttributes *vis_shield = new G4VisAttributes();
		vis_shield->SetColor(0.2,0.85,0.1); // brown
						  //vis_Si->SetLineWidth(2.0);
		vis_shield->SetForceSolid(true);
		vol_shield->SetVisAttributes(vis_shield);

		//rotation in x-z plane by rotation along y

		//placement with rotation at a given position in x, y and z

		//put to the top volume

		new G4PVPlacement(
				yRot,                // rotation
				G4ThreeVector(xpos, ypos, zpos),
				vol_shield,          // its logical volume
				fNam,    // its name
				top,          // its mother  volume
				false,            // no boolean operation
				0,                // copy number
				true);  // checking overlaps
					//

					//clear all event variables
		ClearEvent();


	}//Shield

//_____________________________________________________________________________
G4bool Shield::ProcessHits(G4Step *step, G4TouchableHistory*) {

	//remove the track entering the magnet vessel
	G4Track *track = step->GetTrack();
	G4StepPoint* prePoint = step->GetPreStepPoint();
	G4double energy = prePoint->GetTotalEnergy();
	if(fEnPrim==-9999)fEnPrim=energy;

	//track->SetTrackStatus(fKillTrackAndSecondaries);
	//	track->SetTrackStatus(fAlive);

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


void Shield::ClearEvent() {

	//G4cout << "BoxCal::ClearEvent" << G4endl;

	//clear event variables
	fEdep = 0;
	fEnPrim = -9999;
	fX = 9999.;
	fY = 9999.;
	fZ = 9999.;


}//ClearEvent

void Shield::CreateOutput(TTree *tree) {

	DetUtils u(fNam, tree);

	u.AddBranch("_EnPrim", &fEnPrim, "D");
	u.AddBranch("_en", &fEdep, "D");
	u.AddBranch("_x", &fX, "D");
	u.AddBranch("_y", &fY, "D");
	u.AddBranch("_z", &fZ, "D");


}//CreateOutput


































