
// Geant4 Libraries
// 
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"


// Local Libraries
// 
#include "SiPMHit.hh"


G4ThreadLocal G4Allocator<SiPMHit>* SiPMHitAllocator=0;


SiPMHit::SiPMHit()
  : waveLength(0.)
{}


SiPMHit::~SiPMHit()
{}

G4int SiPMHit::askDetected(G4double phoEner)
{

  waveLength = 1240. / (phoEner/(1.*eV));
  
  if(waveLength > 277. && waveLength <= 300.)
  {
    if(G4UniformRand() <= 0.13)
      return 1;
    else 
      return 0;
  }
  else if(waveLength > 300. && waveLength <= 350)
  {
    if(G4UniformRand() <= 0.18) 
      return 1;
    else
      return 0;
  }
  else if(waveLength > 350. && waveLength <= 400.)
  {
    if(G4UniformRand() <= 0.23) 
      return 1;
    else 
      return 0;
  }
  else if(waveLength > 400. && waveLength <= 450.)
  {
    if(G4UniformRand() <= 0.25)
      return 1;
    else
      return 0;
  }
  else if(waveLength > 450. && waveLength <= 500.)
  {
    if(G4UniformRand() <= 0.24) 
      return 1;
    else 
      return 0;
  }
  else if(waveLength > 500. && waveLength <= 550.)
  {
    if(G4UniformRand() <= 0.21)
      return 1;
    else
      return 0;
  }
  else if(waveLength > 550. && waveLength <= 600.)
  {
    if(G4UniformRand() <= 0.17)
      return 1;
    else 
      return
        0;
  }
  else if(waveLength > 600. && waveLength <= 650.)
  {
    if(G4UniformRand() <= 0.13)
      return 1;
    else 
      return 0;
  }
  else if(waveLength > 650. && waveLength <= 700.)
  {
    if(G4UniformRand() <= 0.09)
      return 1;
    else 
      return 0;
  }
  else if(waveLength > 700. && waveLength <= 750.)
  {
    if(G4UniformRand() <= 0.07)
      return 1;
    else 
      return 0;
  }
  else if(waveLength > 750. && waveLength <= 800.)
  {
    if(G4UniformRand() <= 0.05)
      return 1;
    else 
      return 0;
  }
  else if(waveLength > 800. && waveLength <= 850.)
  {
    if(G4UniformRand() <= 0.03)
      return 1;
    else 
      return 0;
  }
  else if(waveLength > 850. && waveLength <= 900.)
  {
    if(G4UniformRand() <= 0.02)
      return 1;
    else 
      return 0;
  }
else
   return 0;
}



