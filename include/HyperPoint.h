/**
 * <B>HyperPlot</B>,
 * Author: Sam Harnew, sam.harnew@gmail.com ,
 * Date: Dec 2015
 *
 * A point in multi-dimensional space  
 *
 **/
 

#ifndef HYPERPOINT_HH
#define HYPERPOINT_HH

// HyperPlot includes

// Root includes
#include "TMatrixD.h"
#include "TRandom.h"

// std includes
#include <iostream>


class HyperPoint {

  protected:
  
  std::vector<double> _coords; /**< The coordinates of the point in muli-dimensional spave */
  

  public:

  HyperPoint(int dimension);
  HyperPoint(int dimension, double val);

  //some constuctors for a set number of dimensions
  HyperPoint(double x1, double x2, double x3, double x4, double x5);

  //std::vector compatibility
  const double& at(int i) const;
  double& at(int i);

  HyperPoint & operator= (const HyperPoint & other) = default;

  bool operator ==(const HyperPoint& other) const;
  bool operator !=(const HyperPoint& other) const;

  bool allLT  (const HyperPoint& other) const;
  bool allGT  (const HyperPoint& other) const;
  bool allLTOE(const HyperPoint& other) const;
  bool allGTOE(const HyperPoint& other) const;
  
  int size() const {return (int)_coords.size();}
  /**< get the dimensionality of the HyperPoint */

  int getDimension() const{return size();}
  /**< get the dimensionality of the HyperPoint */
  
  bool compatible(const HyperPoint& other, bool printError = true) const;


  ~HyperPoint() = default;

};



#endif

