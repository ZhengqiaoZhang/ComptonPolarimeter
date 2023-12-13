
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
#include "G4NistManager.hh"

//local headers
#include "photonDet/photonDet.h"

//_____________________________________________________________________________
photonDet::photonDet(G4String nam, G4double xpos,G4double ypos,G4double zpos, G4double angle, G4LogicalVolume *top):
	Detector(), G4VSensitiveDetector(nam), fNam(nam) {

		//
		G4double density;
		G4double z,a;
		G4NistManager* man = G4NistManager::Instance();

		man->FindOrBuildMaterial("G4_PbWO4");
		new G4Material("Tungsten",    z=74., a= 183.85*g/mole, density= 19.30*g/cm3);

		auto defaultMaterial = G4Material::GetMaterial("G4_Galactic");
		auto LayerMaterial_1 = G4Material::GetMaterial("Tungsten");
		auto LayerMaterial_2 = G4Material::GetMaterial("G4_PbWO4");




		G4RotationMatrix* yRot = new G4RotationMatrix;
		yRot->rotateY(-1.0*angle*rad);

		auto phDetS
			= new G4Box("Calorimeter",     // its name
					10*cm/2, 10*cm/2, 20*cm/2); // its size

		auto phDetLV
			= new G4LogicalVolume(
					phDetS,     // its solid
					defaultMaterial,  // its material
					"phDet");   // its name

		new G4PVPlacement(
				yRot,                // no rotation
				G4ThreeVector(xpos, ypos, zpos),
				phDetLV,          // its logical volume
				"phDet",    // its name
				top,          // its mother  volume
				false,            // no boolean operation
				0,                // copy number
				true);  // checking overlaps


		auto layerS
			= new G4Box("Layer",           // its name
					10*cm/2, 10*cm/2,1*cm/2); // its size

		auto layerLV
			= new G4LogicalVolume(
					layerS,           // its solid
					defaultMaterial,  // its material
					"Layer");         // its name

		new G4PVReplica(
				"Layer",          // its name
				layerLV,          // its logical volume
				phDetLV,          // its mother
				kZAxis,           // axis of replication
				20,        // number of replica
				1*cm);  // witdth of replica


		G4Box* W_layer = new G4Box("Tungsten_layer", 10.*0.5*cm, 10*0.5*cm, 0.62*0.5*cm); //Tungsten_layer

		G4LogicalVolume *vol_W_layer= new G4LogicalVolume(W_layer, LayerMaterial_1, fNam);
		G4VisAttributes *vis_W_layer = new G4VisAttributes();
		vis_W_layer->SetColor(1, 0, 0); // red
		vis_W_layer->SetLineWidth(2);
		vis_W_layer->SetForceSolid(false);
		vol_W_layer->SetVisAttributes(vis_W_layer);

		G4Box* PbWO4_layer = new G4Box("PbWO4_layer", 10.*0.5*cm, 10*0.5*cm,0.38*0.5*cm); //PbWO4_layer
		G4LogicalVolume *vol_PbWO4_layer= new G4LogicalVolume(PbWO4_layer, LayerMaterial_2, fNam+"_PbWO4_layer_vol");
		G4VisAttributes *vis_PbWO4_layer = new G4VisAttributes();
		vis_PbWO4_layer->SetColor(0, 0, 1); // blue
		vis_PbWO4_layer->SetLineWidth(2);
		vis_PbWO4_layer->SetForceSolid(false);
		vol_PbWO4_layer->SetVisAttributes(vis_PbWO4_layer);


		//layerLV->SetVisAttributes(vis_W_layer);

		//put the Layer vol to the Ecal volume
		new G4PVPlacement(0, G4ThreeVector(0, 0, -0.19*cm), vol_W_layer, fNam, layerLV, false, 0);


		new G4PVPlacement(0, G4ThreeVector(0, 0, 0.31*cm), vol_PbWO4_layer, fNam+"_PbWO4_vol", layerLV, false, 0);




	}//photonDet

//_____________________________________________________________________________
G4bool photonDet::ProcessHits(G4Step *step, G4TouchableHistory*) {

	//remove the track entering the magnet vessel
	G4Track *track = step->GetTrack();
	track->SetTrackStatus(fKillTrackAndSecondaries);

	return true;

}//ProcessHits



































