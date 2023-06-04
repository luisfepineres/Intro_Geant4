
// Geant4 Libraries
//
#include "G4Timer.hh"
#include "G4Run.hh"
#include "g4root.hh"
#include "G4SystemOfUnits.hh"

// Local Libraries
//
#include "RunAction.hh"


RunAction::RunAction()
 : G4UserRunAction(),
   fTimer(0)
{
  fTimer = new G4Timer;

	G4AnalysisManager* analysisManager 
      = G4AnalysisManager::Instance();

	analysisManager->SetVerboseLevel(1);
	
  
 
//analysisManager->CreateH1("photoe_time","Photoelectrons vs time", 150, 0., 150.);
analysisManager->CreateH1("scint_photons","Scintillation photons", 50000, 0., 50000.);
//analysisManager->CreateH1("fiber_photons","Photons in the fiber", 5000000, 0., 5000000.);
analysisManager->CreateH1("sipm_photons","Photons in the sipm", 10000, 0., 10000.);
analysisManager->CreateH1("photo_elec","Photoelectrons", 250, 0., 250.);
//analysisManager->CreateH1("fiber_absorption","Atenuation in the fiber",50, 0., 5000.);
//analysisManager->CreateH1("scint_absorption","Atenuation in the scint",500, 0., 500.);
//analysisManager->CreateH1("scint_photons_wavelength","Wavelength of scint photons", 600, 200., 800.);

  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete fTimer;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  fTimer->Start();

  G4AnalysisManager* analysisManager
     = G4AnalysisManager::Instance();

  G4String rootFile = "scintbar";
  analysisManager->OpenFile(rootFile);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
  fTimer->Stop();
  G4cout << "number of event = " << aRun->GetNumberOfEvent()
         << " " << *fTimer << G4endl;

  G4AnalysisManager* analysisManager
     = G4AnalysisManager::Instance();
  analysisManager->Write(); 
  analysisManager->CloseFile();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
