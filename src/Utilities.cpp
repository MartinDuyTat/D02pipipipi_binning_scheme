// Martin Duy Tat 23rd October 2022, based on examples from Xiaodong Shi

#include<vector>
#include<algorithm>
#include<numeric>
#include"TLorentzVector.h"
#include"TVector3.h"
#include"Utilities.h"

namespace Utilities {

  double getPhi(std::array<TLorentzVector, 4> daughters) {
  
    // Find four-vector of mother particle and its boost

    const TLorentzVector mother = std::accumulate(daughters.begin(),
						  daughters.end(),
						  TLorentzVector{});
    const TVector3 boosttomother = -(mother.BoostVector());

    // Boost daughters to the COM frame

    std::for_each(daughters.begin(), daughters.end(),
		  [&](auto &a){a.Boost(boosttomother);});
  
    // Get the three-vectors of each particle

    std::array<TVector3, 4> daughtersVect;

    std::transform(daughters.begin(), daughters.end(), daughtersVect.begin(),
		   [](const auto &a){return a.Vect();});
  
    // Imagine the decay proceeds via D -> AB, A->A1A2, B->B1B2
    // Find the unit three-vectors of A and B
  
    const TVector3 Avect = (daughtersVect[0] + daughtersVect[1]).Unit();
    const TVector3 Bvect = (daughtersVect[2] + daughtersVect[3]).Unit();
  
    // Find the unit vector that is normal to A1 and A2

    const TVector3 normAvect = (daughtersVect[0].Cross(daughtersVect[1])).Unit();

    // Find the unit vector that is normal to B1 and B2

    const TVector3 normBvect = (daughtersVect[2].Cross(daughtersVect[3])).Unit();
  
    // Find the cosine and sine of the angle phi (angle between decay planes)

    const double cosPhi = normAvect.Dot(normBvect);
    const double sinPhi = (normAvect.Cross(normBvect)).Dot(Bvect);
  
    // Find phi from sin phi and cos phi

    const double phi = atan2(sinPhi,cosPhi);
   
    return phi;
  
  }

  double getCosTheta(TLorentzVector particle,
		     const TLorentzVector &parent,
		     TLorentzVector grandparent) {
    
    // Boost the four-vector of the particle and its grandparent (the d meson)
    // to the parent COM frame

    const TVector3 boosttoparent = -(parent.BoostVector());
     
    particle.Boost(boosttoparent);
    grandparent.Boost(boosttoparent);
     
    // Get the three-vector of the particle and its grandparent
    const TVector3 particle3 = particle.Vect();
    const TVector3 grandparent3 = grandparent.Vect();
    
    // Evaluate cos theta
    const double numerator = particle3.Dot(grandparent3);
    const double denominator = (particle3.Mag())*(grandparent3.Mag());

    const double costheta = numerator/denominator;
     
    return costheta;

  }
  
}  
