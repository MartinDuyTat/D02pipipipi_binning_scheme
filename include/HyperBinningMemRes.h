/**
 * <B>HyperPlot</B>,
 * Author: Sam Harnew, sam.harnew@gmail.com ,
 * Date: Dec 2015
 *
 * HyperBinningMemRes is a binning scheme where each bin volume is 
 * defined by a HyperVolume.
 *
 **/

/** \class HyperBinningMemRes

Finding the correct bin number is quite a compuationally
slow process if one has to loop over every bin and
check if a HyperPoint falls within that bin volume. It's not unusual to
have millions of HyperPoints that need to be sorted into tens of thousands of bins. 
This would require billions of calulations.
To speed this process up there is the option to build a hierarchy of bins. A schematic below
shows a 1D example. 

~~~ {.cpp}

       HyperVolume Numbers 

 |-------------0-------------| 

 |------1------|------2------| 

 |--3---|---4--|---5---|--6--|

 |-7-|-8| 

           Bin Numbers

 | 0 | 1|   2  |   3   |  4  |

~~~

Imagine we have a HyperPoint that falls into Bin 0. One would first check if it 
falls into HyperVolume 0 (note the distiction here between Bin/HyperVolume Numbers as
indicated by the figure). First we check if it falls into HyperVolume 0, then HyperVolume 1 or 2, then 
HyperVolume 3 or 4, then HyperVolume 7 or 8. 

In this simple example, it took 7 operations, whereas checking each Bin would have taken 
5. Clearly as the number of bins increases, it becomes computationally much less 
expensive to follow this hierarchy approach.

The 

*/

 
#ifndef HYPERBINNINGMEMRES_HH
#define HYPERBINNINGMEMRES_HH

// HyperPlot includes
#include "HyperPoint.h"
#include "HyperCuboid.h"
#include "HyperVolume.h"
#include "HyperBinning.h"


// Root includes
#include "TRandom3.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TMath.h"

// std includes
#include <algorithm>
#include <sstream>

class HyperBinningMemRes : public HyperBinning {

  protected:

  std::vector< HyperVolume      > _hyperVolumes;
  /**< 
    std::vector containing all of the HyperVolumes. Usually, not all of these are bins,
    but part of the bin hierarchy that is discussed in the class description.
  */
  
  std::vector< int > _primaryVolumeNumbers;
  /**<
    Usually all bins will be accessed through one primary volume i.e. volume 0 in
    the below example. If one wants to merge two binning schemes together e.g. 
    Binning1 and Binning2 this involes appending the list of HyperVolumes from
    Binning2 to Binning1. This brings a problem when trying to sort a HyperPoint
    into a specific bin i.e. getBinNumber(HyperPoint) - if it belongs to Binning2, 
    it will first have to check that it doesn't fall into any of the volumes in Binning1. 

    To remedy this problem we introduce the list of primary volumes. If this list exists
    i.e. its size is greater than 0, all the primary volumes will be checked first. The
    existance of this list also implies that ALL volumes are linked to the primary volumes.  


           HyperVolume Numbers 
    
     |-------------0-------------| 
    
     |------1------|------2------| 
    
     |--3---|---4--|---5---|--6--|
    
     |-7-|-8| 

  */

  std::vector< std::vector<int> > _linkedHyperVolumes; 
  /**< 
    Every HyperVolume has a _linkedHyperVolumes - if this is empty, this
    means the HyperVolume is a true Bin. If not it is part of the binning hierarchy.
    In the below, HyperVolume 0 would be linked to HyperVolume [1,2], although 
    HyperVolume 4 would be linked to nothing.

    ~~~ {.cpp}
    
           HyperVolume Numbers 
    
     |-------------0-------------| 
    
     |------1------|------2------| 
    
     |--3---|---4--|---5---|--6--|
    
     |-7-|-8| 
    
               Bin Numbers
    
     | 0 | 1|   2  |   3   |  4  |
    
    ~~~

  */

  void setBranchAddresses   (TTree* tree, int* binNumber, double* lowCorner, double* highCorner, std::vector<int>** linkedBins) const;
  
  void loadPrimaryVolumeNumbers(TFile* file);
  
  public:
  
  HyperBinningMemRes() = default;

  virtual ~HyperBinningMemRes() = default;

  //Functions we are required (or choose to) to implement (override) from HyperBinning

  virtual void setDimension(int dim);

  virtual void addPrimaryVolumeNumber(int volumeNumber);
  virtual bool addHyperVolume(const HyperVolume& hyperVolume, std::vector<int> linkedVolumes = std::vector<int>(0, 0));
  
  virtual int getNumHyperVolumes() const;  
  virtual HyperVolume getHyperVolume(int volumeNumber) const; /**< get one of the HyperVolumes */
  virtual std::vector<int> getLinkedHyperVolumes( int volumeNumber ) const;

  virtual int getNumPrimaryVolumes  (     ) const;  
  virtual int getPrimaryVolumeNumber(int i) const;  


  //Functions we are required to implement from BinningBase that were not implemented in HyperBinning

  virtual void load(TString filename, TString option = "READ");

  virtual BinningBase* clone() const;


};



#endif
