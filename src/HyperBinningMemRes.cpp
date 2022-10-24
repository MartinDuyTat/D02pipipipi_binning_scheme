#include "HyperBinningMemRes.h"


///Set the dimension of the HyperBinningMemRes. This can only be 
///called once, when it is known what dimesnion it is.
void HyperBinningMemRes::setDimension(int dim){
  
  if (getDimension() == 0){
    BinningBase::setDimension(dim);
  }

}

std::vector<int> HyperBinningMemRes::getLinkedHyperVolumes( int volumeNumber ) const{

  return _linkedHyperVolumes.at(volumeNumber);

}

BinningBase* HyperBinningMemRes::clone() const{

  return dynamic_cast<BinningBase*>(new HyperBinningMemRes(*this));

}

///Add a primary volume number
///
void HyperBinningMemRes::addPrimaryVolumeNumber(int volumeNumber){
  _primaryVolumeNumbers.push_back(volumeNumber);
}

///get the number of HyperVolumes
///
int HyperBinningMemRes::getNumHyperVolumes() const{
  return _hyperVolumes.size();
}

int HyperBinningMemRes::getNumPrimaryVolumes  () const{
  return _primaryVolumeNumbers.size();
}

int HyperBinningMemRes::getPrimaryVolumeNumber(int i) const{
  return _primaryVolumeNumbers.at(i);
}

///Add a HyperVolume to the HyperBinningMemRes and add a set of empty
///HyperVolume links.
bool HyperBinningMemRes::addHyperVolume(const HyperVolume& hyperVolume, std::vector<int> linkedVolumes){
  
  //If this is the first volume that has been added, use it to set the dimension
  if (_hyperVolumes.size() == 0){
    setDimension( hyperVolume.getDimension() );
  }

  if (hyperVolume.getDimension() == getDimension()) {
    _hyperVolumes.push_back(hyperVolume); 
    _linkedHyperVolumes.push_back(linkedVolumes);
    updateCash();
    return true;
  }

  std::cerr << "This HyperVolume has the wrong dimensionality for this HyperBinningMemRes";
  return false;

}

///Save the list of Primary Volume Numbers to the open (and in scope) TFile.
///
void HyperBinningMemRes::loadPrimaryVolumeNumbers(TFile* file) {

  TTree* tree = dynamic_cast<TTree*>( file->Get("PrimaryVolumeNumbers") );
  
  if (tree == 0){
    std::cerr << "Could not open TTree in HyperBinningMemRes::loadPrimaryVolumeNumbers()";
    return;
  }

  //Define branch addresses
  int volumeNumber = -1;

  tree->SetBranchAddress("volumeNumber", &volumeNumber);

  //Loop over each Primary Volume
  for(int i = 0; i < tree->GetEntries(); i++ ){
    tree->GetEntry(i);
    _primaryVolumeNumbers.push_back(volumeNumber);
  }
  
}

///Set branch addresses for loading HyperBinningMemRes
///from a file.
void HyperBinningMemRes::setBranchAddresses(TTree* tree, int* binNumber, double* lowCorner, double* highCorner, std::vector<int>** linkedBins) const{

  tree->SetBranchAddress("binNumber", binNumber);
  tree->SetBranchAddress("linkedBins", linkedBins);
  for (int i = 0; i < getDimension(); i++) {
    TString lowCornerName  = "lowCorner_"; lowCornerName += i; 
    TString highCornerName = "highCorner_"; highCornerName += i;
    tree->SetBranchAddress(lowCornerName, lowCorner + i);
    tree->SetBranchAddress(highCornerName, highCorner + i);
  }

}

HyperVolume HyperBinningMemRes::getHyperVolume(int volumeNumber) const{
  return _hyperVolumes.at(volumeNumber);
}

///Load HyperBinningMemRes from a file
///
void HyperBinningMemRes::load(TString filename, TString option){
  
  if (option != "READ"){
    std::cout << "For a memory resident HyperBinning you should always use the READ option. Setting to READ" << std::endl;
    option = "READ";
  }

  TFile* file = new TFile(filename, "READ");

  if (file == 0){
    std::cerr << "Could not open TFile in HyperBinningMemRes::load(" << filename << ")";
    return;
  }

  loadPrimaryVolumeNumbers(file);

  TTree* tree = (TTree*)file->Get("HyperBinning");

  if (tree == 0){
    std::cerr << "Could not open TTree in HyperBinningMemRes::load()";
    return;
  }
  
  //Figure out how many dimensions there are from the tree
  setDimension( getHyperBinningDimFromTree(tree) );

  //Create branch addresses and link them to TTree
  int binNumber = -1;
  double* lowCorner  = new double [getDimension()];
  double* highCorner = new double [getDimension()];  
  std::vector<int>* linkedBins = new std::vector<int>();

  setBranchAddresses(tree, &binNumber, lowCorner, highCorner, &linkedBins);
  

  //Loop over the TTree and fill the HyperBinningMemRes
  int nEntries = tree->GetEntries();

  int currentBinNumber = -1;
  HyperVolume* currentHyperVolume = new HyperVolume(getDimension());
  std::vector<int> currentLinkedVolumes;

  //_verbose = true;

  for(int ent = 0; ent < nEntries; ent++){
    tree->GetEntry(ent);
    
    //If the bin number hasn't changed, need to
    //add HyperCube to previous HyperVolume
    if(ent == 0 || currentBinNumber == binNumber){
      currentBinNumber = binNumber;
      currentLinkedVolumes = *linkedBins;
      HyperPoint lowCornerVect (getDimension());
      HyperPoint highCornerVect(getDimension());
      for (int i = 0; i < getDimension(); i++){
        lowCornerVect .at(i) = lowCorner [i];
        highCornerVect.at(i) = highCorner[i];
      }
      currentHyperVolume->addHyperCuboid(lowCornerVect, highCornerVect);
    }

    //if the bin number has changed, need to add
    //previous HyperVolume to HyperBinningMemRes
    //and start a new HyperVolume
    else {
      this->addHyperVolume(*currentHyperVolume, currentLinkedVolumes);
      delete currentHyperVolume;
      currentHyperVolume = new HyperVolume(getDimension());
      currentLinkedVolumes = *linkedBins;
      currentBinNumber = binNumber;
      HyperPoint lowCornerVect (getDimension());
      HyperPoint highCornerVect(getDimension());
      for (int i = 0; i < getDimension(); i++){
        lowCornerVect .at(i) = lowCorner [i];
        highCornerVect.at(i) = highCorner[i];
      }
      currentHyperVolume->addHyperCuboid(lowCornerVect, highCornerVect);
      
    }

    //if it's the final iteration, need to add the final HyperVolume
    if (ent == nEntries - 1) {
      this->addHyperVolume(*currentHyperVolume);
    }
  }
  

  //_verbose = false;

  delete currentHyperVolume;
  delete lowCorner;
  delete highCorner;
  delete linkedBins;

  updateCash();

  file->Close();

}
