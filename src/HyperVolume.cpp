#include "HyperVolume.h"

///Simple constuctor that only takes the dimensionality of 
///the HyperVolume.
///
HyperVolume::HyperVolume(int dimension) : 
  _dimension ( dimension )
{ 
}

///Check to see if a HyperPoint is within the HyperVolume
///
bool HyperVolume::inVolume(const HyperPoint& coords) const{

  for(unsigned int i = 0; i < _hyperCuboids.size(); i++){
    if(_hyperCuboids.at(i).inVolume(coords)==1) return 1;
  }
  return 0;

}

///add a HyperCuboid (defined by upper and lower HyperPoints) to the HyperVolume
///
void HyperVolume::addHyperCuboid(const HyperPoint& lowCorner,const HyperPoint& highCorner){

  addHyperCuboid( HyperCuboid(lowCorner, highCorner) );
}

///add a HyperCuboid to the HyperVolume
///
void HyperVolume::addHyperCuboid(const HyperCuboid& hyperCuboid){
  if (hyperCuboid.getDimension() == _dimension) _hyperCuboids.push_back(hyperCuboid);
  else std::cerr << "The HyperCuboid you are adding to this HyperVolume has the wrong dimension";
}

///Find the Minimum value in a given dimension.
///
double HyperVolume::getMin(int dimension) const{

  if (dimension < _dimension) {
    double min = _hyperCuboids.at(0).getLowCorner().at(dimension);
    for(unsigned int i = 1; i < _hyperCuboids.size(); i++){
      double temp = _hyperCuboids.at(i).getLowCorner().at(dimension);
      if (min > temp) min = temp;
    }
    return min;
  }

  std::cerr << "You are requesting a dimensionality that does not exist in this HyperVolume";
  return -1.0;

}

///Find the Maximum value in a given dimension.
///
double HyperVolume::getMax(int dimension) const{

  if (dimension < _dimension) {
    double max = _hyperCuboids.at(0).getHighCorner().at(dimension);
    for(unsigned int i = 1; i < _hyperCuboids.size(); i++){
      double temp = _hyperCuboids.at(i).getHighCorner().at(dimension);
      if (max < temp) max = temp;
    }
    return max;
  }

  std::cerr << "You are requesting a dimensionality that does not exist in this HyperVolume";
  return -1.0;

}

///get the limits of the HyperVolume
///
HyperCuboid HyperVolume::getLimits() const{
  HyperCuboid limits(getDimension());
  for (int i = 0; i < getDimension(); i++){
    limits.getLowCorner ().at(i) = getMin(i);
    limits.getHighCorner().at(i) = getMax(i);
  }
  return limits;
}
