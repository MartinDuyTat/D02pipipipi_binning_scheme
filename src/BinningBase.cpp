#include "BinningBase.h"

BinningBase::BinningBase() :
  _dimension  (0 ),
  _binningType("")   
{


}

bool BinningBase::isSameBinningType( const BinningBase& other ) const{
  return (other._binningType == _binningType);
}

void BinningBase::setBinningType(TString binningType){
  _binningType = binningType;
}



const int& BinningBase::getDimension () const{ 
  return _dimension; 
}  

void BinningBase::setDimension (int dimension){
  if (_dimension == 0){
    _dimension       = dimension;
  }
}


double BinningBase::getMin(int dimension) const{
  return getLimits().getLowCorner().at(dimension);
}

double BinningBase::getMax(int dimension) const{
  return getLimits().getHighCorner().at(dimension);
}

bool BinningBase::isDiskResident() const{
  return false;
}
