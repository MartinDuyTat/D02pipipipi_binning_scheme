#include "HyperHistogram.h"

/**
  Load a HyperHistogram from filename. If
*/
HyperHistogram::HyperHistogram(TString filename, TString option) :
  HistogramBase(0),
  _binning(0)
{

  load(filename, option);

}

/**
Get the bin content where the given HyperPoint lies
*/
double HyperHistogram::getVal(const HyperPoint& point) const{ 
  
  int binNumber = _binning->getBinNum(point);
  return this->getBinContent(binNumber);

}

int HyperHistogram::getDimension() const{

  if (_binning == 0){
    std::cerr << "HyperHistogram::getDimension - cannot get dimension, binning not set." << std::endl;
    return 0;
  }
  return _binning->getDimension();

}

/**
Get binning type from file
*/
TString HyperHistogram::getBinningType(TString filename){

  TFile* file = new TFile(filename, "READ");

  if (file == 0){
    std::cerr << "Could not open TFile in HyperBinning::load(" << filename << ")";
    return "";
  }

  TTree* tree  = (TTree*)file->Get("HyperBinning");

  if (tree != 0){
    file->Close();
    return "HyperBinning";
  }

  file->Close();
  return "";

}

/**
Load the HyperHistogram from a TFile
*/
void HyperHistogram::load(TString filename, TString option){

  //If loading from a file, we first need to figure out what 
  //type of binning is saved in that file. 
  
  TString binningType = getBinningType(filename);
 
  if (binningType.Contains("HyperBinning")){
    
    if (_binning != 0) {
      delete _binning;
      _binning = 0;
    }

    if (!option.Contains("DISK")){
      _binning = new HyperBinningMemRes();
    }

  }

  if (binningType == ""){
    std::cerr << "HyperHistogram::load - I could not find any binning scheme in this file" << std::endl;
  }

  _binning->load(filename, "READ");
  this->loadBase(filename);

}

/**
Destructor
*/
HyperHistogram::~HyperHistogram(){

  if (!_binning){
    delete _binning;
    _binning = nullptr;
  }
  

}
