/**
 * <B>HyperPlot</B>,
 * Author: Sam Harnew, sam.harnew@gmail.com ,
 * Date: Dec 2015
 *
 * HyperBinning is a binning scheme where each bin volume is 
 * defined by a HyperVolume.
 *
 **/

/** \class HyperBinning

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



 
#ifndef HYPERBINNING_HH
#define HYPERBINNING_HH

// HyperPlot includes
#include "HyperPoint.h"
#include "HyperVolume.h"
#include "BinningBase.h"
#include "CachedVar.h"


// Root includes
#include "TRandom3.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TMath.h"
#include "TTree.h"
#include "TFile.h"

// std includes
#include <algorithm>
#include <sstream>

class HyperBinning : public BinningBase {

  private:
  
  //mutable bool _changed; 
  /**< 
    keep a record to check if the binning changes -
    if so _binNum, _hyperVolumeNumFromBinNum, _averageBinWidth,
    and _minmax need to be redetermined.
  */

  mutable CachedVar<HyperPoint> _averageBinWidth;
  /**< store the a Hyperpoint giving the average bin width in each dimension. This
  is calculated once, and reused. */

  mutable CachedVar<HyperCuboid> _minmax; 
  /**< store the HyperCuboid that surrounds the binning. This
  is calculated once, and reused. */

  mutable CachedVar<std::vector< int > >              _binNum; 
  /**< 
    Every HyperVolume has a Bin Number, although these will be -1
    if it's not a true bin, and just part of the bin hierarchy.
    These numbers run from 0 to nBins - 1. In the below example
             
    ~~~ {.cpp}
                  0   1   2   3   4   5   6   7   8
     _binNum = { -1, -1, -1, -1,  2,  3,  4,  0   1 }


           HyperVolume Numbers 
    
     |-------------0-------------| 
    
     |------1------|------2------| 
    
     |--3---|---4--|---5---|--6--|
    
     |-7-|-8| 
    
               Bin Numbers
    
     | 0 | 1|   2  |   3   |  4  |
    
    ~~~

  */

  mutable CachedVar< std::vector< int > >     _hyperVolumeNumFromBinNum;
  /**< 
    The opposite of the member _binNum. This gives the HyperVolume number
    from the Bin Number. In the below example
             
    ~~~ {.cpp}
                                    0   1   2   3   4  
     _hyperVolumeNumFromBinNum = {  7,  8,  4,  5,  6 }


           HyperVolume Numbers 
    
     |-------------0-------------| 
    
     |------1------|------2------| 
    
     |--3---|---4--|---5---|--6--|
    
     |-7-|-8| 
    
               Bin Numbers
    
     | 0 | 1|   2  |   3   |  4  |
    
    ~~~
  */

  protected:


  int followBinLinks(const HyperPoint& coords, int binNumber) const; 

  void updateCash() const; 
  void updateBinNumbering() const; 
  void updateMinMax() const;

  int getHyperBinningDimFromTree(TTree* tree);


  public:
  
  HyperBinning();
  

  //Used for getting between the bin number and HyperVolume numbers
  int getHyperVolumeNumber(int binNumber) const;
  int getBinNum(int volumeNumber) const;

  //virtual std::vector<int> getPrimaryVolumeNumbers() const;

  virtual ~HyperBinning() = default;

  /* Virtual functions that need to implemented in any derrived class */

  virtual void setDimension(int dim);

  virtual std::vector<int> getLinkedHyperVolumes( int volumeNumber ) const = 0;
  virtual HyperVolume getHyperVolume(int volumeNumber) const = 0; /**< get one of the HyperVolumes */
  //virtual void addPrimaryVolumeNumber(int volumeNumber) = 0;
  virtual bool addHyperVolume(const HyperVolume& hyperVolume, std::vector<int> linkedVolumes = std::vector<int>(0, 0)) = 0;
  virtual int getNumHyperVolumes() const = 0;  

  virtual int getNumPrimaryVolumes  () const = 0;  
  virtual int getPrimaryVolumeNumber(int i) const = 0;  



  /* Virtual functions that need to implemented from BinningBase */
  /*    These will be implemented in the derived classes */

  virtual void load(TString filename, TString option = "READ") = 0;
  virtual BinningBase* clone() const = 0;

  /*    These will be implemented in this class */


  //virtual void mergeBinnings( const BinningBase& other );

  virtual int getNumBins() const;
  virtual int getBinNum(const HyperPoint& coords) const;



  virtual HyperVolume getBinHyperVolume(int binNumber) const;

  virtual HyperCuboid getLimits()          const;



};



#endif

