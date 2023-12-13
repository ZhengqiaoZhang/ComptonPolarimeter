// MySteppingAction.h
#ifndef MYSTEPPINGACTION_H
#define MYSTEPPINGACTION_H 1

#include "G4UserSteppingAction.hh"
#include "TFile.h"
#include "TTree.h"

class MySteppingAction : public G4UserSteppingAction {
public:
    MySteppingAction();
    virtual ~MySteppingAction();

    virtual void UserSteppingAction(const G4Step*);

private:
    TFile* file;       // ROOT文件对象
    TTree* tree;       // ROOT树对象
    Double_t stepX, stepY, stepZ, stepE; // 用于存储每个step位置和能量的变量
};

#endif

