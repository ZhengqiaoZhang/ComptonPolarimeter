
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
#include "G4GDMLParser.hh"
#include "G4Material.hh"
#include "G4TransportationManager.hh"
#include "G4UserLimits.hh"

//local classes
#include "DetectorConstruction.h"
#include "RootOut.h"
#include "MCEvent.h"
#include "BeamMagnetDipole.h"
#include "BeamMagnetDipoleTorus.h"
#include "BeamMagnetQuadrupole.h"
#include "BeamMagnetQuadrupole_cone.h"
#include "beampipe/BeamPipeEllipse_Quad.h"
#include "beampipe/BeamPipeEllipse_Quad5.h"
#include "beampipe/BeamPipeEllipse_Dipole.h"
#include "beampipe/BeamPipeEllipseCone.h"
#include "beampipe/BeamPipeHalfEllipse.h"
#include "beampipe/BeamPipeHalfEllipseCone.h"
#include "beampipe/BeamPipeTorus.h"
#include "CrabCavity.h"
#include "electronDet.h"
#include "Shield.h"
#include "Collimator.h"
#include "photonDet/photonDet.h"
#include "photonDet/photonDet_ver2.h"
#include "photonDet/photonDet_PbWO4.h"
#include "photonDet/photonDet_Lead.h"
#include "photonDet/photonDet_Si.h"
#include "calBox.h"
#include "calTubs.h"
#include "ExitWin.h"
#include "diamond.h"
#include "beampipe/BeamPipeEllipse.h"
#include "beampipe/BeamPipeEllipseAsym.h"
#include "beampipe/BeamPipeEllipseAsym_WinDesign.h"
#include "beampipe/BeamPipeExitWin.h"

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
	G4Box *top_s = new G4Box("top_s", 2000*meter, 20*meter, 200*meter);
	//G4Box *top_s = new G4Box("top_s", 3*m, 3*m, 3*m);
	G4LogicalVolume *top_l = new G4LogicalVolume(top_s, top_m, "top_l");
	top_l->SetVisAttributes( G4VisAttributes::GetInvisible() );
	G4VPhysicalVolume *top_p = new G4PVPlacement(0, G4ThreeVector(), top_l, "top_p", 0, false, 0);

	G4double Q7_bfield,Q6_bfield,Q5_bfield,Q4_bfield,Q3_bfield,D2EF_bfield;

	int add_air =1.0;

	double energy_ratio = 1.0;//10/17.846262;
				  //
	double shift = 0.0;//589.0;




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

	G4double xpos,ypos,zpos,length,theta,r1,r2,dout,angle,bfield,gradient;

	int number = 0;
	while(!magfile.eof()){
		getline(magfile,line);
		//if(number>12)continue;
		if(line=="")continue;
		number++;

		if(number<3||number>35)continue; //for the study of beam pipe

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
		//		ss>>theta;
		ss>>angle;
		ss>>bfield;
		ss>>gradient;

		if(cc2=="\"D6EF_6_4\""||cc2=="\"D6EF_6_3\""||cc2=="\"D6EF_6_2\""||cc2=="\"D6EF_6_1\"")theta=0.01189657362;
		if(cc2=="\"D5EF_6\""||cc2=="\"D4EF_6\"")theta=0.0015;
		if(cc2=="\"D3EF_6\"")theta=0.013;
		if(cc2=="\"D2EF_6_2\""||cc2=="\"D2EF_6_1\"")theta=-0.0116506;
		if(cc2=="\"D1EF_6\"")theta=-0.0015; //hard coded here, may input theta in the inputfile later

		G4cout << "test= ****************"<<endl;
		G4cout << "read mag file" <<" "<<cc2<<" "<<xpos<<" "<<ypos<<" "<<zpos<<" "<<length<<" "<<theta<<" "<<r1<<" "<<r2<<" "<<dout<<" "<<angle<<" "<<bfield<<" "<<gradient<<" "<<endl;


		if(cc1=="DB")AddDetector(new BeamMagnetDipoleTorus(cc2,(xpos+shift)*meter,ypos*meter,zpos*meter,length*meter,theta,r1*meter,r2*meter,dout*meter,angle,bfield*energy_ratio*tesla, top_l));//add Dipole

		if((cc1=="QF"||cc1=="QD"))AddDetector(new BeamMagnetQuadrupole(cc2,(xpos+shift)*meter,ypos*meter,zpos*meter,length*meter,r1*meter,r2*meter,dout*meter,angle,gradient*energy_ratio*tesla/meter, 1,0,0,top_l));
		if(cc1=="CRAB")AddDetector(new CrabCavity(cc2,(xpos+shift)*meter,ypos*meter,zpos*meter,length*meter,r1*meter,r2*meter,dout*meter,angle,top_l));


	}



	/*
	 * AddDetector(new ExitWin("ExitWin",(-590.05091+shift)*meter,0*meter,60.846674*meter,0.1*meter,0.001*meter,3.14389,"G4_Be",0.,1.0,1.0,top_l)); //exit window 
	 AddDetector(new Collimator("collimator",(-590.03712+shift)*meter,0*meter,54.846690*meter,0.025*meter,0.6*meter,0.35*meter,3.14389,"G4_Fe",1.0,1.0,1.0,top_l)); //collimater
	 AddDetector(new Shield("Shield","G4_W",(-590.01874+shift)*meter,0,46.846711*meter,3.14389,30*cm,0.2*mm,top_l)); //W shield
	*/


	//photon detector
	//
	AddDetector(new calBox("phoDet",(-5.900141496e+02+shift)*meter,0*meter,4.4846716e+01*meter,20*cm,10*cm,3.14389,1.0,1.0,1.0,top_l)); //photon detector

	//test beam line
	//AddDetector(new calBox("elecDet", -589.56994564337*meter,0*meter,38.300141312954*meter,135*cm,1*cm,3.1584,1.0,1.0,1.0,top_l));
	//AddDetector(new calBox("Det_after_SQ6",  -589.86148*meter,0*meter,55.644245*meter,135*cm,1*cm,3.15839,1.0,1.0,1.0,top_l)); //elec det after SQ6
	//AddDetector(new calBox("Det_after_SQ6",  -589.94748*meter,0*meter,60.769545*meter,135*cm,1*cm,3.15839,1.0,1.0,1.0,top_l)); //elec det after SQ7
	//AddDetector(new calBox("Det_after_SQ6",  -589.91064*meter,0*meter,58.5948285*meter,135*cm,1*cm,3.15839,1.0,1.0,1.0,top_l)); //elec det after SQ7


	// electron detectors	
	AddDetector(new calBox("Det_after_SQ7",-589.85446*meter,0*meter,60.771114*meter,15.0*cm,1*cm,3.1583938535898,1.0,1.0,1.0,top_l)); //elec det after SQ7
	AddDetector(new calBox("Det_after_SQ6",-589.76835*meter,0*meter,55.645838*meter,15.0*cm,1*cm,3.1583938535898,1.0,1.0,1.0,top_l)); //elec det after SQ6


	//AddDetector(new diamond("Edet",394.59006,0,62381.122,3.14080831030455609E+000,top_l));  //electron detector of the diamond type
	//extract the GDML file
	//		AddDetector(new photonDet_ver3("Gdet",8.75043390000000045e+02,0,42168.933,3.12632159689653655E+000,top_l));
	//G4GDMLParser parser;
	//	parser.Write("output.gdml", top_l);
	//
	if (top_p->CheckOverlaps())
		G4cout << "Overlaps detected!" << G4endl;
	else
		G4cout << "No overlaps detected." << G4endl;





	return top_p;

}//Construct

//_____________________________________________________________________________
void DetectorConstruction::BeginEvent(const G4Event *evt) const {

	//detector loop for  ClearEvent  in each detector
	std::for_each(fDet->begin(), fDet->end(), std::mem_fn( &Detector::ClearEvent ));

	//set MC
	fMC->BeginEvent(evt);

}//BeginEvent

//_____________________________________________________________________________
void DetectorConstruction::FinishEvent() const {

	//detector loop
	std::for_each(fDet->begin(), fDet->end(), std::mem_fn( &Detector::FinishEvent ));

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



















