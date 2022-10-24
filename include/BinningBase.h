/**
 * <B>HyperPlot</B>,
 * Author: Sam Harnew, sam.harnew@gmail.com ,
 * Date: Dec 2015
 *
 * The base class for any Binning object 
 *
 **/

 
#ifndef BINNINGBASE_HH
#define BINNINGBASE_HH

// HyperPlot includes
#include "HyperPoint.h"
#include "HyperVolume.h"


// Root includes


// std includes


class BinningBase {
  
  private:

  int       _dimension;
  /**< 
    Dimensionality of the binning. Can only be set ONCE with setDimension();
  */  

  TString _binningType; 
  /**< 
    Useful to know what kind of binning this actually is (will be set by derrived type)
  */  

  protected:
  
  virtual void setDimension (int dimension);  /**< set the dimensionality of the binning */
  void setBinningType(TString binningType);

  public:

  BinningBase();

  const int& getDimension () const;       /**< get the dimensionality of the binning */

  double getMin(int dimension) const;
  double getMax(int dimension) const;
  
  bool isSameBinningType(const BinningBase& other) const;

  virtual bool isDiskResident() const;

  virtual ~BinningBase() = default;
  
  //Purely virtual functions

  virtual void load(TString filename, TString option = "READ") = 0;


  virtual BinningBase* clone() const = 0;
  
  virtual int getNumBins() const = 0;
  virtual int getBinNum(const HyperPoint& coords) const = 0;

  virtual HyperVolume getBinHyperVolume(int binNumber) const = 0;

  //virtual void mergeBinnings( const BinningBase& other ) = 0;

  virtual HyperCuboid getLimits() const = 0;
 


};



#endif

