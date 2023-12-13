
#ifndef BeamPipeExitWin_h
#define BeamPipeExitWin_h

// simple calorimeter, V2

#include "Detector.h"
#include "G4VSensitiveDetector.hh"

class G4VPhysicalVolume;
class G4Step;

class BeamPipeExitWin : public Detector, public G4VSensitiveDetector {

	public:
		BeamPipeExitWin(G4String nam,G4double xpos, G4double ypos,G4double zpos, G4double xsize,G4double ysize, G4double pipeThick,G4double winsize, G4double angle,G4LogicalVolume *top);
		virtual ~BeamPipeExitWin() {}

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

		 Bool_t fIsHit; // hit by primary particle
    Double_t fEnPrim; // energy of the primary particle
    Double_t fEnAll; // sum of energy of all particles in event

    Double_t fHx; // hit position in x
    Double_t fHy; // hit position in y
    Double_t fHz; // hit position in z


};

#endif



















