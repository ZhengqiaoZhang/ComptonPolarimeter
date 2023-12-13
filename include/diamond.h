
#ifndef diamond_h
#define diamond_h

// beamline dipole magnet V2

#include "Detector.h"
#include "G4VSensitiveDetector.hh"

class diamond : public Detector, public G4VSensitiveDetector {

  public:

    diamond(G4String nam, G4double xpos,G4double ypos,G4double zpos,G4double angle,G4LogicalVolume*);
    virtual ~diamond() {}

    //Detector
    virtual const G4String& GetName() const {return fNam;}

    //G4VSensitiveDetector
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory*);

  private:

    G4String fNam; // magnet name

};

#endif

