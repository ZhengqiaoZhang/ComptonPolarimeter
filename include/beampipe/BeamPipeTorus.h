
#ifndef BeamPipeTorus_h
#define BeamPipeTorus_h

// simple calorimeter, V2

#include "Detector.h"
#include "G4VSensitiveDetector.hh"

class G4VPhysicalVolume;
class G4Step;

class BeamPipeTorus : public Detector, public G4VSensitiveDetector {

	public:
		BeamPipeTorus(G4String nam,  G4double xpos,G4double ypos,G4double zpos,G4double length, G4double pipeSizeX,G4double pipeSizeY, G4double pipeThick, G4double angle, G4double theta,G4double bfield,G4LogicalVolume *top);
		virtual ~BeamPipeTorus() {}

		//called via G4VSensitiveDetector
		virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory*);

		//called via Detector
		virtual const G4String& GetName() const {return fNam;}
		virtual void CreateOutput(TTree *tree);
		virtual void ClearEvent();

	private:

		G4String fNam; // detector name

		Bool_t fSelectPrim; // flag to select only primary track in ProcessHits
				    //
		int index;

		int fPDG[10];

		Bool_t fIsHit[10]; // hit by primary particle
		Double_t fEnPrim[10]; // energy of the primary particle
		Double_t fEnAll; // sum of energy of all particles in event

		Double_t fHx[10]; // hit position in x
		Double_t fHy[10]; // hit position in y
		Double_t fHz[10]; // hit position in z

		int ntrack;
};

#endif



















