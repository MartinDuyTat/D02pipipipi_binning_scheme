#include "HistogramBase.h"

#include <iostream>


///Construct a histogram base with a specified number of bins
///
HistogramBase::HistogramBase(int nBins) :
  _nBins(nBins),
  _binContents(nBins+1,0.0),
  _sumW2      (nBins+1,0.0),
  _min(-999.9),
  _max(-999.9)
{
}

///Update the number of bins and set all contents to zero
///
void HistogramBase::resetBinContents(int nBins){
  _nBins = nBins;
  _binContents = std::vector<double>(nBins+1,0.0);
  _sumW2       = std::vector<double>(nBins+1,0.0);
}

/// Load the contents, sumw2, and bin numbers from a TTree
/// in the ROOT file specified (opened using READ)
void HistogramBase::loadBase(TString filename){

  TFile file(filename, "READ");
  TTree* tree = (TTree*)file.Get("HistogramBase");

  int    binNumber  = -1;
  double binContent = 0.0;
  double sumW2      = 0.0;
  
  int nEntries = tree->GetEntries();

  tree->SetBranchAddress("binNumber" , &binNumber );
  tree->SetBranchAddress("binContent", &binContent);
  tree->SetBranchAddress("sumW2"     , &sumW2     );
  
  this->resetBinContents(nEntries - 1);


  for(int ent = 0; ent < nEntries; ent++){
    tree->GetEntry(ent);
    

    _binContents.at((int)binNumber) = binContent;
    _sumW2      .at((int)binNumber) = sumW2;
  }
  
  file.Close();

}

///Check if it's a valid bin number, if not
///return the overflow/underflow bin
int HistogramBase::checkBinNumber(int bin) const{
  if(bin == -1){
    //This is also a valid way to add to the underflow/overflow bin with no error message
    return _nBins;
  }
  if(bin > _nBins || bin < 0) {
    std::cerr << "Bin " << bin << " does not exist! Adding to underflow/overflow bin " << _nBins;
    return _nBins;
  }
  return bin;
}

///Get the content of a bin
///
double HistogramBase::getBinContent(int bin) const{
  bin = checkBinNumber(bin); 
  return _binContents[bin];
}
