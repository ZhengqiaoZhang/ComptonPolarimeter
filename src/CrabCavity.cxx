
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
#include "G4VisAttributes.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4RotationMatrix.hh"
#include "G4DormandPrince745.hh"

//local headers
#include "CrabCavity.h"


//_____________________________________________________________________________
CrabCavity::CrabCavity(G4String nam, G4double xpos,G4double ypos,G4double zpos,G4double length, G4double r1, G4double r2, G4double dout, G4double angle, G4LogicalVolume *top):
	Detector(), G4VSensitiveDetector(nam), fNam(nam) {

		//conical inner core
		//G4double length = 4.22367*meter;
		//G4double r1 = 0.09*meter;
		//G4double r2 = 0.09*meter;


		G4RotationMatrix* yRot = new G4RotationMatrix;
		//yRot->rotateY(-12.699*1e-3*rad);
		yRot->rotateY(-1.0*angle*rad);

		G4String nam_inner = fNam+"_inner";
		G4Cons *shape_inner = new G4Cons(nam_inner, 0, r2, 0, r1, length/2, 0, 360*deg);

		G4Material *mat_inner = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
		G4LogicalVolume *vol_inner = new G4LogicalVolume(shape_inner, mat_inner, nam_inner);
		vol_inner->SetVisAttributes( G4VisAttributes::GetInvisible() );

		//put the inner core to the top volume
		new G4PVPlacement(yRot, G4ThreeVector(xpos,ypos, zpos), vol_inner, nam_inner, top, false, 0);

		//cylindrical outer shape
		G4Tubs *shape_outer = new G4Tubs(fNam+"_outer", 0., dout*0.5, length/2-1e-4*meter, 0., 360.*deg);

		//magnet vessel around the inner magnetic core
		G4SubtractionSolid *shape_vessel = new G4SubtractionSolid(fNam, shape_outer, shape_inner);

		G4Material *mat_outer = G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");
		G4LogicalVolume *vol_vessel = new G4LogicalVolume(shape_vessel, mat_outer, fNam);

		//vessel visibility
		G4VisAttributes *vis_vessel = new G4VisAttributes();
		vis_vessel->SetColor(0.5, 0.5, 0.5); // grey
		vis_vessel->SetLineWidth(2);
		vis_vessel->SetForceSolid(true);
		//vis_vessel->SetForceAuxEdgeVisible(true);
		vol_vessel->SetVisAttributes(vis_vessel);

		//put the magnet vessel to the top volume
		new G4PVPlacement(yRot, G4ThreeVector(xpos, ypos, zpos), vol_vessel, fNam, top, false, 0);

	}//CrabCavity

//_____________________________________________________________________________
G4bool CrabCavity::ProcessHits(G4Step *step, G4TouchableHistory*) {

	//remove the track entering the magnet vessel
	G4Track *track = step->GetTrack();
	track->SetTrackStatus(fKillTrackAndSecondaries);

	return true;

}//ProcessHits



































