#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"

// Local Libraries
#include "DetectorConstruction.hh"
#include "SiPMSD.hh"



DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction()
{
  G4cout << "...DetectorConstruction..." << G4endl;

  fBarLength        = 600.0*mm; 
  fBarBase          = 5.0*mm; 
  fHoleRadius       = 0.9*mm;
  fHoleLength       = 599.75*mm;
  fCoatingThickness = 0.25*mm;
  fSurfaceRoughness = 1; 

  fExtrusionPolish = 1.; 
  fExtrusionReflectivity = 1.; 		
 
  fXYRatio = 1.0; 

  fWLSfiberZ     = fHoleLength-2.5*mm; 
  fWLSfiberRY  = 0.5*mm;
  fWLSfiberOrigin = -2.5*mm; 
 
  fWLSfiberRX  = fXYRatio * fWLSfiberRY;

  fClad1RX = fWLSfiberRX + 0.03*fWLSfiberRX;
  fClad1RY = fWLSfiberRY + 0.03*fWLSfiberRY;
  fClad1Z  = fWLSfiberZ;

  fClad2RX = fClad1RX + 0.03*fWLSfiberRX;
  fClad2RY = fClad1RY + 0.03*fWLSfiberRY;
  fClad2Z  = fWLSfiberZ;
  
  fWorldSizeX = 150.0*cm;
  fWorldSizeY = 150.*cm;
  fWorldSizeZ = 150.*cm;
  
  O = H = C = Ti = NULL;

 TiO2  = Pethylene = FPethylene = Polystyrene = PMMA = NULL;

 SetCouple = true;

}


DetectorConstruction::~DetectorConstruction()
{}


// ****************************
// Doing Elements and Materials
// ****************************

void DetectorConstruction::DefineMaterials()
{

  // ----------------
  // *** Elements ***
  // ----------------
  G4double a, z, density, fractionmass;

  //N = new G4Element("Nitrogen", "N", z = 7 , a = 14.01*g/mole);
  C = new G4Element("Carbon"  , "C", z = 6 , a = 12.01*g/mole);
  O = new G4Element("Oxygen"  , "O", z = 8 , a = 16.00*g/mole);
  H = new G4Element("Hydrogen", "H", z=1 , a = 1.01*g/mole);
  Ti = new G4Element("Titanium", "Ti", z=22 , a = 47.867*g/mole);


  // -----------------
  // *** Materials ***
  // -----------------
  G4NistManager* nist = G4NistManager::Instance();
  

  // *** Air ***
  Air = nist->FindOrBuildMaterial("G4_AIR");
  
  // *** Aluminio ***
  Aluminio = nist->FindOrBuildMaterial("G4_Al");
  

  //--------------------------------------------------
  // WLSfiber PMMA
  //--------------------------------------------------
  PMMA = new G4Material("PMMA", density= 1.190*g/cm3, 3);
  PMMA->AddElement(C, 5);
  PMMA->AddElement(H, 8);
  PMMA->AddElement(O, 2);


  //--------------------------------------------------
  // Cladding (polyethylene)
  //--------------------------------------------------
  Pethylene= new G4Material("Pethylene", density= 1.200*g/cm3, 2);
  Pethylene->AddElement(C, 2);
  Pethylene->AddElement(H, 4);


  //--------------------------------------------------
  // Double Cladding (fluorinated polyethylene) 
  //--------------------------------------------------
  FPethylene= new G4Material("FPethylene", density= 1.400*g/cm3, 2);
  FPethylene->AddElement(C, 2);
  FPethylene->AddElement(H, 4);


  //--------------------------------------------------
  // Polystyrene Material base del centellador
  //--------------------------------------------------
  Polystyrene = new G4Material("Polystyrene", density= 1.050*g/cm3, 2);
  Polystyrene->AddElement(C, 8);
  Polystyrene->AddElement(H, 8);


  //--------------------------------------------------
  // TiO2 
  //--------------------------------------------------
  TiO2 = new G4Material("TiO2", density= 4.26*g/cm3, 2);
  TiO2->AddElement(Ti, 1);
  TiO2->AddElement(O, 2);


  //--------------------------------------------------
  // Scintillator Coating 
  //--------------------------------------------------
  Coating = new G4Material("Coating", density = 1.52*g/cm3, 2);
  Coating->AddMaterial(TiO2, fractionmass = 15.0*perCent);
  Coating->AddMaterial(Polystyrene, fractionmass = 85.0*perCent);

 
  // *** Optical Propierties ***

  // ------------ Generate & Add Material Properties Table ------------

  G4double photonEnergy[] =
  {2.00*eV,2.03*eV,2.06*eV,2.09*eV,2.12*eV,
   2.15*eV,2.18*eV,2.21*eV,2.24*eV,2.27*eV,
   2.30*eV,2.33*eV,2.36*eV,2.39*eV,2.42*eV,
   2.45*eV,2.48*eV,2.51*eV,2.54*eV,2.57*eV,
   2.60*eV,2.63*eV,2.66*eV,2.69*eV,2.72*eV,
   2.75*eV,2.78*eV,2.81*eV,2.84*eV,2.87*eV,
   2.90*eV,2.93*eV,2.96*eV,2.99*eV,3.02*eV,
   3.05*eV,3.08*eV,3.11*eV,3.14*eV,3.17*eV,
   3.20*eV,3.23*eV,3.26*eV,3.29*eV,3.32*eV,
   3.35*eV,3.38*eV,3.41*eV,3.44*eV,3.47*eV};

  const G4int nEntries = sizeof(photonEnergy)/sizeof(G4double);

  //--------------------------------------------------
  // Air
  //--------------------------------------------------
  G4double refractiveIndex[] =
  { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00};

  assert(sizeof(refractiveIndex) == sizeof(photonEnergy));

  G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
  mpt->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries);

  Air->SetMaterialPropertiesTable(mpt);


  //--------------------------------------------------
  //  PMMA for WLSfibers
  //--------------------------------------------------
  G4double refractiveIndexWLSfiber[] =
  { 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
    1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
    1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
    1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
    1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60};

  assert(sizeof(refractiveIndexWLSfiber) == sizeof(photonEnergy));

  G4double absWLSfiber[] =
  {5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,
   5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,
   5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,1.10*m,
   1.10*m,1.10*m,1.10*m,1.10*m,1.10*m,1.10*m, 1.*mm, 1.*mm, 1.*mm, 1.*mm,
    1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm};

  assert(sizeof(absWLSfiber) == sizeof(photonEnergy));

  G4double emissionFib[] =
  {0.05, 0.10, 0.30, 0.50, 0.75, 1.00, 1.50, 1.85, 2.30, 2.75,
   3.25, 3.80, 4.50, 5.20, 6.00, 7.00, 8.50, 9.50, 11.1, 12.4,
   12.9, 13.0, 12.8, 12.3, 11.1, 11.0, 12.0, 11.0, 17.0, 16.9,
   15.0, 9.00, 2.50, 1.00, 0.05, 0.00, 0.00, 0.00, 0.00, 0.00,
   0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};

  assert(sizeof(emissionFib) == sizeof(photonEnergy));

  // Add entries into properties table
  G4MaterialPropertiesTable* mptWLSfiber = new G4MaterialPropertiesTable();
  mptWLSfiber->AddProperty("RINDEX",photonEnergy,refractiveIndexWLSfiber,nEntries);
  mptWLSfiber->AddProperty("WLSABSLENGTH",photonEnergy,absWLSfiber,nEntries);
  mptWLSfiber->AddProperty("WLSCOMPONENT",photonEnergy,emissionFib,nEntries);
  mptWLSfiber->AddConstProperty("WLSTIMECONSTANT", 0.5*ns);

  PMMA->SetMaterialPropertiesTable(mptWLSfiber);


  //--------------------------------------------------
  //  Polyethylene
  //--------------------------------------------------
  G4double refractiveIndexClad1[] =
  { 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
    1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
    1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
    1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
    1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49};

  assert(sizeof(refractiveIndexClad1) == sizeof(photonEnergy));

  G4double absClad[] =
  {20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
   20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
   20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
   20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
   20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m};

  assert(sizeof(absClad) == sizeof(photonEnergy));

  // Add entries into properties table
  G4MaterialPropertiesTable* mptClad1 = new G4MaterialPropertiesTable();
  mptClad1->AddProperty("RINDEX",photonEnergy,refractiveIndexClad1,nEntries);
  mptClad1->AddProperty("ABSLENGTH",photonEnergy,absClad,nEntries);

  Pethylene->SetMaterialPropertiesTable(mptClad1);


  //--------------------------------------------------
  // Fluorinated Polyethylene
  //--------------------------------------------------
   G4double refractiveIndexClad2[] =
   { 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42,
     1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42,
     1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42,
     1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42,
     1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42, 1.42};

   assert(sizeof(refractiveIndexClad2) == sizeof(photonEnergy));

  // Add entries into properties table
  G4MaterialPropertiesTable* mptClad2 = new G4MaterialPropertiesTable();
  mptClad2->AddProperty("RINDEX",photonEnergy,refractiveIndexClad2,nEntries);
  mptClad2->AddProperty("ABSLENGTH",photonEnergy,absClad,nEntries);

  FPethylene->SetMaterialPropertiesTable(mptClad2);


  //--------------------------------------------------
  //  Polystyrene
  //--------------------------------------------------
  G4double refractiveIndexPS[] =
  { 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
    1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
    1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
    1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
    1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50};

  assert(sizeof(refractiveIndexPS) == sizeof(photonEnergy));
G4double factor_absPS = 2.75;
  G4double absPS[] =
  {factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,
   factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,
   factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,
   factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,
   factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm,factor_absPS*2.*cm};

  assert(sizeof(absPS) == sizeof(photonEnergy));

  G4double scintilFast[] =
  {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
   0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
   1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
   1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

  assert(sizeof(scintilFast) == sizeof(photonEnergy));

  // Add entries into properties table
  G4MaterialPropertiesTable* mptPolystyrene = new G4MaterialPropertiesTable();
  mptPolystyrene->AddProperty("RINDEX",photonEnergy,refractiveIndexPS,nEntries);
  mptPolystyrene->AddProperty("ABSLENGTH",photonEnergy,absPS,nEntries);
  mptPolystyrene->AddProperty("FASTCOMPONENT",photonEnergy, scintilFast,nEntries);
  mptPolystyrene->AddConstProperty("SCINTILLATIONYIELD",10./keV);
  mptPolystyrene->AddConstProperty("RESOLUTIONSCALE",1.0);
  mptPolystyrene->AddConstProperty("FASTTIMECONSTANT", 10.*ns);
 
  Polystyrene->SetMaterialPropertiesTable(mptPolystyrene);

  // Set the Birks Constant for the Polystyrene scintillator
  Polystyrene->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

}
  

// *************************
// Mechanical Detector
// ************************* 

G4VPhysicalVolume* DetectorConstruction::Construct()
{                        

  // ----------------------
  // *** Make Materials ***
  // ----------------------
  DefineMaterials();

  G4bool checkOverlaps = true;


  //-----------------
  // World (air)
  //-----------------
  G4VSolid* solidWorld = new G4Box("World", fWorldSizeX, fWorldSizeY, fWorldSizeZ);

  fLogicWorld = new G4LogicalVolume(solidWorld, Air, "World");

  fPhysiWorld = new G4PVPlacement(0,
                                  G4ThreeVector(),
                                  fLogicWorld,
                                  "World",
                                  0,
                                  false,
                                  0, checkOverlaps);

  //--------------------
  // Extrusion (TiO2)
  //--------------------
  G4VSolid* solidExtrusion =
        new G4Box("Extrusion",fBarBase,4*fBarBase,fBarLength);

  G4LogicalVolume* logicExtrusion1 =
                      new G4LogicalVolume(solidExtrusion,
                                          Coating, 
                                          "Extrusion1");

  G4OpticalSurface* TiO2Surface = new G4OpticalSurface("TiO2Surface",
                                                       glisur,
                                                       ground,
                                                       dielectric_metal,
                                                       fExtrusionPolish);

  G4MaterialPropertiesTable* TiO2SurfaceProperty =
                                             new G4MaterialPropertiesTable();

  G4double p_TiO2[] = {2.00*eV, 3.47*eV};
  const G4int nbins = sizeof(p_TiO2)/sizeof(G4double);

  G4double refl_TiO2[] = {fExtrusionReflectivity,fExtrusionReflectivity};
  assert(sizeof(refl_TiO2) == sizeof(p_TiO2));
  G4double effi_TiO2[] = {0, 0};
  assert(sizeof(effi_TiO2) == sizeof(p_TiO2));

  TiO2SurfaceProperty -> AddProperty("REFLECTIVITY",p_TiO2,refl_TiO2,nbins);
  TiO2SurfaceProperty -> AddProperty("EFFICIENCY",p_TiO2,effi_TiO2,nbins);

  TiO2Surface -> SetMaterialPropertiesTable(TiO2SurfaceProperty);

  new G4PVPlacement(0,
                    G4ThreeVector(0.*cm, 0.*cm, 0.*cm),
                    logicExtrusion1,
                    "Extrusion1",
                    fLogicWorld,
                    false,
                    0, checkOverlaps);


  new G4LogicalSkinSurface("TiO2Surface",logicExtrusion1,TiO2Surface);
 

  //--------------------------------------------------
  // Scintillator (Polystyrene)
  //--------------------------------------------------
  G4VSolid* solidScintillator = new G4Box("Scintillator",
                                fBarBase-fCoatingThickness,
                                4*fBarBase-fCoatingThickness,
                                fHoleLength);

  G4LogicalVolume* logicScintillator1 =
                             new G4LogicalVolume(solidScintillator,
                                                Polystyrene,
                                                 "Scintillator1");

   new G4PVPlacement(0,
                    G4ThreeVector(),
                    logicScintillator1,
                    "Scintillator1",
                    logicExtrusion1,
                    false,
                    0, checkOverlaps);


  //----------------------------------
  // Hole (air)
  //----------------------------------
        G4VSolid* solidHole = new G4Tubs("Hole",
                                         0.0*cm,
                                         fHoleRadius,
                                         fHoleLength,
                                         0.*deg,
                                         360.*deg);

        fLogicHole1 = new G4LogicalVolume(solidHole, Air,
                                         "Hole1");
	
	
        fPhysiHole1 = new G4PVPlacement(0,
                                       G4ThreeVector(0.0,0.0,0.0),
                                       fLogicHole1,
                                       "Hole1",
				       logicScintillator1,
                                       false,
                                       0, checkOverlaps);
  
    G4VPhysicalVolume* physiPlacement = fPhysiHole1;
  

  //--------------------------------------------------
  // Fiber Construction
  //-------------------------------------------------- 
  // Boundary Surface Properties
  G4OpticalSurface* opSurface = NULL;
 
  if (fSurfaceRoughness < 1.)
     opSurface = new G4OpticalSurface("RoughSurface",          
                                      glisur,                  
				      ground,
                                      dielectric_dielectric,   
                                      fSurfaceRoughness);     

  G4LogicalVolume   *logicClad1_1;

  G4LogicalVolume   *logicClad2_1;   

  G4VPhysicalVolume *physiClad1_1;

  G4VPhysicalVolume *physiClad2_1;


     //--------------------------------------------------
     // Cladding 2
     //--------------------------------------------------
     	G4VSolid* solidClad2;
 
	solidClad2 = new G4Tubs("Clad2",fClad1RX,fClad2RX,fClad2Z,0.0*rad,twopi*rad);
    
	logicClad2_1 = new G4LogicalVolume(solidClad2, FPethylene,
                                       "Clad2_1");

	physiClad2_1 = new G4PVPlacement(0,
                                    G4ThreeVector(0.0,0.0,fWLSfiberOrigin),
                                    logicClad2_1,
                                    "Clad2_1",
                                    fLogicHole1,
                                    false,
                                    0, checkOverlaps);

     // Place the rough surface only if needed
     if (opSurface) {
       new G4LogicalBorderSurface("surfaceClad2Out",
                                  physiClad2_1,
                                  physiPlacement,
                                  opSurface);
       new G4LogicalBorderSurface("surfaceClad2In",
                                  physiPlacement,
                                  physiClad2_1,
                                  opSurface);
     			}

    
     physiPlacement = physiClad2_1;


     //--------------------------------------------------
     // Cladding 1
     //--------------------------------------------------
	G4VSolid* solidClad1;

     	solidClad1 = new G4Tubs("Clad1",fWLSfiberRX,fClad1RX,fClad1Z,0.0*rad,twopi*rad);
     
	logicClad1_1 = new G4LogicalVolume(solidClad1, Pethylene,
                                      "Clad1_1");

	physiClad1_1 = new G4PVPlacement(0,
                                    G4ThreeVector(0.0,0.0,fWLSfiberOrigin),
                                    logicClad1_1,
                                    "Clad1_1",
                                    fLogicHole1,
                                    false,
                                    0, checkOverlaps);
 

     // Place the rough surface only if needed
     if (opSurface) {
       new G4LogicalBorderSurface("surfaceClad1Out",
                                  physiClad1_1,
                                  physiPlacement,
                                  opSurface);
       new G4LogicalBorderSurface("surfaceClad1In",
                                  physiPlacement,
                                  physiClad1_1,
                                  opSurface);
     			}


     physiPlacement = physiClad1_1;


     //--------------------------------------------------
     // WLS Fiber
     //--------------------------------------------------
     G4VSolid* solidWLSfiber;

       solidWLSfiber =
           new G4Tubs("WLSFiber",0.,fWLSfiberRX,fWLSfiberZ,0.0*rad,twopi*rad);

      G4LogicalVolume* logicWLSfiber1 = new G4LogicalVolume(solidWLSfiber, PMMA, "WLSFiber1");

      G4VPhysicalVolume* physiWLSfiber1 = new G4PVPlacement(0,
                                       G4ThreeVector(0.0,0.0,fWLSfiberOrigin),
                                       logicWLSfiber1,
                                       "WLSFiber1",
                                       fLogicHole1,
                                       false,
                                       0, checkOverlaps);

     // Place the rough surface only if needed
     if (opSurface) {
       new G4LogicalBorderSurface("surfaceWLSOut",
                                  physiWLSfiber1,
                                  physiPlacement,
                                  opSurface);
       new G4LogicalBorderSurface("surfaceWLSIn",
                                  physiPlacement,
                                  physiWLSfiber1,
                                  opSurface);
                       }


  //--------------------------------------------------
  // SiPM (air)
  //--------------------------------------------------  
  G4VSolid* solidPhotonDet;

  solidPhotonDet = new G4Box("PhotonDet", 0.65*mm, 0.65*mm, 0.1*mm);

  G4LogicalVolume*   logicPhotonDet1 =
                                    new G4LogicalVolume(solidPhotonDet,
                                                       Air,
                                                        "PhotonDet1");
 new G4PVPlacement(0,
                G4ThreeVector(0.0,0.0,595.95*mm),//distance 1mm
            //      G4ThreeVector(0.0,0.0,595.35*mm), //distance 0.5mm
		//    G4ThreeVector(0.0,0.0,594.85*mm),//distance 0	
                    logicPhotonDet1,
                    "PhotonDet1",
                    fLogicHole1,
                    false,
                    0, checkOverlaps);


  //--------------------------------------------------
  // End of the fiber (air)
  //--------------------------------------------------  
  G4VSolid* EndFiber = new G4Tubs("EndFiber",0.,fClad2RX,0.05*mm,0.0*rad,twopi*rad);
  G4LogicalVolume*   logicEndFiber = new G4LogicalVolume(EndFiber,
                                                         Air,
                                                       "EndFiber");
  new G4PVPlacement(0,
                    G4ThreeVector(0.0,0.0,-59.980*cm),
                    logicEndFiber,
                    "EndFiber",
                    logicExtrusion1,
                    false,
                    0, checkOverlaps); 



  return fPhysiWorld;

}
