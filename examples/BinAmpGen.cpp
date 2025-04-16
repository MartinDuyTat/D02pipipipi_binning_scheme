/**
 * This application reads a decay tree of LHCb events and assigns bin numbers
 * @param 1 Filename of AmpGen tuple
 * @param 2 Filename of binned AmpGen tuple
 * @param 3 Filename of binning scheme
 */

#include<iostream>
#include<array>
#include<string>
#include<numeric>
#include<algorithm>
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
  const std::array<std::string, 4> Components{"Px", "Py", "Pz", "E"};
  // Particle names in AmpGen
  const std::array<std::string, 4> AmpGenNames{"_1_pi~", "_3_pi~", "_2_pi#", "_4_pi#"};
  // Load AmpGen four-momenta and time
  std::cout << "Loading events...\n";
  TChain Chain("DalitzEventList");
  Chain.Add(argv[1]);
  for(std::size_t i = 0; i < 4; i++) {
    for(std::size_t j = 0; j < 4; j++) {
      const std::string ComponentName = AmpGenNames[i] + "_" + Components[j];
      Chain.SetBranchAddress(ComponentName.c_str(), &Daughters[i][j]);
    }
  }
  double D_decayTime;
  Chain.SetBranchAddress("D_decayTime", &D_decayTime);
  std::cout << "Events ready to be binned\n";
  std::cout << "Setting up output file...\n";
  // Open new file with new tuple
  TFile File(argv[2], "RECREATE");
  auto DataTree = Chain.CloneTree(0);
  int PSBin, TimeBin;
  DataTree->Branch("PSBin", &PSBin);
  DataTree->Branch("TimeBin", &TimeBin);
  std::cout << "Output file ready\n";
  std::cout << "Loading binning scheme...\n";
  // Load binning scheme
  const HyperHistogram hyperHistogram(argv[3], "MEMRES READ");
  std::cout << "Binning scheme ready ready to bin\n";
  std::cout << "Going through all events...\n";
  // Loop over all events
  for(Long64_t n = 0; n < Chain.GetEntries(); n++) {
    Chain.GetEntry(n);
    // Time bins
    constexpr std::array<double, 10> TimeEdges{0.94, 1.185, 1.40, 1.62, 1.85,
	                                       2.13, 2.45, 2.87, 3.5, 8.0};
    double Dlifetime = D_decayTime/0.0004103;
    TimeBin = std::lower_bound(TimeEdges.begin(), TimeEdges.end(), Dlifetime)
            - TimeEdges.begin();
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
    PSBin = hyperHistogram.getVal(hyperPoint)*(Flip ? -1 : 1);
    // Save event
    DataTree->Fill();
  }
  std::cout << "All events assigned to time and phase space bins\n";
  // Save binned tuples to file
  File.cd();
  DataTree->Write();
  File.Close();
  return 0;
}
