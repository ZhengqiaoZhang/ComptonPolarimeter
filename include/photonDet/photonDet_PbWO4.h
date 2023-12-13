
#ifndef photonDet_PbWO4_h
#define photonDet_PbWO4_h

// beamline dipole magnet V2

#include "Detector.h"
#include "G4VSensitiveDetector.hh"

class photonDet_PbWO4 : public Detector, public G4VSensitiveDetector {

  public:

    photonDet_PbWO4(G4String nam, G4double xpos,G4double ypos,G4double zpos,G4double angle,G4LogicalVolume*);
    virtual ~photonDet_PbWO4() {}

    //Detector
    virtual const G4String& GetName() const {return fNam;}

    //G4VSensitiveDetector
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory*);


    //called via Detector
    virtual void CreateOutput(TTree *tree);
    virtual void ClearEvent();


  private:

    G4String fNam; // magnet name
    G4VPhysicalVolume *fSens; //detector sensitive volume

    void AddBranch(const std::string& nam, double_t *val, TTree *tree);

    double_t fEdep; // deposited energy in the detector
    double_t fX; // x of first point in the detector
    double_t fY; // y of first point
    double_t fZ; // z of first point

    double_t fNphot; // number of optical photons
    double_t fNscin; // scintillation photons
    double_t fNcerenkov; // Cerenkov photons

    G4int fScinType; // scintillation process type
    G4int fScinSubType; // scintillation process subtype
    G4int fCerenkovType; // Cerenkov process type
    G4int fCerenkovSubType; // Cerenkov process subtype

};

#endif

