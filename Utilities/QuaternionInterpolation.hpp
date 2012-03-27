#ifndef QuaternionInterpolation
#define QuaternionInterpolation

#include "Interpolate.hpp"
#include "Quaternions.hpp"
#include <vector>

namespace WaveformUtilities {
  
  //// Slerp is spherical linear interpolation of quaternions.  The
  //// input value t∈[0,1] gives the extent of interpolation from q0
  //// to q1.  If you only have two Quaternions (as in hybridization),
  //// this is the method to choose.
  WaveformUtilities::Quaternion Slerp(double t, const WaveformUtilities::Quaternion& q0, const WaveformUtilities::Quaternion& q1, bool shortest=true);
  
  //// Squad interpolates between Quaternions, paying attention to the
  //// speed of the curve leaving q0 and arriving at q1.  This is
  //// useful, e.g., when there are more than 2 Quaternions.  We can
  //// calculate the appropriate speed when coming into and going out
  //// from q0 and q1.  The Squad function is basically a bezier curve
  //// for Quaternions --- c0 and c1 being the control points.
  WaveformUtilities::Quaternion Squad(double t,
				      const WaveformUtilities::Quaternion& q0,
				      const WaveformUtilities::Quaternion& c0,
				      const WaveformUtilities::Quaternion& c1,
				      const WaveformUtilities::Quaternion& q1);
  
  //// The following function performs Squad interpolation,
  //// automatically setting the appropriate speeds.
  std::vector<Quaternion> Squad(const std::vector<double>& tIn, const std::vector<Quaternion>& qIn, const std::vector<double>& tOut);
  
  //// This class inherits from the standard Interpolator base class,
  //// mostly for its hunt and locate functions.  The standard
  //// functions can't be used, because they expect to return doubles.
  class QuaternionInterpolator : Interpolator {
    const std::vector<Quaternion> &q;
    const std::vector<Quaternion> c;
  public:
    QuaternionInterpolator(const std::vector<double>& xv, const std::vector<WaveformUtilities::Quaternion>& Q)
      : Interpolator(xv,xv,2), q(Q), c(SetControlPoints(Q)) { }
    std::vector<Quaternion> SetControlPoints(const std::vector<Quaternion>& Q);
    WaveformUtilities::Quaternion Interpolate(double t) {
      int jlo = cor ? hunt(t) : locate(t);
      return RawInterpolate(jlo,t);
    }
    WaveformUtilities::Quaternion RawInterpolate(int jlo, double t);
    double rawinterp(int jlo, double x);
  };
  
  
} // namespace WaveformUtilities

#endif // QuaternionInterpolation
