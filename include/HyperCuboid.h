/**
 * <B>HyperPlot</B>,
 * Author: Sam Harnew, sam.harnew@gmail.com ,
 * Date: Dec 2015
 *
 * A multi-dimensional cuboid that is defined by 2 corners (low and high)
 *
 **/
 
#ifndef HYPERCUBOID_HH
#define HYPERCUBOID_HH

class HyperCuboid;

// HyperPlot includes
#include "HyperPoint.h"
#include "HyperVolume.h"

// Root includes

// std includes
#include <bitset>
#include <iostream>


class HyperCuboid {

  private:
  
  int _dimension;         /**< The dimensionality of the cuboid */

  HyperPoint _lowCorner;  /**< The lower  corner of the cuboid */
  HyperPoint _highCorner; /**< The higher corner of the cuboid */
  
  
  void updateFaceCash() const;

  public:
    
  const int& getDimension () const {return _dimension;}
  /**< get the dimensionality */

  HyperCuboid(int dimension);
  HyperCuboid(const HyperPoint& lowCorner, const HyperPoint& highCorner);
  HyperCuboid(int dimension, double low, double high);


  bool inVolume(const HyperPoint& coords) const;


  

  bool        inVolume(const HyperPoint& coords, std::vector<int> dims) const;

  const HyperPoint& getLowCorner () const{ return _lowCorner ; }
  /**< return the low HyperPoint corner */
  const HyperPoint& getHighCorner() const{ return _highCorner; }
  /**< return the high HyperPoint corner */
  HyperPoint& getLowCorner () { return _lowCorner ; }
  /**< return the low HyperPoint corner */
  HyperPoint& getHighCorner() { return _highCorner; }
  /**< return the high HyperPoint corner */


  bool operator ==(const HyperCuboid& other) const;
  bool operator !=(const HyperCuboid& other) const;

  ~HyperCuboid() = default;

};



#endif

