/**
 * <B>HyperPlot</B>,
 * Author: Sam Harnew, sam.harnew@gmail.com ,
 * Date: Dec 2015
 *
 * Used for variables we want to cache. Provides a flag that tells us
 * when the cached variable needs updating
 **/

 
#ifndef CACHED_VAR_HH
#define CACHED_VAR_HH

// HyperPlot includes


// Root includes


// std includes


template <class T> 
class CachedVar {
  
  private:

  bool       _needsUpdate;
  T          _cachedVar;
  
  public:

  CachedVar(const T& var = T());
  
  void changed(){
    _needsUpdate = true;
  }
  void updated(){
    _needsUpdate = false;
  }
  
  T& get(){return _cachedVar;}
  const T& get() const{return _cachedVar;}

  CachedVar& operator=(const T& other){
    _cachedVar = other;
    return *this;
  }

  operator const T&() {
    return _cachedVar;
  }

  bool isUpdateNeeded(){
    return _needsUpdate;
  }

  ~CachedVar(){

  }
  

};

template <class T> CachedVar<T>::CachedVar (const T& var) :
  _needsUpdate(true),
  _cachedVar(var)
{ 
    
} 


#endif

