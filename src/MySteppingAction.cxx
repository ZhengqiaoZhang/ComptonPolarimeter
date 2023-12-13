#include "MySteppingAction.h"
#include "G4Step.hh"

MySteppingAction::MySteppingAction() {
    // 创建ROOT文件和TTree
    file = new TFile("stepData.root", "RECREATE");
    tree = new TTree("StepTree", "Tree storing step points");

    // 将基础类型数组链接到TTree分支
    tree->Branch("stepX", &stepX, "stepX/D");
    tree->Branch("stepY", &stepY, "stepY/D");
    tree->Branch("stepZ", &stepZ, "stepZ/D");
    tree->Branch("stepE", &stepE, "stepE/D");
}

MySteppingAction::~MySteppingAction() {
    // 写入TTree到文件并关闭文件
    file->Write();
    file->Close();
    delete file;
}

void MySteppingAction::UserSteppingAction(const G4Step* step) {
    // 获取当前step的信息
    G4double stepLength = step->GetStepLength();
//    G4cout << "Step length: " << stepLength << G4endl;
    G4Track* track = step->GetTrack();
    stepX = track->GetPosition().x();
    stepY = track->GetPosition().y();
    stepZ = track->GetPosition().z();
    stepE = step->GetTotalEnergyDeposit();

    // 将数据填充到TTree
//    tree->Fill();
}

// 其他代码...

