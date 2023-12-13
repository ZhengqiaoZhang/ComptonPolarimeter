
#ifndef BeamMagnetDipoleTorus_h
#define BeamMagnetDipoleTorus_h

// beamline dipole magnet V3

#include "Detector.h"
#include "G4VSensitiveDetector.hh"

class BeamMagnetDipoleTorus : public Detector, public G4VSensitiveDetector {

  public:
    BeamMagnetDipoleTorus(G4String nam, G4double xpos,G4double ypos,G4double zpos,G4double length,G4double theta, G4double r1, G4double r2, G4double dout, G4double angle, G4double bfield, G4LogicalVolume*);
    virtual ~BeamMagnetDipoleTorus() {}

    //Detector
    virtual const G4String& GetName() const {return fNam;}

    //G4VSensitiveDetector
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory*);

  private:

    G4String fNam; // magnet name

};

#endif

