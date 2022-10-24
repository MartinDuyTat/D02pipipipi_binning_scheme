#include "HyperCuboid.h"

///Most basic constructor where only the dimension
///of the cuboid is specified.
HyperCuboid::HyperCuboid(int dimension) : 
  _dimension ( dimension ),
  _lowCorner ( dimension ),
  _highCorner( dimension )
{ 
}

///Construct the HyperCuboid with two HyperPoints, one in the low corner, 
///and one in the high corner
HyperCuboid::HyperCuboid(const HyperPoint& lowCorner, const HyperPoint& highCorner) :
  _dimension( lowCorner.size() ),
  _lowCorner ( _dimension ),  //note this assignment sets the dimension of this variable forever, so if 
  _highCorner( _dimension )   //low and high corners are a different size, an error will be thrown inside HyperPoint
{

  if (lowCorner.allLT(highCorner)){
    _lowCorner  = lowCorner;
    _highCorner = highCorner;
  }

}

///Construct the HyperCuboid with two HyperPoints (x, x, x ....) 
///and (y, y, y ....)
HyperCuboid::HyperCuboid(int dimension, double low, double high) :
  _dimension ( dimension ),
  _lowCorner ( dimension ),  //note this assignment sets the dimension of this variable forever, so if 
  _highCorner( dimension )   //low and high corners are a different size, an error will be thrown inside HyperPoint
{
  
  for (int i = 0; i < _dimension; i++){
    _lowCorner.at(i)  = low ;
    _highCorner.at(i) = high;
  }

  if (low > high) {
    std::cerr << "Your lowCorner isn't lower than your highCorner" << std::endl;
  }
}

///See if two HyperCuboids are the same
///
bool HyperCuboid::operator ==(const HyperCuboid& other) const{
  if (other.getDimension() != this->getDimension()) {
    std::cerr << "Trying to compare HyperCuboids of different dimensions, returning false";
    return false; 
  }
  if (getLowCorner()  != other.getLowCorner() ) return false;
  if (getHighCorner() != other.getHighCorner()) return false;
  return true;
}

///See if two HyperCuboids are not the same
///
bool HyperCuboid::operator !=(const HyperCuboid& other) const{
  if (other.getDimension() != this->getDimension()) {
    std::cerr << "Trying to compare HyperCuboids of different dimensions, returning false";
    return false; 
  }
  if (getLowCorner()  != other.getLowCorner() ) return true;
  if (getHighCorner() != other.getHighCorner()) return true;
  return false;
}


///See if a HyperPoint is within the HyperCuboid volume
///
bool HyperCuboid::inVolume(const HyperPoint& coords) const{

  if (_lowCorner.allLT(coords) &&  _highCorner.allGTOE(coords)) return 1;
  return 0;

}

///See if a HyperPoint is within the HyperCuboid volume, but only
///for selected dimensions.
bool HyperCuboid::inVolume(const HyperPoint& coords, std::vector<int> dims) const{

  for (unsigned i = 0; i < dims.size(); i++){
    int dim = dims.at(i);
    double minEdge = getLowCorner ().at(dim);
    double maxEdge = getHighCorner().at(dim);
    double val     = coords.at(dim);
    if ( ( minEdge < val && val <= maxEdge ) == false ) return false;
  }

  return true;

}
