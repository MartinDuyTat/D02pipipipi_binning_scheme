/**
 * <B>HyperPlot</B>,
 * Author: Sam Harnew, sam.harnew@gmail.com ,
 * Date: Dec 2015
 *
 * The base class for any histogram object (my version of a TH1)  
 *
 **/

 
#ifndef HISTOGRAMBASE_HH
#define HISTOGRAMBASE_HH

// HyperPlot includes

// Root includes
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TRandom3.h"  
#include "TMath.h"

// std includes


class HistogramBase {
  
  protected:
  
  int     _nBins;                   /**< Number of bins in the histogram */
  std::vector<double> _binContents; /**< Bin contents (note that bin nBins is underflow/overflow) */
  std::vector<double> _sumW2;       /**< Sum of weights^2 for each bin (note that bin nBins is underflow/overflow) */

  double _min;         /**< Minimum bin content (for plotting) */
  double _max;         /**< Maximum bin content (for plotting) */

  public:

  HistogramBase(int nBins);
  virtual ~HistogramBase() = default;
  
  int checkBinNumber(int bin) const;

  void resetBinContents(int nBins);

  double getBinContent(int bin) const;

  void loadBase(TString filename);

};

#endif

