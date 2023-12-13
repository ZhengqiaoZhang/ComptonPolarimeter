
//_____________________________________________________________________________
//
// beamline Quadrupole magnet
//
//_____________________________________________________________________________

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

//local headers
#include "diamond.h"

//_____________________________________________________________________________
diamond::diamond(G4String nam, G4double xpos,G4double ypos,G4double zpos, G4double angle, G4LogicalVolume *top):
	Detector(), G4VSensitiveDetector(nam), fNam(nam) {

		//conical inner core
		//
		auto defaultMaterial = G4Material::GetMaterial("G4_Galactic");



		G4RotationMatrix* yRot = new G4RotationMatrix;
		yRot->rotateY(-1.0*angle*rad);
		G4RotationMatrix* Rot = new G4RotationMatrix;

		auto edetS
			= new G4Box("Calorimeter",     // its name
					15*cm/2, 5*cm/2, 12.5*cm/2); // its size

		auto edetLV
			= new G4LogicalVolume(
					edetS,     // its solid
					defaultMaterial,  // its material
					"edet");   // its name

		new G4PVPlacement(
				yRot,                // no rotation
				G4ThreeVector(xpos, ypos, zpos),
				edetLV,          // its logical volume
				"edet",    // its name
				top,          // its mother  volume
				false,            // no boolean operation
				0,                // copy number
				true);  // checking overlaps


		auto layerS
			= new G4Box("Layer",           // its name
					15*cm/2, 5*cm/2,1*cm/2); // its size

		auto layerLV
			= new G4LogicalVolume(
					layerS,           // its solid
					defaultMaterial,  // its material
					"Layer");         // its name

		new G4PVReplica(
				"Layer",          // its name
				layerLV,          // its logical volume
				edetLV,          // its mother
				kZAxis,           // axis of replication
				5,        // number of replica
				2.5*cm);  // witdth of replica



		G4String nam_Al_frame = fNam+"_Al_frame";
		G4String nam_Al_f_subtract_main = fNam+"_Al_f_subtract_main";
		G4String nam_Al_f_subtract_1 = fNam+"_Al_f_subtract_1";

		G4Box* Al_frame = new G4Box(nam_Al_frame, 15.*0.5*cm, 5.*0.5*cm, 0.5*0.5*cm); //Al frame size
		G4Box* Al_frame_sub1 = new G4Box(fNam+"_Al_frame_sub1",12.002*0.5*cm, 3.*0.5*cm, 0.501*0.5*cm); //Al frame subtraction 1
		G4Box* Al_frame_sub2  = new G4Box(fNam+"_Al_frame_sub2",12.*0.5*cm, 3.5*0.5*cm, 0.1*0.5*cm);  //Al frame subtraction 2 due to the PCB

		G4Material *mat_Al_frame = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");

		G4SubtractionSolid *shape_Al_frame_1 = new G4SubtractionSolid(fNam+"_Al_frame_1", Al_frame, Al_frame_sub1, 0,G4ThreeVector(-1.5001*cm,0,0)); //set to -1.5001 only for vistrualization!!

		G4SubtractionSolid *shape_Al_frame_2 = new G4SubtractionSolid(fNam+"_Al_frame_2",shape_Al_frame_1,Al_frame_sub2,Rot,G4ThreeVector(-1.0*cm,0,0));

		G4LogicalVolume *vol_Al_frame= new G4LogicalVolume(shape_Al_frame_2, mat_Al_frame, fNam+"_Al_frame_vol");
		G4VisAttributes *vis_Al_frame = new G4VisAttributes();
		vis_Al_frame->SetColor(1, 0, 0); // red
		vis_Al_frame->SetLineWidth(2);
		vis_Al_frame->SetForceSolid(false);
		vol_Al_frame->SetVisAttributes(vis_Al_frame);

		G4Box* PCB_frame = new G4Box(fNam+"_PCB_frame", 12*0.5*cm, 3.5*0.5*cm, 0.1*0.5*cm); 
		G4Material *mat_PCB_frame = G4NistManager::Instance()->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");


		G4Box* PCB_frame_sub1 = new G4Box(fNam+"_PCB_frame_sub1",10.*0.5*cm,1.0*0.5*cm,0.101*0.5*cm); //set to 0.101 for vistrualization!! this does affect the actual gemotry 

		G4SubtractionSolid *shape_PCB_frame = new G4SubtractionSolid(fNam+"_PCB_frame_1",PCB_frame,PCB_frame_sub1,0,G4ThreeVector(-1.00*cm,0,0));  //check

		G4LogicalVolume *vol_PCB = new G4LogicalVolume(shape_PCB_frame,mat_PCB_frame,fNam+"_PCB_frame_vol");
		G4VisAttributes *vis_PCB = new G4VisAttributes();
		vis_PCB->SetColor(0, 1, 0); // Green
		vis_PCB->SetLineWidth(2);
		vis_PCB->SetForceSolid(true);
		vol_PCB->SetVisAttributes(vis_PCB);

		G4Box* Si_frame = new G4Box(fNam+"_Si_frame",10.0*0.5*cm,1.0*0.5*cm,0.05*0.5*cm);
		G4Material *mat_Si_frame  = G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");

		G4LogicalVolume *vol_Si = new G4LogicalVolume(Si_frame,mat_Si_frame,fNam);
		G4VisAttributes *vis_Si = new G4VisAttributes();
		vis_Si->SetColor(0.45, 0.35, 0); // Brown
		vis_Si->SetLineWidth(2);
		vis_Si->SetForceSolid(true);
		vol_Si->SetVisAttributes(vis_Si);



		G4Box* Al_frame_side = new G4Box(fNam+"_Al_frame_side", 15.*0.5*cm, 5.*0.5*cm, 2.0*0.5*cm); //Al frame size
                G4Box* Al_frame_side_sub = new G4Box(fNam+"_Al_frame_side_sub",14.502*0.5*cm, 4.*0.5*cm, 2.001*0.5*cm); //Al frame subtraction 1  for vistrualization thickness set to 2.001 (will affect the gemotry by 0.0005), the actual value should be 2.0 !!! 


                G4SubtractionSolid *shape_Al_frame_side = new G4SubtractionSolid(fNam+"_Al_frame_Side", Al_frame_side, Al_frame_side_sub, 0,G4ThreeVector(-0.251*cm,0,0)); //set to -0.251 only for vistrualization!!


                G4LogicalVolume *vol_Al_frame_side= new G4LogicalVolume(shape_Al_frame_side, mat_Al_frame, fNam+"_Al_frame_side_vol");
                G4VisAttributes *vis_Al_frame_side = new G4VisAttributes();
                vis_Al_frame_side->SetColor(1, 0, 0); // red
                vis_Al_frame_side->SetLineWidth(2);
                vis_Al_frame_side->SetForceSolid(false);
                vol_Al_frame_side->SetVisAttributes(vis_Al_frame_side);


		G4Box* shape_actuator_rod = new G4Box(fNam+"_actuator_rod", 5.*0.5*cm, 3.*0.5*cm, 3.0*0.5*cm);  //actuator_rod size

		G4LogicalVolume *vol_actuator_rod= new G4LogicalVolume(shape_actuator_rod, mat_Al_frame, fNam+"_actuator_rod_vol");
		G4VisAttributes *vis_actuator_rod = new G4VisAttributes();
                vis_actuator_rod->SetColor(1, 1, 1); // red
                vis_actuator_rod->SetLineWidth(2);
                vis_actuator_rod->SetForceSolid(true);
                vol_actuator_rod->SetVisAttributes(vis_actuator_rod);






		








		//put the magnet vessel to the top volume
		new G4PVPlacement(0, G4ThreeVector(0, 0, 0), vol_Al_frame, fNam+"_Al_frame_vol", layerLV, false, 0);

		new G4PVPlacement(0, G4ThreeVector(-1.0*cm, 0, 0), vol_PCB, fNam+"_PCB_frame_vol", layerLV, false, 0);

		new G4PVPlacement(0, G4ThreeVector(-2.0*cm, 0, 0), vol_Si, fNam, layerLV, false, 0);

		new G4PVPlacement(0, G4ThreeVector(0, 0, +1.25*cm), vol_Al_frame_side, fNam+"_Al_frame_side_vol", layerLV, false, 0);


		new G4PVPlacement(0, G4ThreeVector(xpos+10.0*cm*cos(angle), 0, zpos-10.0*cm*sin(angle)), vol_actuator_rod, fNam+"_vol_actuator_rod", top, false, 0);  //place on top vol
		//


	}//diamond

//_____________________________________________________________________________
G4bool diamond::ProcessHits(G4Step *step, G4TouchableHistory*) {

	//remove the track entering the magnet vessel
	G4Track *track = step->GetTrack();
	track->SetTrackStatus(fKillTrackAndSecondaries);

	return true;

}//ProcessHits



































