#include "NumericalRecipes.hpp"

#include "OrbitalPhasing_T4.hpp"

#include "ODEIntegrator.hpp"
#include "VectorFunctions.hpp"
namespace WU = WaveformUtilities;
typedef int NRerror;
using WaveformUtilities::Output;
using WaveformUtilities::Odeint;
using WaveformUtilities::StepperDopr853;
using std::vector;

inline double CUB(const double x) { return x*x*x; }

class T4 {
private:
  double nu;
  double dvdt2, dvdt3, dvdt4, dvdt5, dvdt6, dvdt6Ln4v, dvdt7;

public:
  T4(const double delta, const double chis, const double chia)
    : nu((1.0-delta*delta)/4.0),
      dvdt2(-2.2113095238095237 - 2.75*nu),
      dvdt3(0.08333333333333333*(150.79644737231007 - 113.*chis - 113.*chia*delta + 76.*chis*nu)),
      dvdt4(0.00005511463844797178*(34103. + 91854.*pow(chia,2) + 91854.*pow(chis,2) + 183708.*chia*chis*delta + 122949.*nu - 362880.*pow(chia,2)*nu - 4536.*pow(chis,2)*nu + 59472.*pow(nu,2))),
      dvdt5(0.000496031746031746*(-39197.65153883985 - 63142.*chis - 4536.*pow(chia,2)*chis - 1512.*pow(chis,3) - 63142.*chia*delta - 1512.*pow(chia,3)*delta - 4536.*chia*pow(chis,2)*delta - 149627.77490517468*nu + 185312.*chis*nu + 13608.*pow(chia,2)*chis*nu + 4536.*pow(chis,3)*nu + 97860.*chia*delta*nu + 1512.*pow(chia,3)*delta*nu + 4536.*chia*pow(chis,2)*delta*nu - 53088.*chis*pow(nu,2))),
      dvdt6(2.385915084327783e-9*(6.745934508094527e10 + 4.022865e8*pow(chia,2) - 4.937716571870764e10*chis + 2.67141105e10*pow(chis,2) - 4.937716571870764e10*chia*delta + 5.3428221e10*chia*chis*delta + 2.6311824e10*pow(chia,2)*pow(delta,2) - 6.931556164404614e10*nu - 4.5561285e9*pow(chia,2)*nu + 3.247920233941658e10*chis*nu - 3.41136873e10*pow(chis,2)*nu - 3.70606698e10*chia*chis*delta*nu + 2.53066275e8*pow(nu,2) + 1.24340832e10*pow(chia,2)*pow(nu,2) + 8.8307604e9*pow(chis,2)*pow(nu,2) - 9.063285e8*pow(nu,3))),
      dvdt6Ln4v(-16.304761904761904),
      dvdt7(9.185773074661964e-6*(-374493.5522711713 + 4.104076111684791e6*pow(chia,2) - 1.0117628e7*chis - 7.116984e6*pow(chia,2)*chis + 4.104076111684791e6*pow(chis,2) - 6.87204e6*pow(chis,3) - 1.0117628e7*chia*delta - 6.87204e6*pow(chia,3)*delta + 8.208152223369582e6*chia*chis*delta - 2.061612e7*chia*pow(chis,2)*delta - 1.3499136e7*pow(chia,2)*chis*pow(delta,2) + 2.028259341047374e7*nu - 1.6416304446739163e7*pow(chia,2)*nu + 2.1545842e7*chis*nu + 3.1783752e7*pow(chia,2)*chis*nu + 4.440744e6*pow(chis,3)*nu + 1.5224886e7*chia*delta*nu + 2.6925696e7*pow(chia,3)*delta*nu + 8.319024e6*chia*pow(chis,2)*delta*nu + 2.0695681428494263e7*pow(nu,2) - 2.1492918e7*chis*pow(nu,2) - 1.5408792e7*pow(chia,2)*chis*pow(nu,2) - 49896.*pow(chis,3)*pow(nu,2) - 5.235426e6*chia*delta*pow(nu,2) + 13608.*pow(chia,3)*delta*pow(nu,2) + 40824.*chia*pow(chis,2)*delta*pow(nu,2) + 1.03068e6*chis*pow(nu,3)))
  { }

  void operator() (const double t, const vector<double>& y, vector<double>& dydt) {
    const double& v=y[0];
    dydt[0] = (6.4*nu)*CUB(CUB(v))
      * (1.0 + v*v*(dvdt2 + v*(dvdt3 + v*(dvdt4 + v*(dvdt5 + v*(dvdt6 + dvdt6Ln4v*log(4.0*v) + v*(dvdt7) ) ) ) ) ) );
    dydt[1]=CUB(v);
  }

  bool ContinueIntegrating(const double& t, const vector<double>& y, const vector<double>& dydt) const {
    return (dydt[0]>0.0 && y[0]<1.0);
  }

};

typedef bool (T4::*ContinueTest)(const double& t, const vector<double>& y, const vector<double>& dydt) const;

void WU::TaylorT4(const double delta, const double chis, const double chia, const double v0,
                  vector<double>& t, vector<double>& v, vector<double>& Phi,
                  const int nsave, const bool denseish)
{
  const double nu( (1.0-delta*delta)/4.0 );
  const double GuessedLength = 1.1 * 5.0/(256.0*nu*pow(v0,8));
  const double rtol=1.0e-11, atol=0.0, h1=1.0e2, hmin=1.0e-3, t0=-GuessedLength, t1=0.0;
  vector<double> ystart(2);
  ystart[0]=v0;
  ystart[1]=0.0;
  Output out(nsave);
  T4 d(delta, chis, chia);
  ContinueTest test = &T4::ContinueIntegrating;
  Odeint<StepperDopr853<T4> > ode(ystart,t0,t1,atol,rtol,h1,hmin,out,d,denseish,test);
  try {
    ode.integrate();
  } catch(NRerror err) { }

  out.xsave.resize(out.count);
  t.swap(out.xsave);
  out.ysave.resize(out.ysave.nrows(), out.count);
  v.swap(out.ysave[0]);
  Phi.swap(out.ysave[1]);
  t -= t.back();

  return;
}
