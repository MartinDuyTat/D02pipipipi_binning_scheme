/**
 * <B>HyperPlot</B>,
 * Author: Sam Harnew, sam.harnew@gmail.com ,
 * Date: Dec 2015
 *
 * HyperVolume is made up from a vector of HyperCuboids
 *
 **/

 
#ifndef HYPERVOLUME_HH
#define HYPERVOLUME_HH

class HyperVolume;

// HyperPlot includes
#include "HyperPoint.h"
#include "HyperCuboid.h"

// Root includes

// std includes


class HyperVolume {
  
  private:

  int _dimension;                          /**< Dimensionality of the HyperVolume */

  std::vector<HyperCuboid> _hyperCuboids;  /**< vector containing HyperCuboids that define the HyperVolume */


  public:

  HyperVolume(int dimension); 

  const int& getDimension () const {return _dimension;}
  /**< get the dimensionality of the HyperVolume */

  void addHyperCuboid(const HyperPoint& lowCorner, const HyperPoint& highCorner);
  void addHyperCuboid(const HyperCuboid& hyperCuboid);
  
  /**< return the std::vector containing the HyperCuboids */

  const HyperCuboid&              at            (int i) const{return _hyperCuboids.at(i);}
  HyperCuboid&              at            (int i){return _hyperCuboids.at(i);}

  /**< return one of the HyperCuboids */

  /**< return the number of HyperCuboids that make up the HyperVolume */

  HyperVolume & operator= (const HyperVolume & other) = delete;

  bool inVolume(const HyperPoint& coords) const;

  double getMin(int dimension) const;
  double getMax(int dimension) const;
  
  HyperCuboid getLimits() const;

  ~HyperVolume() = default;

};



#endif

