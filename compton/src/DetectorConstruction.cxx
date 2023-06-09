
//_____________________________________________________________________________
//
// main detector construction,
// detector definition is here
//_____________________________________________________________________________

//C++
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <fstream>

//Geant
#include "G4GenericMessenger.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"

//local classes
#include "DetectorConstruction.h"
#include "RootOut.h"
#include "MCEvent.h"
#include "BeamMagnetDipole.h"
#include "BeamMagnetQuadrupole.h"
#include "electronDet.h"
#include "photonDet.h"

using namespace std;

//_____________________________________________________________________________
DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction(), fDet(0), fOut(0), fMsg(0), 
	fIncMagnet(0), fIncB2eR(0),fIncQF1(0) {

		G4cout << "DetectorConstruction::DetectorConstruction" << G4endl;

		//output file and tree
		fOut = new RootOut();

		//all detectors and their parts
		fDet = new std::vector<Detector*>;

		//MC event, also inherits from Detector
		fMC = new MCEvent();
		AddDetector(fMC);


		//messenger for detectors and components
		fMsg = new G4GenericMessenger(this, "/EPol/construct/");
		fMsg->DeclareProperty("B2eR", fIncB2eR);
		fMsg->DeclareProperty("QF1", fIncQF1);

	}//DetectorConstruction

//_____________________________________________________________________________
DetectorConstruction::~DetectorConstruction() {

	//write the tree and close output file
	fOut->Close();

	delete fDet;

}//~DetectorConstruction

//_____________________________________________________________________________
G4VPhysicalVolume* DetectorConstruction::Construct() {

	G4cout << G4endl << "DetectorConstruction::Construct" << G4endl;

	//vacuum top material
	G4Material* top_m = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

	//top world volume
	G4Box *top_s = new G4Box("top_s", 20*meter, 20*meter, 200*meter);
	//G4Box *top_s = new G4Box("top_s", 3*m, 3*m, 3*m);
	G4LogicalVolume *top_l = new G4LogicalVolume(top_s, top_m, "top_l");
	top_l->SetVisAttributes( G4VisAttributes::GetInvisible() );
	G4VPhysicalVolume *top_p = new G4PVPlacement(0, G4ThreeVector(), top_l, "top_p", 0, false, 0);




	//detectors

	//beamline magnet
	//if(fIncB2eR) new BeamMagnet(-12.254*meter, top_l);
	ifstream magfile;
	magfile.open("../polarimeter_magnet.dat");

	string line;


	if(magfile.fail()) {
		G4cout << "eee ****************"<<endl;
		string description = "Can't open mag file";
		G4Exception("EventReader::OpenInput", "InputNotOpen01", FatalException, description.c_str());
	}

	string cc1,cc2;

	G4double xpos,ypos,zpos,length,r1,r2,dout,angle,bfield,gradient;

	int number = 0;

	while(!magfile.eof()){
		getline(magfile,line);
		if(number>7)continue;

		//if( line.find("DB") != string::npos ) {
		stringstream ss(line);
		ss>>cc1;
		ss>>cc2;
		ss>>xpos;
		ss>>ypos;
		ss>>zpos;
		ss>>r1;
		ss>>r2;
		ss>>dout;
		ss>>length;
		ss>>angle;
		ss>>bfield;
		ss>>gradient;
		G4cout << "test= ****************"<<endl;
		G4cout << "read mag file" <<xpos<<" "<<ypos<<" "<<zpos<<" "<<length<<" "<<r1<<" "<<r2<<" "<<dout<<" "<<angle<<" "<<bfield<<" "<<endl;
		if(cc1=="DB")AddDetector(new BeamMagnetDipole(cc2,xpos*meter,ypos*meter,zpos*meter,length*meter,r1*meter,r2*meter,dout*meter,angle,bfield*tesla, top_l));//add Dipole
		if(cc1=="QF"){
			G4cout << "read mag file" <<xpos<<" "<<ypos<<" "<<zpos<<" "<<length<<" "<<r1<<" "<<r2<<" "<<dout<<" "<<angle<<" "<<bfield<<" "<<endl;
			AddDetector(new BeamMagnetQuadrupole(cc2,xpos*meter,ypos*meter,zpos*meter,length*meter,r1*meter,r2*meter,dout*meter,angle,gradient*tesla/meter, top_l));} //add Quadrupole
		number++;
		//}
	}
	AddDetector(new electronDet("Edet",328.737,0,-24500,0,top_l)); //add electron detector
	AddDetector(new photonDet("Gdet",-493.57,0,-44269.63,0,top_l)); //add photon detector


	G4cout << "test= &&&&&&&&"<<endl;

	// if(fIncB2eR) AddDetector(new BeamMagnetDipole("B2eR",0.238873*meter,0.0*meter, -6.6488*meter,4.22367*meter,0.1*meter,0.1*meter,0.5*meter,-12.699,-0.272916*tesla, top_l));
	//  if(fIncQF1) AddDetector(new BeamMagnetQuadrupole("QF1", -9.5956*meter, top_l));
	// if(fIncQF1) AddDetector(new BeamMagnetQuadrupole("QF2", -19.5956*meter, top_l));


	return top_p;

}//Construct

//_____________________________________________________________________________
void DetectorConstruction::BeginEvent(const G4Event *evt) const {

	//detector loop for  ClearEvent  in each detector
	std::for_each(fDet->begin(), fDet->end(), std::mem_fun( &Detector::ClearEvent ));

	//set MC
	fMC->BeginEvent(evt);

}//BeginEvent

//_____________________________________________________________________________
void DetectorConstruction::FinishEvent() const {

	//detector loop
	std::for_each(fDet->begin(), fDet->end(), std::mem_fun( &Detector::FinishEvent ));

	//fill the output tree
	fOut->FillTree();

}//WriteEvent

//_____________________________________________________________________________
void DetectorConstruction::AddDetector(Detector *det) {

	//add detector to all detectors
	det->Add(fDet);

}//AddDetector

//_____________________________________________________________________________
void DetectorConstruction::CreateOutput() const {

	//open output file
	fOut->Open();

	//detector loop to call CreateOutput
	std::vector<Detector*>::iterator i = fDet->begin();
	while(i != fDet->end()) {
		(*i++)->CreateOutput( fOut->GetTree() );
	}//detector loop

}//CreateOutput

//_____________________________________________________________________________
void DetectorConstruction::ConstructSDandField() {

	G4cout << "DetectorConstruction::ConstructSDandField" << G4endl;

	//detector loop
	std::vector<Detector*>::iterator i;
	for(i = fDet->begin(); i != fDet->end(); ++i) {
		Detector *det = *i;

		G4VSensitiveDetector *sd = dynamic_cast<G4VSensitiveDetector*>(det);
		if(!sd) continue;

		//detector inherits also from G4VSensitiveDetector, add it to Geant

		G4SDManager::GetSDMpointer()->AddNewDetector(sd);
		SetSensitiveDetector(det->GetName(), sd);

		G4cout << "  " << det->GetName() << G4endl;
	}//detector loop

}//ConstructSDandField



















