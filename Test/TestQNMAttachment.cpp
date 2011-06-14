#include "NumericalRecipes.hpp"

#include <fstream>
#include <iomanip>
#include <ctime>

#include "Waveform.hpp"
#include "PostNewtonian.hpp"
#include "EasyParser.hpp"
using WaveformObjects::Waveform;
using namespace std;

int main() {
  const bool WriteFiles = true;
  
  const double q = 1;
  const double delta=(q-1.0)/(q+1.0);
  const double chis = 0.0;//-0.9900;
  const double chia = 0.0;
  const double v0 = 0.12;//0.02025;
  vector<double> t, v, Phi;
  const int nsave = 20;
  const bool denseish = true;
  cout << "v0 = " << v0 << "\tOmega0 = " << v0*v0*v0 << "\tdelta = " << delta << "\tnu = " << (1.0-delta*delta)/4.0 << "\tchis = " << chis << endl;
  
  clock_t start,end;
  
//   for(double rtol=1e-5; rtol>1.01e-10; rtol/=10) {
  const double rtol=1e-10;
    start = clock();
    Waveform W("EOB", delta, chis, chia, v0, WaveformUtilities::QNMLMs(), nsave, denseish);
    end = clock();
    cout << "rtol=" << rtol << " took " << setprecision(10) << double(end-start)/double(CLOCKS_PER_SEC) << " seconds." << flush;
    start = clock();
    W.AttachQNMs(delta, WaveformUtilities::FinalSpinApproximation(delta, chis));
    end = clock();
    cout << "QNM attachment took " << setprecision(10) << double(end-start)/double(CLOCKS_PER_SEC) << " seconds." << flush;
    if(WriteFiles) {
      ofstream ofs("Outputs/TestQNMAttachment.dat");
      ofs << W;
      ofs.close();
    }
    cout << "  t.size()=" << t.size();
    cout << "  ☺\n" << endl;
//   }
  
  return 0;
}
