#include "HyperPoint.h"

///Most basic constructor that makes a HyperPoint with a 
///specified dimension, and filled with 0.0 for each element.
HyperPoint::HyperPoint(int dimension) :
  _coords(dimension, 0.0)
{
}

///Constructor that makes a HyperPoint with a 
///specified dimension, and fills each element with
///a specified value.
HyperPoint::HyperPoint(int dimension, double val) :
  _coords(dimension, val)
{
}

///Constuctor for a 5D space.
///
HyperPoint::HyperPoint(double x1, double x2, double x3, double x4, double x5) :
  _coords(5, 0.0)
{
  at(0) = x1;
  at(1) = x2;
  at(2) = x3;
  at(3) = x4;
  at(4) = x5;
}

///Check if two HyperPoints are compatible  (of the same dimension)
///
bool HyperPoint::compatible(const HyperPoint & other, bool printError) const{

  if (getDimension() != other.getDimension()) {
    if (printError == true) std::cerr << "HyperPoints are of different dimension i.e. not compatible!!!"; 
    return 0;
  }
  return 1;

}

///Checks if every element of v1 is less than the  
///same element in v2. i.e. v1_0 < v2_0 && v1_1 < v2_1 && ...
bool HyperPoint::allLT (const HyperPoint& other) const{
  if (this->compatible(other) == 0) return false; 

  for (int i = 0; i < size(); i++) {
    if (this->at(i) >= other.at(i)) return false;
  }
  return true;
}

///Checks if every element of v2 is greater than the  
///same element in v2. i.e. v1_0 > v2_0 && v1_1 > v2_1 && ...
bool HyperPoint::allGT  (const HyperPoint& other) const{
  if (this->compatible(other) == 0) return false; 

  for (int i = 0; i < size(); i++) {
    if (this->at(i) <= other.at(i)) return false;
  }
  return true;
}

///Checks if every element of v1 is less than or equal to the  
///same element in v2. i.e. v1_0 <= v2_0 && v1_1 <= v2_1 && ...
bool HyperPoint::allLTOE(const HyperPoint& other) const{
  if (this->compatible(other) == 0) return false; 

  for (int i = 0; i < size(); i++) {
    if (this->at(i) > other.at(i)) return false;
  }
  return true;
}

///Checks if every element of v1 is greater than or equal to the  
///same element in v2. i.e. v1_0 >= v2_0 && v1_1 >= v2_1 && ...
bool HyperPoint::allGTOE(const HyperPoint& other) const{
  if (this->compatible(other) == 0) return false; 
  
  for (int i = 0; i < size(); i++) {
    if (this->at(i) < other.at(i)) return false;
  }
  return true;
}

///Checks if two HyperPoints are identical
///
bool HyperPoint::operator ==(const HyperPoint& other) const{
  if (this->compatible(other) == 0) return false; 
  
  for (int i = 0; i < size(); i++) if (this->at(i) != other.at(i)) return false;
  return true;
}

///Checks if two HyperPoints are not identical
///
bool HyperPoint::operator !=(const HyperPoint& other) const{
  if (this->compatible(other) == 0) return false; 
  
  for (int i = 0; i < size(); i++) if (this->at(i) != other.at(i)) return true;
  return false;
}

///returns a const reference to a choosen element of the HyperPoint.
///Can be used for getting the value, but not setting
const double& HyperPoint::at  (int i) const{
  if (i >= this->size()) {
    std::cerr << "The component of this HyperPoint you have selected is out of range";
    return _coords.at(this->size() - 1);
  }
  return _coords.at(i);
}

///returns a reference to a choosen element of the HyperPoint.
///Can be used for getting or setting it's value.
double& HyperPoint::at  (int i){
  if (i >= size()) {
    std::cerr << "The component of this HyperPoint you have selected is out of range";
    return _coords.at(size() - 1);
  }
  return _coords.at(i);
}
