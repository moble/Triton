#ifndef QUATERNIONS_HPP
#define QUATERNIONS_HPP

#include <vector>
#include <cmath>
#include <iostream>

// This file is taken from the GWFrames code I wrote for my paper on
// frames.  I've changed the namespace, and removed the final two
// functions.  Otherwise, though, I'm trying to make sure that they're
// just the same.

namespace WaveformUtilities {

  // The class for an individual quaternion
  class Quaternion {
  private:
    double w, x, y, z;
  public: // Constructors
    Quaternion();
    Quaternion(const Quaternion& Q);
    Quaternion(const double vartheta, const double varphi);
    Quaternion(const double alpha, const double beta, const double gamma);
    Quaternion(const double w0, const double x0, const double y0, const double z0);
    Quaternion(const std::vector<double>& q);
    Quaternion(const double angle, const std::vector<double>& axis);
  public: // Access and manipulation
    Quaternion& operator=(const Quaternion& Q) { w=Q.w; x=Q.x; y=Q.y; z=Q.z; return *this; }
    double operator[](const unsigned int i) const;
    double& operator[](const unsigned int i);
    inline bool operator!=(const Quaternion& Q) const { if(w!=Q.w) return false; if(x!=Q.x) return false; if(y!=Q.y) return false; if(z!=Q.z) return false; return true; }
    inline bool operator==(const Quaternion& Q) const { return !((*this) != Q); }
    inline Quaternion operator-() const { return Quaternion(-w, -x, -y, -z); }
    inline Quaternion operator+(const double t) const { return Quaternion(w+t, x, y, z); }
    inline Quaternion operator-(const double t) const { return Quaternion(w-t, x, y, z); }
    inline Quaternion operator*(const double t) const { return Quaternion(w*t, x*t, y*t, z*t); }
    inline Quaternion operator/(const double t) const { return Quaternion(w/t, x/t, y/t, z/t); }
    inline Quaternion operator+(const Quaternion& Q) const { return Quaternion(w+Q.w,x+Q.x,y+Q.y,z+Q.z); }
    inline Quaternion operator-(const Quaternion& Q) const { return Quaternion(w-Q.w,x-Q.x,y-Q.y,z-Q.z); }
    Quaternion operator*(const Quaternion& Q) const;
    inline Quaternion operator/(const Quaternion& Q) const { return (*this)*(Q.inverse()); }
    inline Quaternion pow(const double t) const { return (this->log() * t).exp(); }
    inline Quaternion pow(const Quaternion& Q) const { return (this->log() * Q).exp(); }
    inline double     dot(const Quaternion& Q) const { return x*Q.x+y*Q.y+z*Q.z; }
    inline double     abs() const { return std::sqrt(normsquared()); }
    Quaternion log() const;
    Quaternion exp() const;
    inline Quaternion sqrt() const { const double Abs = abs(); return (*this/Abs+1)*std::sqrt(Abs/(2+2*w/Abs)); }
    inline double     angle() const { return 2*log().abs(); }
    inline Quaternion inverse() const { return conjugate()/normsquared(); }
    inline Quaternion conjugate() const { return Quaternion(w, -x, -y, -z); }
    inline Quaternion normalized() const { return (*this)/abs(); }
    inline double     normsquared() const { return (w*w+x*x+y*y+z*z); }
    inline Quaternion sqrtOfRotor() const { return (*this+1)/std::sqrt(2+2*w); }
    inline std::vector<double> vec() const { std::vector<double> v(3); v[0]=x; v[1]=y; v[2]=z; return v; }
  };
  // Helper (non-member) functions for single Quaternions follow
  inline Quaternion operator+(const double a, const Quaternion& Q) { return Q+a; }
  inline Quaternion operator-(const double a, const Quaternion& Q) { return Q-a; }
  inline Quaternion operator*(const double a, const Quaternion& Q) { return Q*a; }
  inline Quaternion operator/(const double a, const Quaternion& Q) { return Q/a; }
  inline Quaternion pow(const Quaternion& Q, const double x) { return Q.pow(x); }
  inline Quaternion pow(const Quaternion& Q, const Quaternion& P) { return Q.pow(P); }
  inline double     dot(const Quaternion& Q, const Quaternion& P) { return Q.dot(P); }
  inline double     abs(const Quaternion& Q) { return Q.abs(); }
  inline Quaternion log(const Quaternion& Q) { return Q.log(); }
  inline Quaternion exp(const Quaternion& Q) { return Q.exp(); }
  inline Quaternion sqrt(const Quaternion& Q) { return Q.sqrt(); }
  inline double     angle(const Quaternion& Q) { return Q.angle(); }
  inline Quaternion inverse(const Quaternion& Q) { return Q.inverse(); }
  inline Quaternion conjugate(const Quaternion& Q) { return Q.conjugate(); }
  inline Quaternion normalized(const Quaternion& Q) { return Q.normalized(); }
  inline double     normsquared(const Quaternion& Q) { return Q.normsquared(); }
  inline Quaternion sqrtOfRotor(const Quaternion& Q) { return Q.sqrtOfRotor(); }
  inline Quaternion Slerp(const double tau, const Quaternion& Qa, const Quaternion& Qb) { return ( pow((Qb/Qa), tau) * Qa ); }
  std::ostream& operator<<(std::ostream& out, const WaveformUtilities::Quaternion& q);


  // Functions for arrays of Quaternion objects
  std::vector<Quaternion> CenteredDifferencing(const std::vector<Quaternion>& QIn, const std::vector<double>& tIn);
  std::vector<Quaternion> MinimalRotation(const std::vector<Quaternion>& R, const std::vector<double>& T, const unsigned int NIterations=5);
  std::vector<Quaternion> FrameFromXY(const std::vector<Quaternion>& X, const std::vector<Quaternion>& Y);
  std::vector<Quaternion> FrameFromZ(const std::vector<Quaternion>& Z, const std::vector<double>& T, const unsigned int NIterations=5);
  std::vector<Quaternion> UnflipRotors(const std::vector<Quaternion>& R, const double discont=1.4142135623730951);
  std::vector<Quaternion> RDelta(const std::vector<Quaternion>& R1, const std::vector<Quaternion>& R2, const unsigned int IndexOfFiducialTime=0);
  std::vector<Quaternion> Squad(const std::vector<Quaternion>& RIn, const std::vector<double>& tIn, const std::vector<double>& tOut);
  std::vector<Quaternion> operator+(const double a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator-(const double a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator*(const double a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator/(const double a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator+(const std::vector<double>& a, const Quaternion& Q);
  std::vector<Quaternion> operator-(const std::vector<double>& a, const Quaternion& Q);
  std::vector<Quaternion> operator*(const std::vector<double>& a, const Quaternion& Q);
  std::vector<Quaternion> operator/(const std::vector<double>& a, const Quaternion& Q);
  std::vector<Quaternion> operator+(const std::vector<double>& a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator-(const std::vector<double>& a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator*(const std::vector<double>& a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator/(const std::vector<double>& a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator+(const Quaternion& a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator-(const Quaternion& a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator*(const Quaternion& a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator/(const Quaternion& a, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator+(const std::vector<Quaternion>& Q, const Quaternion& a);
  std::vector<Quaternion> operator-(const std::vector<Quaternion>& Q, const Quaternion& a);
  std::vector<Quaternion> operator*(const std::vector<Quaternion>& Q, const Quaternion& a);
  std::vector<Quaternion> operator/(const std::vector<Quaternion>& Q, const Quaternion& a);
  std::vector<Quaternion> operator+(const std::vector<Quaternion>& Q, const double a);
  std::vector<Quaternion> operator-(const std::vector<Quaternion>& Q, const double a);
  std::vector<Quaternion> operator*(const std::vector<Quaternion>& Q, const double a);
  std::vector<Quaternion> operator/(const std::vector<Quaternion>& Q, const double a);
  std::vector<Quaternion> operator+(const Quaternion& Q, const std::vector<double>& a);
  std::vector<Quaternion> operator-(const Quaternion& Q, const std::vector<double>& a);
  std::vector<Quaternion> operator*(const Quaternion& Q, const std::vector<double>& a);
  std::vector<Quaternion> operator/(const Quaternion& Q, const std::vector<double>& a);
  std::vector<Quaternion> operator+(const std::vector<Quaternion>& Q, const std::vector<double>& a);
  std::vector<Quaternion> operator-(const std::vector<Quaternion>& Q, const std::vector<double>& a);
  std::vector<Quaternion> operator*(const std::vector<Quaternion>& Q, const std::vector<double>& a);
  std::vector<Quaternion> operator/(const std::vector<Quaternion>& Q, const std::vector<double>& a);
  std::vector<Quaternion> operator+(const std::vector<Quaternion>& P, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator-(const std::vector<Quaternion>& P, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator*(const std::vector<Quaternion>& P, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> operator/(const std::vector<Quaternion>& P, const std::vector<Quaternion>& Q);
  std::vector<Quaternion> pow(const std::vector<Quaternion>& Q, const double x);
  std::vector<Quaternion> pow(const std::vector<Quaternion>& Q, const Quaternion& P);
  std::vector<Quaternion> pow(const Quaternion& Q, const std::vector<double>& x);
  std::vector<Quaternion> pow(const Quaternion& Q, const std::vector<Quaternion>& P);
  std::vector<Quaternion> pow(const std::vector<Quaternion>& Q, const std::vector<double>& x);
  std::vector<Quaternion> pow(const std::vector<Quaternion>& Q, const std::vector<Quaternion>& P);
  std::vector<double>     abs(const std::vector<Quaternion>& Q);
  std::vector<Quaternion> log(const std::vector<Quaternion>& Q);
  std::vector<Quaternion> exp(const std::vector<Quaternion>& Q);
  std::vector<Quaternion> sqrt(const std::vector<Quaternion>& Q);
  std::vector<double>     angle(const std::vector<Quaternion>& Q);
  std::vector<Quaternion> inverse(const std::vector<Quaternion>& Q);
  std::vector<Quaternion> conjugate(const std::vector<Quaternion>& Q);
  std::vector<Quaternion> normalized(const std::vector<Quaternion>& Q);
  std::vector<double>     normsquared(const std::vector<Quaternion>& Q);
  std::vector<Quaternion> sqrtOfRotor(const std::vector<Quaternion>& Q);
  std::vector<Quaternion> Quaternions(const std::vector<double>& vartheta, const std::vector<double>& varphi);
  std::vector<Quaternion> Quaternions(const std::vector<double>& alpha, const std::vector<double>& beta, const std::vector<double>& gamma);
  std::vector<Quaternion> Quaternions(const std::vector<double>& w0, const std::vector<double>& x0, const std::vector<double>& y0, const std::vector<double>& z0);
  std::vector<Quaternion> Quaternions(const std::vector<std::vector<double> >& q);
  std::vector<Quaternion> Quaternions(const std::vector<double>& angle, const std::vector<std::vector<double> >& axis);
  std::vector<double> Component(const std::vector<WaveformUtilities::Quaternion>& Q, const unsigned int i);
  std::vector<double> Component0(const std::vector<WaveformUtilities::Quaternion>& Q);
  std::vector<double> Component1(const std::vector<WaveformUtilities::Quaternion>& Q);
  std::vector<double> Component2(const std::vector<WaveformUtilities::Quaternion>& Q);
  std::vector<double> Component3(const std::vector<WaveformUtilities::Quaternion>& Q);

} // namespace WaveformUtilities

#endif // QUATERNIONS_HPP
