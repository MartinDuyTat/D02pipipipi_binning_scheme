/**
 * This application reads a decay tree of LHCb events and assigns bin numbers
 * @param 1 Filename of LHCb tuple
 * @param 2 Filename of binned LHCb tuple
 * @param 3 Filename of binning scheme
 */

#include<iostream>
#include<array>
#include<string>
#include<numeric>
#include"TChain.h"
#include"TFile.h"
#include"TTree.h"
#include"HyperPoint.h"
#include"HyperHistogram.h"
#include"Utilities.h"

int main(int argc, char *argv[]) {
  if(argc != 4) {
    return 0;
  }
  std::cout << "Binning D0->pipipipi events\n";
  // D daughter momenta
  std::array<TLorentzVector, 4> Daughters;
  // Momentum component names
  const std::array<std::string, 4> Components{"PX", "PY", "PZ", "PE"};
  // Load LHCb data
  std::cout << "Loading events...\n";
  TChain DataChain("DecayTree");
  DataChain.Add(argv[1]);
  for(std::size_t i = 1; i <= 4; i++) {
    for(std::size_t j = 0; j < 4; j++) {
      const std::string ComponentName = "Bu_constD0PV_D0_h"
	                              + std::to_string(i) + "_"
	                              + Components[j];
      DataChain.SetBranchAddress(ComponentName.c_str(), &Daughters[i - 1][j]);
    }
  }
  std::cout << "Events ready to be binned\n";
  std::cout << "Setting up output file...\n";
  // Open new file with new tuple
  TFile File(argv[2], "RECREATE");
  auto DataTree = DataChain.CloneTree(0);
  int BinNumber;
  DataTree->Branch("BinNumber_5Bins", &BinNumber);
  std::cout << "Output file ready\n";
  std::cout << "Loading binning scheme...\n";
  // Load binning scheme
  const HyperHistogram hyperHistogram(argv[3], "MEMRES READ");
  std::cout << "Binning scheme ready ready to bin\n";
  std::cout << "Going through all events...\n";
  // Loop over all events
  for(Long64_t n = 0; n < DataChain.GetEntries(); n++) {
    DataChain.GetEntry(n);
    // Calculate the five variables that parameterise the D decay
    const double mPlus = (Daughters[0] + Daughters[1]).M();
    const double mMinus = (Daughters[2] + Daughters[3]).M();
    const TLorentzVector P_D = std::accumulate(Daughters.begin(),
					       Daughters.end(),
					       TLorentzVector());
    double cosThetaPlus =
      Utilities::getCosTheta(Daughters[0], Daughters[0] + Daughters[1], P_D);
    double cosThetaMinus =
      Utilities::getCosTheta(Daughters[2], Daughters[2] + Daughters[3], P_D);
    double phi = Utilities::getPhi(Daughters);
    constexpr double mMin = 2.0*0.13957039*1000.0;
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
    // Change from MeV to GeV
    mPlusPrime /= 1000.0;
    mMinusPrime /= 1000.0;
    // Create hyperpoint and determine bin number
    HyperPoint hyperPoint(mPlusPrime, mMinusPrime, cosThetaPlus, cosThetaMinus, phi);
    BinNumber = hyperHistogram.getVal(hyperPoint)*(Flip ? -1 : 1);
    // Save event
    DataTree->Fill();
  }
  std::cout << "All events assigned to phase space bins\n";
  // Save binned tuples to file
  File.cd();
  DataTree->Write();
  File.Close();
  return 0;
}
