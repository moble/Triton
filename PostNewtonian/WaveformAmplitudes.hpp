#ifndef WAVEFORMAMPLITUDES_HPP
#define WAVEFORMAMPLITUDES_HPP

#include <vector>

namespace WaveformUtilities {
  
  class WaveformAmplitudes {
  private:
    double delta, nu, chis, NormalizationFactor;
    double Hhat_L2_M2_Re_v0, Hhat_L2_M2_Re_v2, Hhat_L2_M2_Re_v3, Hhat_L2_M2_Re_v4, Hhat_L2_M2_Re_v5, Hhat_L2_M2_Im_v5,
      Hhat_L2_M2_Re_v6, Hhat_L2_M2_Im_v6, Hhat_L2_M2_Re_v6lnv, Hhat_L2_M1_Im_v1, Hhat_L2_M1_Im_v3, Hhat_L2_M1_Re_v4,
      Hhat_L2_M1_Im_v4, Hhat_L2_M1_Im_v5, Hhat_L2_M1_Re_v6, Hhat_L2_M1_Im_v6, Hhat_L2_M0_Re_v0, Hhat_L3_M3_Im_v1,
      Hhat_L3_M3_Im_v3, Hhat_L3_M3_Re_v4, Hhat_L3_M3_Im_v4, Hhat_L3_M3_Im_v5, Hhat_L3_M3_Re_v6, Hhat_L3_M3_Im_v6,
      Hhat_L3_M2_Re_v2, Hhat_L3_M2_Re_v4, Hhat_L3_M2_Re_v5, Hhat_L3_M2_Im_v5, Hhat_L3_M2_Re_v6, Hhat_L3_M1_Im_v1,
      Hhat_L3_M1_Im_v3, Hhat_L3_M1_Re_v4, Hhat_L3_M1_Im_v4, Hhat_L3_M1_Im_v5, Hhat_L3_M1_Re_v6, Hhat_L3_M1_Im_v6,
      Hhat_L3_M0_Im_v5, Hhat_L4_M4_Re_v2, Hhat_L4_M4_Re_v4, Hhat_L4_M4_Re_v5, Hhat_L4_M4_Im_v5, Hhat_L4_M4_Re_v6,
      Hhat_L4_M3_Im_v3, Hhat_L4_M3_Im_v5, Hhat_L4_M3_Re_v6, Hhat_L4_M3_Im_v6, Hhat_L4_M2_Re_v2, Hhat_L4_M2_Re_v4,
      Hhat_L4_M2_Re_v5, Hhat_L4_M2_Im_v5, Hhat_L4_M2_Re_v6, Hhat_L4_M1_Im_v3, Hhat_L4_M1_Im_v5, Hhat_L4_M1_Re_v6,
      Hhat_L4_M1_Im_v6, Hhat_L4_M0_Re_v0, Hhat_L5_M5_Im_v3, Hhat_L5_M5_Im_v5, Hhat_L5_M5_Re_v6, Hhat_L5_M5_Im_v6,
      Hhat_L5_M4_Re_v4, Hhat_L5_M4_Re_v6, Hhat_L5_M3_Im_v3, Hhat_L5_M3_Im_v5, Hhat_L5_M3_Re_v6, Hhat_L5_M3_Im_v6,
      Hhat_L5_M2_Re_v4, Hhat_L5_M2_Re_v6, Hhat_L5_M1_Im_v3, Hhat_L5_M1_Im_v5, Hhat_L5_M1_Re_v6, Hhat_L5_M1_Im_v6,
      Hhat_L6_M6_Re_v4, Hhat_L6_M6_Re_v6, Hhat_L6_M5_Im_v5, Hhat_L6_M4_Re_v4, Hhat_L6_M4_Re_v6, Hhat_L6_M3_Im_v5,
      Hhat_L6_M2_Re_v4, Hhat_L6_M2_Re_v6, Hhat_L6_M1_Im_v5, Hhat_L7_M7_Im_v5, Hhat_L7_M6_Re_v6, Hhat_L7_M5_Im_v5,
      Hhat_L7_M4_Re_v6, Hhat_L7_M3_Im_v5, Hhat_L7_M2_Re_v6, Hhat_L7_M1_Im_v5, Hhat_L8_M8_Re_v6, Hhat_L8_M6_Re_v6,
      Hhat_L8_M4_Re_v6, Hhat_L8_M2_Re_v6;
    
  public:
    WaveformAmplitudes(const double idelta, const double ichis);
    void Hhat(const int L, const int M, const double v, double& Re, double& Im) const;
    void rhOverM(const int L, const int M, const double v, const double psi, double& Mag, double& Arg) const;
    void rhOverM(const int L, const int M, const std::vector<double>& v, const std::vector<double>& psi, std::vector<double>& Mag, std::vector<double>& Arg) const;
  };
  
}


#endif // WAVEFORMAMPLITUDES_HPP
