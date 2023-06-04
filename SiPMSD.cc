
// Geant4 Libraries
//
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "g4root.hh"
#include "G4SystemOfUnits.hh"


// Local Libraries
//
#include "SiPMSD.hh"
#include "SiPMHit.hh"
#include "DetectorConstruction.hh"
//#include "UserTrackInformation.hh"


// C++ Libraries
//
/*#include <locale>
#include <string>
#include <sstream>
#include <iomanip>*/


SiPMSD::SiPMSD()
  : phoHit(),
  pulseNumber(6),
  phoDetec(0.)
//  : G4VSensitiveDetector(name),
{
  G4cout << "...SiPMSD..." << G4endl;
  phoHit = new SiPMHit;
  kinEnergy = 0.;
  phoDetec = 0;
}


SiPMSD::~SiPMSD()
{}


G4int SiPMSD::ProcessHits(const G4Step* step, G4double time)
{
  kinEnergy = step->GetTrack()->GetTotalEnergy();
  phoDetec = phoHit->askDetected(kinEnergy);

  if (phoDetec)

    doPulse(time);//step->GetTrack(), time);

  return phoDetec;
}


void SiPMSD::resetSiPMSD()
{
  phoDetec = 0;
  kinEnergy = 0.;
}


void SiPMSD::doPulse(G4double time)//const G4Track* track, G4double time)
{
  G4AnalysisManager* analysisManager 
    = G4AnalysisManager::Instance();
 
  analysisManager->FillH1(0,(time/(1.*ns)));
  pulseNumber++;
}
