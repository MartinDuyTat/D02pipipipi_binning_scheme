/**
 * This is an example of the D->4pi binning code
 * A random event, flat in phase space, is generated and the bin number is determined
 * @param 1 Filename of binning scheme
 * @param 2 Seed for random event generation
 * @param 3 Number of events to generate
 */

#include<array>
#include<string>
#include<iostream>
#include<algorithm>
#include"TLorentzVector.h"
#include"TGenPhaseSpace.h"
#include"TRandom.h"
#include"HyperPoint.h"
#include"HyperHistogram.h"
#include"Utilities.h"

int main(int argc, char *argv[]) {

  if(argc != 3 && argc != 4) {
    return 0;
  }

  // Four-momentum of D in its rest frame

  TLorentzVector P_D(0.0, 0.0, 0.0, 1.86483);

  // D daughter masses

  const double PionMass = 0.13957039;
  const std::array DaughterMasses{PionMass, PionMass, PionMass, PionMass};

  // Set seed

  gRandom->SetSeed(std::stoi(std::string(argv[2])));

  // Load binning scheme

  const HyperHistogram hyperHistogram(argv[1], "MEMRES READ");

  // Number of random events to generate

  const std::size_t NumberIterations = argc == 4 ?
                                       std::stoi(std::string(argv[3])) : 1;
  
  for(std::size_t n = 0; n < NumberIterations; n++) {

    // Generate random D decay

    TGenPhaseSpace PhaseSpace;
    PhaseSpace.SetDecay(P_D, 4, DaughterMasses.data());
    PhaseSpace.Generate();

    // Particle ordering: pi+ pi+ pi- pi-
    std::array<TLorentzVector, 4> Daughters;
    for(std::size_t i = 0; i < 4; i++) {
      Daughters[i] = *PhaseSpace.GetDecay(i);
    }

    // Calculate the five variables that parameterise the D decay

    const double mPlus = (Daughters[0] + Daughters[1]).M();
    const double mMinus = (Daughters[2] + Daughters[3]).M();

    double cosThetaPlus =
      Utilities::getCosTheta(Daughters[0], Daughters[0] + Daughters[1], P_D);

    double cosThetaMinus =
      Utilities::getCosTheta(Daughters[2], Daughters[2] + Daughters[3], P_D);

    double phi = Utilities::getPhi(Daughters);

    constexpr double mMin = 2.0*0.13957039;

    double mPlusPrime, mMinusPrime;
    if(mMinus > mPlus) {
      mPlusPrime  = mPlus  + (mPlus - mMin);
      mMinusPrime = mMinus + (mPlus - mMin);
    }
    else {
      mPlusPrime  = mPlus  + (mMinus - mMin);
      mMinusPrime = mMinus + (mMinus - mMin);
    }

    // Transform the variables so that cosThetaPlus > 0, cosThetaMinus > 0 and phi > 0

    if (cosThetaPlus < 0.0){
      cosThetaPlus = -cosThetaPlus;
      phi = phi - TMath::Pi();
    }

    if (cosThetaMinus < 0.0){
      cosThetaMinus = -cosThetaMinus;
      phi = phi- TMath::Pi();
    }

    while (phi < -TMath::Pi()){
      phi += 2.0*TMath::Pi();
    }

    while (phi > TMath::Pi()){
      phi -= 2.0*TMath::Pi();
    }

    bool Flip = false;
    if (phi < 0){
      std::swap(cosThetaPlus, cosThetaMinus); 
      std::swap(mPlusPrime  , mMinusPrime  ); 
      phi = -phi;
      Flip = true;
    }

    // Create hyperpoint and determine bin number

    HyperPoint hyperPoint(mPlusPrime, mMinusPrime, cosThetaPlus, cosThetaMinus, phi);
    const double BinNumber = hyperHistogram.getVal(hyperPoint)*(Flip ? -1 : 1);

    // Print event
    for(std::size_t i = 0; i < 4; i++) {
      if(i < 2) {
	std::cout << "pi+: (";
      } else {
	std::cout << "pi-: (";
      }
      for(std::size_t j = 0; j < 4; j++) {
	std::cout << Daughters[i][j];
	if(j == 4) {
	  std::cout << ")\n";
	} else {
	  std::cout << ", ";
	}
      }
      std::cout << "\n";
    }
    std::cout << "Bin number: " << BinNumber << "\n";
  }

  return 0;
}
