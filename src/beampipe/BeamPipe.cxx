
//_____________________________________________________________________________
//
// beamline dipole magnet, version 2, intended first for B2eR
//
//_____________________________________________________________________________

//Geant headers
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4Cons.hh"
#include "G4SystemOfUnits.hh"
#include "G4PVPlacement.hh"
#include "G4FieldManager.hh"
#include "G4UniformMagField.hh"
#include "G4VisAttributes.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4RotationMatrix.hh"
#include "G4DormandPrince745.hh"
#include "G4EqMagElectricField.hh"
#include "G4TransportationManager.hh"
#include "G4Torus.hh"

//local headers
#include "beampipe/BeamPipe.h"


//_____________________________________________________________________________
BeamPipe::BeamPipe(G4String nam, G4String type, G4double xpos,G4double ypos,G4double zpos,G4double length, G4double pipeSize, G4double pipeThick, G4double angle, G4double theta,G4LogicalVolume *top):
	Detector(), G4VSensitiveDetector(nam), fNam(nam) {

		G4RotationMatrix* yRot = new G4RotationMatrix;
		//yRot->rotateY(-1.0*angle*rad);
		yRot->rotateX(3.1415926536*0.5*rad);

		G4double Rtor = length/theta;

		//3.1415926536

		G4Torus *shape = new G4Torus(fNam,pipeSize,pipeSize+pipeThick,Rtor,3.1415926536-2.12562536799637281,theta);


		G4Material *mat= G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
		G4LogicalVolume *vol_logic = new G4LogicalVolume(shape,mat,fNam);


		//put the inner core to the top volume

		G4VisAttributes *vis_vessel = new G4VisAttributes();
		vis_vessel->SetColor(0.5, 0.5, 0.5); // blue
		vis_vessel->SetLineWidth(2);
		vis_vessel->SetForceSolid(true);
		vol_logic->SetVisAttributes(vis_vessel);

		new G4PVPlacement(yRot, G4ThreeVector(xpos,0*meter, zpos), vol_logic,fNam, top, false, 0);


	}//BeamPipe

//_____________________________________________________________________________
G4bool BeamPipe::ProcessHits(G4Step *step, G4TouchableHistory*) {

	//remove the track entering the magnet vessel
	G4Track *track = step->GetTrack();
	track->SetTrackStatus(fKillTrackAndSecondaries);

	return true;

}//ProcessHits



































