
#ifndef Shield_h
#define Shield_h

// beamline dipole magnet V2

#include "Detector.h"
#include "G4VSensitiveDetector.hh"

class Shield : public Detector, public G4VSensitiveDetector {

  public:

    Shield(G4String nam,G4String mat, G4double xpos,G4double ypos,G4double zpos, G4double angle,G4double xysiz_shield,G4double zsiz_shield, G4LogicalVolume *top);
    virtual ~Shield() {}

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

    double_t fEnPrim;
    double_t fEdep; // deposited energy in the detector
    double_t fX; // x of first point in the detector
    double_t fY; // y of first point
    double_t fZ; // z of first point


};

#endif

