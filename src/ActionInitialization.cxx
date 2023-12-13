
//_____________________________________________________________________________
//
// standard action initialization,
// selects the event generator
//_____________________________________________________________________________

//local headers
#include "ActionInitialization.h"
#include "GeneratorAction.h"
#include "EventAction.h"
#include "EventReader.h"
#include "RunAction.h"
#include "MySteppingAction.h"

//_____________________________________________________________________________
void ActionInitialization::Build() const {

  //select the generator
  //SetUserAction(new GeneratorAction);
  SetUserAction(new EventReader);
  SetUserAction(new EventAction);
  SetUserAction(new MySteppingAction);
  SetUserAction(new RunAction);

}//build

