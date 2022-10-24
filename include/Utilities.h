// Martin Duy Tat 23rd October 2022, based on examples by Xiaodong Shi
/**
 * Useful functions for converting between various coordinates
 */

#ifndef UTILITIES
#define UTILITIES

#include<array>
#include"TLorentzVector.h"

namespace Utilities {
  /**
   * Determine the angle phi between the pi+pi+ and pi-pi- decay planes
   * @param daughters 4-momentum of the daughters in the order pi+ pi+ pi- pi-
   */
  double getPhi(std::array<TLorentzVector, 4> daughters);
  /**
   * Determine helicity angle
   * @param particle 4-momentum of particle we're interested in
   * @param parent 4-momentum of parent
   * @param grandparent 4-momentum of grand parent
   */
  double getCosTheta(TLorentzVector particle,
		     const TLorentzVector &parent,
		     TLorentzVector grandparent);
};

#endif
