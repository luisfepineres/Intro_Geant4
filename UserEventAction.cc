
// Geant4 Libraries
//
#include "G4UserEventAction.hh"
#include "G4Event.hh"

// Local Libraries
//
#include "UserEventAction.hh"


UserEventAction::UserEventAction()
{}


UserEventAction::~UserEventAction()
{}


void UserEventAction::BeginOfEventAction(const G4Event* event)
{
  G4cout << "Begin Event: " << event->GetEventID() << G4endl;
}


void UserEventAction::EndOfEventAction(const G4Event* event)
{
 G4cout << "End Event: " << event->GetEventID() << G4endl;

}
