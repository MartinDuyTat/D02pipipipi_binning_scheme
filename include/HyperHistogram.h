/**
  <B>HyperPlot</B>,
  Author: Sam Harnew, sam.harnew@gmail.com ,
  Date: Dec 2015
 

 Binning Algorithms:

  - HyperBinningAlgorithms::SMART             (see HyperBinningMakerSmart for details on the algorithm          )
  - HyperBinningAlgorithms::MINT              (see HyperBinningMakerMint for details on the algorithm           )
  - HyperBinningAlgorithms::MINT_SMART        (see HyperBinningMakerMintSmart for details on the algorithm      )
  - HyperBinningAlgorithms::MINT_RANDOM       (see HyperBinningMakerMintRandomise for details on the algorithm  )
  - HyperBinningAlgorithms::SMART_RANDOM      (see HyperBinningMakerSmartRandomise for details on the algorithm )
  - HyperBinningAlgorithms::LIKELIHOOD        (see HyperBinningMakerLikelihood for details on the algorithm     )
  - HyperBinningAlgorithms::SMART_LIKELIHOOD  (see HyperBinningMakerSmartLikelihood for details on the algorithm)

Binning Algorithm Options:

  - AlgOption::StartDimension     (int dim                  )
  - AlgOption::BinningDimensions  (std::vector<int> dims    )
  - AlgOption::RandomSeed         (int seed                 )
  - AlgOption::MinBinWidth        (double width             )
  - AlgOption::MinBinWidth        (HyperPoint widths        ) 
  - AlgOption::MinBinContent      (double val               )
  - AlgOption::MinShadowBinContent(double val               )
  - AlgOption::UseWeights         (bool   val = true        )
  - AlgOption::UseShadowData      (const HyperPointSet& data)
  - AlgOption::Empty              (                         )
 
 **/
 
#ifndef HYPERHISTOGRAM_HH
#define HYPERHISTOGRAM_HH

// HyperPlot includes
#include "HistogramBase.h"
#include "BinningBase.h"
#include "HyperBinning.h"
#include "HyperBinningMemRes.h"

// Root includes
#include "TRandom.h"

// std includes
#include <iostream>
#include <fstream>
#include <iomanip>

class HyperHistogram : public HistogramBase {

  protected:

  BinningBase* _binning; /**< The HyperVolumeBinning used for the HyperHistogram */
  
  public:
  
  HyperHistogram(TString filename, TString option = "MEMRES READ");

  HyperHistogram(const HyperHistogram& other) = delete;

  HyperHistogram& operator=(const HyperHistogram& other) = delete;

  int getDimension() const;
  
  virtual double getVal(const HyperPoint& point) const;

  TString getBinningType(TString filename);

  void load     (TString filename, TString option = "MEMRES READ");

  virtual ~HyperHistogram();

};

#endif

