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
#include "photonDet/photonDet_PbWO4.h"

//_____________________________________________________________________________
photonDet_PbWO4::photonDet_PbWO4(G4String nam, G4double xpos,G4double ypos,G4double zpos, G4double angle, G4LogicalVolume *top):
	Detector(), G4VSensitiveDetector(nam), fNam(nam) {

		//
		G4double density;
		G4double z,a;
		G4NistManager* man = G4NistManager::Instance();

		man->FindOrBuildMaterial("G4_PbWO4");
		new G4Material("Tungsten",    z=74., a= 183.85*g/mole, density= 19.30*g/cm3);

		auto defaultMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
		auto LayerMaterial_1 = G4NistManager::Instance()->FindOrBuildMaterial("Tungsten");
		auto LayerMaterial_2 = G4NistManager::Instance()->FindOrBuildMaterial("G4_PbWO4");




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
			= new G4Box("LayerX",           // its name
					10*cm/2, 2*cm/2,20*cm/2); // its size

		auto layerLV
			= new G4LogicalVolume(
					layerS,           // its solid
					defaultMaterial,  // its material
					"Layer");         // its name

		new G4PVReplica(
				"Layer",          // its name
				layerLV,          // its logical volume
				phDetLV,          // its mother
				kYAxis,           // axis of replication
				5,        // number of replica
				2*cm);  // witdth of replicafNam
					//
		auto layerS_XY
                        = new G4Box("LayerXY",           // its name
                                        2*cm/2, 2*cm/2,20*cm/2); // its size
								 //
		auto layerLV_XY
                        = new G4LogicalVolume(
                                        layerS_XY,           // its solid
                                        defaultMaterial,  // its material
                                        "layer_XY");         // its name
		 new G4PVReplica(
                                "Layer",          // its name
                                layerLV_XY,          // its logical volume
                                layerLV,          // its mother
                                kXAxis,           // axis of replication
                                5,        // number of replica
                                2*cm);  // witdth of replica


		G4Box* PbWO4_layer = new G4Box("PbWO4_layer", 2.*0.5*cm, 2*0.5*cm,20*0.5*cm); //PbWO4_layer
		G4LogicalVolume *vol_PbWO4_layer= new G4LogicalVolume(PbWO4_layer, LayerMaterial_2, fNam);
		G4VisAttributes *vis_PbWO4_layer = new G4VisAttributes();
		vis_PbWO4_layer->SetColor(1.0, 0, 0); // blue
		vis_PbWO4_layer->SetLineWidth(3);
		vis_PbWO4_layer->SetForceSolid(false);
		vol_PbWO4_layer->SetVisAttributes(vis_PbWO4_layer);




		new G4PVPlacement(0, G4ThreeVector(0, 0, 0), vol_PbWO4_layer, nam, layerLV_XY, false, 0);



//clear all event variables
  ClearEvent();


	}//photonDet_PbWO4

//_____________________________________________________________________________
G4bool photonDet_PbWO4::ProcessHits(G4Step *step, G4TouchableHistory*) {

	//remove the track entering the magnet vessel
	G4Track *track = step->GetTrack();
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


  //number of optical photons in the event from secondary tracks
  const std::vector<const G4Track*> *sec = step->GetSecondaryInCurrentStep();
  std::vector<const G4Track*>::const_iterator i;
  for(i = sec->begin(); i != sec->end(); i++) {
    if((*i)->GetParentID() <= 0) continue;

    //all optical photons
    if((*i)->GetDynamicParticle()->GetParticleDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) continue;
    fNphot++;

    //identify the process
/*    G4int ptype = (*i)->GetCreatorProcess()->GetProcessType();
    G4int pstype = (*i)->GetCreatorProcess()->GetProcessSubType();
    //scintillation photons
    if(ptype == fScinType && pstype == fScinSubType) fNscin++;
    //Cerenkov photons
    if(ptype == fCerenkovType && pstype == fCerenkovSubType) fNcerenkov++;  */

  }//secondary tracks loop

  return true;

}//ProcessHits


void photonDet_PbWO4::ClearEvent() {

  //G4cout << "BoxCal::ClearEvent" << G4endl;

  //clear event variables
  fEdep = 0;
  fX = 9999.;
  fY = 9999.;
  fZ = 9999.;

  fNphot = 0;
  fNscin = 0;
  fNcerenkov = 0;

}//ClearEvent

void photonDet_PbWO4::CreateOutput(TTree *tree) {

  DetUtils u(fNam, tree);

  u.AddBranch("_en", &fEdep, "D");
  u.AddBranch("_x", &fX, "D");
  u.AddBranch("_y", &fY, "D");
  u.AddBranch("_z", &fZ, "D");

  u.AddBranch("_nphot", &fNphot, "I");
  u.AddBranch("_nscin", &fNscin, "I");
  u.AddBranch("_ncerenkov", &fNcerenkov, "I");

}//CreateOutput


































