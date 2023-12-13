
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
#include "G4Torus.hh"
#include "G4SubtractionSolid.hh"
#include "G4RotationMatrix.hh"
#include "G4DormandPrince745.hh"
#include "G4EqMagElectricField.hh"
#include "G4TransportationManager.hh"

//local headers
#include "BeamMagnetDipoleTorus.h"


//_____________________________________________________________________________
BeamMagnetDipoleTorus::BeamMagnetDipoleTorus(G4String nam, G4double xpos,G4double ypos,G4double zpos,G4double length, G4double theta,G4double r1, G4double r2, G4double dout, G4double angle, G4double bfield, G4LogicalVolume *top):
	Detector(), G4VSensitiveDetector(nam), fNam(nam) {


		G4RotationMatrix* yRot = new G4RotationMatrix;
		yRot->rotateY(1.0*(M_PI-angle)*rad);
		yRot->rotateX(-1.0*M_PI*0.5*rad);
		
		G4int signOfTheta = (theta >= 0) ? 1 : -1;
		G4double DeltaTheta = 0;
		if(signOfTheta==-1)DeltaTheta=M_PI;

		G4double Rtor = length/abs(theta);
		G4double z0 = zpos - Rtor*sin(angle)*signOfTheta;
		G4double x0 = xpos + Rtor*cos(angle)*signOfTheta;
		G4cout<<"R="<<Rtor<<" z0="<<z0<<" x0="<<x0<<G4endl;
		G4cout<<"length="<<length<<" theta="<<theta<<" angle="<<angle<<G4endl;


		G4Torus *shape_1 = new G4Torus(fNam,r1,dout/2.0,Rtor,-0.5*theta*signOfTheta+DeltaTheta,abs(theta));


		G4Material *mat= G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");

		G4LogicalVolume *vol_logic = new G4LogicalVolume(shape_1,mat,fNam);

		G4Torus *shape_Bfield = new G4Torus ("Bfield_torus",0.*meter,r1-0.001*meter,Rtor,-0.5*theta*signOfTheta+DeltaTheta,abs(theta));
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

		vis_vessel->SetColor(0, 0, 1); // blue
		vis_vessel->SetLineWidth(2);
		vis_vessel->SetForceSolid(true);
		vol_logic->SetVisAttributes(vis_vessel);
		//              vol_logic2->SetVisAttributes(vis_vessel);
		//


		new G4PVPlacement(yRot, G4ThreeVector(x0,0*meter, z0), vol_logic,fNam, top, false, 0);




	}//BeamMagnetDipoleTorus

//_____________________________________________________________________________
G4bool BeamMagnetDipoleTorus::ProcessHits(G4Step *step, G4TouchableHistory*) {

	//remove the track entering the magnet vessel
	G4Track *track = step->GetTrack();
	track->SetTrackStatus(fKillTrackAndSecondaries);

	return true;

}//ProcessHits



































