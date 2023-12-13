
#ifndef photonDet_ver2_h
#define photonDet_ver2_h

// beamline dipole magnet V2

#include "Detector.h"
#include "G4VSensitiveDetector.hh"

class photonDet_ver2 : public Detector, public G4VSensitiveDetector {

  public:

    photonDet_ver2(G4String nam, G4double xpos,G4double ypos,G4double zpos,G4double angle,G4LogicalVolume*);
    virtual ~photonDet_ver2() {}

    //Detector
    virtual const G4String& GetName() const {return fNam;}

    //G4VSensitiveDetector
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory*);

  private:

    G4String fNam; // magnet name

};

#endif

