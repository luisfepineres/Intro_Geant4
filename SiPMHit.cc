// Geant4 Libraries and  Local Libraries 
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "SiPMHit.hh"

G4ThreadLocal G4Allocator<SiPMHit>* SiPMHitAllocator=0;

SiPMHit::SiPMHit()
  : waveLength(0.)
{}

SiPMHit::~SiPMHit()
{}

 // Simple function for Silicon PMT Hit
G4int SiPMHit::askDetected(G4double phoEner)
{ 
  //To define the waveLength giving the PhotoEnergy
  waveLength = 1240. / (phoEner/(1.*eV));
  // An array with three elements : {Initial Wavelength, Final Wavelength, Probability}
  const G4double ranges[][3] = {
      {277., 300., 0.13},
      {300., 350., 0.18},
      {350., 400., 0.23},
      {400., 450., 0.25},
      {450., 500., 0.24},
      {500., 550., 0.21},
      {550., 600., 0.17},
      {600., 650., 0.13},
      {650., 700., 0.09},
      {700., 750., 0.07},
      {750., 800., 0.05},
      {800., 850., 0.03},
      {850., 900., 0.02}
  };
  
  const size_t numRanges = sizeof(ranges) / sizeof(ranges[0]);
  for (size_t i = 0; i < numRanges; ++i)
  {
    if (waveLength > ranges[i][0] && waveLength <= ranges[i][1])
    {
      if (G4UniformRand() <= ranges[i][2])
        return 1;
      else
        return 0;
    }
  }
  
  return 0;
}
