
#ifndef photonDet_Lead_h
#define photonDet_Lead_h

// beamline dipole magnet V2

#include "Detector.h"
#include "G4VSensitiveDetector.hh"

class photonDet_Lead : public Detector, public G4VSensitiveDetector {

  public:

    photonDet_Lead(G4String nam, G4double xpos,G4double ypos,G4double zpos, G4double angle,G4double xysiz_lead,G4double zsiz_lead,G4double deltaD, G4LogicalVolume *top);
    virtual ~photonDet_Lead() {}

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

