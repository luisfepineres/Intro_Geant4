
#include "PrimaryGeneratorMessenger.hh"

#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::
  PrimaryGeneratorMessenger(PrimaryGeneratorAction* Gun)
  : G4UImessenger(),
    fAction(Gun)
{
   G4cout << "...PrimaryGeneratorMessenger..." << G4endl;
  fGunDir = new G4UIdirectory("//gun/");
  fGunDir->SetGuidance("PrimaryGenerator control");

  fPolarCmd =
           new G4UIcmdWithADoubleAndUnit("//gun/optPhotonPolar",this);
  fPolarCmd->SetGuidance("Set linear polarization");
  fPolarCmd->SetGuidance("  angle w.r.t. (k,n) plane");
  fPolarCmd->SetParameterName("angle",true);
  fPolarCmd->SetUnitCategory("Angle");
  fPolarCmd->SetDefaultValue(-360.0);
  fPolarCmd->SetDefaultUnit("deg");
  fPolarCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete fPolarCmd;
  delete fGunDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{
  if( command == fPolarCmd ) {
      G4double angle = fPolarCmd->GetNewDoubleValue(newValue);
      if ( angle == -360.0*deg ) {
         fAction->SetOptPhotonPolar();
      } else {
         fAction->SetOptPhotonPolar(angle);
      }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
