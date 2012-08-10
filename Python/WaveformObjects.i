// -*- c++ -*-

%module WaveformObjects
#pragma SWIG nowarn=401

%exception {
  try {
    $action;
  } catch(int i) {
    PyErr_SetString(PyExc_RuntimeError, "Unknown exception");
    return NULL;
  }
}

%include "WaveformObjects_Doc.i"

/////////////////////////////////////////////////
//// These will be needed by the c++ wrapper ////
/////////////////////////////////////////////////
%{
  #include <iostream>
  #include <sstream>
  #include <iomanip>
  #include "../Objects/WaveformAtAPoint.hpp"
  #include "../Objects/WaveformAtAPointFT.hpp"
  #include "../Objects/Waveforms.hpp"
  #include "../Utilities/Quaternions.hpp"
  #include "../Utilities/QuaternionInterpolation.hpp"
%}


//////////////////////////////////////////////////////////////////////
//// The following translates between c++ and python types nicely ////
//////////////////////////////////////////////////////////////////////
//// This lets me use numpy.array in the code below
%pythoncode %{
  import numpy;
  %}
//// Make sure std::strings are dealt with appropriately
%include "std_string.i"
//// Make sure std::vectors are dealt with appropriately
%include "std_vector.i"
namespace std {
  %template(vectori) vector<int>;
  %template(vectord) vector<double>;
  %template(vectors) vector<string>;
  %template(vectorvectori) vector<vector<int> >;
  %template(vectorvectord) vector<vector<double> >;
};
//// This should help python understand matrices
%ignore WaveformUtilities::Matrix::operator=;
%ignore WaveformUtilities::Matrix::operator[];
%rename(__getitem__) WaveformUtilities::Matrix<int>::operator[] const;
%rename(__getitem__) WaveformUtilities::Matrix<double>::operator[] const;
%include "../Utilities/Matrix.hpp"
namespace WaveformUtilities {
  %template(MatrixInt) Matrix<int>;
  %template(MatrixDouble) Matrix<double>;
};
//// I need to use my Quaternion class, to pass arguments into PyGW
%ignore WaveformUtilities::Quaternion::operator=;
%rename(__getitem__) WaveformUtilities::Quaternion::operator [](unsigned int const) const;
%rename(__setitem__) WaveformUtilities::Quaternion::operator [](unsigned int const);
%include "../Utilities/Quaternions.hpp"
%extend WaveformUtilities::Quaternion {
  //// This function is called when printing a Quaternion object
  std::string __str__() {
    std::stringstream S;
    S << std::setprecision(14) << "["
      << $self->operator[](0) << ", "
      << $self->operator[](1) << ", "
      << $self->operator[](2) << ", " 
      << $self->operator[](3) << "]";
    return S.str();
  }
 };
namespace std {
  %template(vectorq) vector<WaveformUtilities::Quaternion>;
}
%include "../Utilities/QuaternionInterpolation.hpp"

//////////////////////////////////////////////////////////////////////


////////////////////////////////////
//// Read in the Waveform class ////
////////////////////////////////////
//// Ignore things that don't translate well...
%ignore operator<<;
%ignore WaveformObjects::Waveform::operator=;
//// ...and rename things that do
%rename(__div__) WaveformObjects::Waveform::operator/;
%rename(__getitem__) WaveformObjects::Waveform::operator[] const;
//// These will convert the output data to numpy.ndarray for easier use
%feature("pythonappend") WaveformObjects::Waveform::T() const %{ if isinstance(val, tuple) : val = numpy.array(val) %}
%feature("pythonappend") WaveformObjects::Waveform::R() const %{ if isinstance(val, tuple) : val = numpy.array(val) %}
%feature("pythonappend") WaveformObjects::Waveform::Frame() const %{ if isinstance(val, tuple) : val = numpy.array(val) %}
%feature("pythonappend") WaveformObjects::Waveform::LM() const %{ if isinstance(val, tuple) : val = numpy.array(val) %}
%feature("pythonappend") WaveformObjects::Waveform::Mag() const %{ if isinstance(val, tuple) : val = numpy.array(val) %}
%feature("pythonappend") WaveformObjects::Waveform::Arg() const %{ if isinstance(val, tuple) : val = numpy.array(val) %}
%feature("pythonappend") WaveformObjects::Waveform::Omega2m2() const %{ if isinstance(val, tuple) : val = numpy.array(val) %}
%feature("pythonappend") WaveformObjects::Waveform::Flux() const %{ if isinstance(val, tuple) : val = numpy.array(val) %}
//// Parse the header file to generate wrappers
%include "../Objects/Waveform.hpp"
//// Make any additions to the Waveform class here
%extend WaveformObjects::Waveform {
  //// This function is called when printing the Waveform object
  std::string __str__() {
    std::stringstream S;
    S << ($self->HistoryStr()) << "###\n"
      << "### # In python:\n"
      << "### import PyGW\n"
      << "### print(this)" << std::endl << std::setprecision(14);
    for(unsigned int t=0; t<$self->NTimes(); ++t) {
      S << $self->T(t) << " ";
      for(unsigned int mode=0; mode<$self->NModes(); ++mode) {
	S << $self->Mag(mode, t) << " " << $self->Arg(mode, t) << " ";
      }
      S << std::endl;
    }
    return S.str();
  }
  //// Allow Waveform objects to be pickled
  %insert("python") %{
    def __getstate__(self) :
      return (self.HistoryStr(),
	      self.TypeIndex(),
	      self.TimeScale(),
	      self.T(),
	      self.R(),
	      self.Frame(),
	      self.LM(),
	      self.Mag(),
	      self.Arg()
	      )
    __safe_for_unpickling__ = True
    def __reduce__(self) :
        return (Waveform, (), self.__getstate__())
    def __setstate__(self, data) :
      self.SetHistory(data[0])
      self.SetTypeIndex(data[1])
      self.SetTimeScale(data[2])
      self.SetT(data[3])
      self.SetR(data[4])
      self.SetFrame(data[5])
      self.SetLM(MatrixInt(data[6].tolist()))
      self.SetMag(MatrixDouble(data[7]))
      self.SetArg(MatrixDouble(data[8]))
  %}
 };


////////////////////////////////////////////
//// Read in the WaveformAtAPoint class ////
////////////////////////////////////////////
//// Parse the header file to generate wrappers
%include "../Objects/WaveformAtAPoint.hpp"


//////////////////////////////////////////////
//// Read in the WaveformAtAPointFT class ////
//////////////////////////////////////////////
//// Parse the header file to generate wrappers
%include "../Objects/WaveformAtAPointFT.hpp"


/////////////////////////////////////
//// Read in the Waveforms class ////
/////////////////////////////////////
//// Make sure vectors of Waveform are understood
namespace std {
  %template(vectorW) vector<WaveformObjects::Waveform>;
};
//// Ignore this, as neither const nor non-const will work (copy constructor issues?)
%ignore WaveformObjects::Waveforms::operator[];
%rename(__getitem__) WaveformObjects::Waveforms::operator[] const;
//// Parse the header file to generate wrappers
%include "../Objects/Waveforms.hpp"
%extend WaveformObjects::Waveforms {
  void __setitem__(int i, const WaveformObjects::Waveform& W) {
    $self->operator[](i) = W;
    return;
  }
 };


//// Add a function to read the new h5 format for multiple radii
//// Note that this is defined in the PyGW namespace
%insert("python") %{

def PickChMass(File='Horizons.h5') :
    import h5py
    f=h5py.File(File)
    ChMass = f['AhA.dir/ChristodoulouMass.dat'][:,1]+f['AhB.dir/ChristodoulouMass.dat'][:,1]
    f.close()
    hist, bins = numpy.histogram(ChMass, bins=len(ChMass))
    return bins[hist.argmax()]

def ReadFiniteRadiusData(ChMass=1.0, Dir='.', File='rh_FiniteRadii_CodeUnits.h5') :
    import h5py
    import PyGW
    import re
    import numpy
    YlmRegex = re.compile(r"""Y_l(?P<L>[0-9]+)_m(?P<M>[-0-9]+)\.dat""")
    f = h5py.File(Dir+'/'+File, 'r')
    WaveformNames = list(f)
    NWaveforms = len(WaveformNames)
    Ws = PyGW.Waveforms(NWaveforms)
    TempW = PyGW.Waveform()
    for n in range(NWaveforms) :
        W = f[WaveformNames[n]]
        NTimes = W['AverageLapse.dat'].shape[0]
        T = W['AverageLapse.dat'][:,0]
        if( not (W['ArealRadius.dat'].shape[0]==NTimes) ) :
            raise ValueError("The number of time steps in this dataset should be {0}; ".format(NTimes) +
                             "it is {0} in ArealRadius.dat.".format(W['ArealRadius.dat'].shape[0]))
        ArealRadius = W['ArealRadius.dat'][:,1]
        AverageLapse = W['AverageLapse.dat'][:,1]
        CoordRadius = W['CoordRadius.dat'][0,1]
        InitialAdmEnergy = W['InitialAdmEnergy.dat'][0,1]
        LM = [[int(m.group('L')), int(m.group('M'))] for DataSet in list(W) for m in [YlmRegex.search(DataSet)] if m]
        NModes = len(LM)
        Mag = numpy.empty((NModes, NTimes))
        Arg = numpy.empty((NModes, NTimes))
        m = 0
        for DataSet in list(W) :
            if(YlmRegex.search(DataSet)) :
                if( not (W[DataSet].shape[0]==NTimes) ) :
                    raise ValueError("The number of time steps in this dataset should be {0}; ".format(NTimes) +
                                     "it is {0} in {1}.".format(W[DataSet].shape[0], DataSet))
                Mag[m,:] = W[DataSet][:,1]
                Arg[m,:] = W[DataSet][:,2]
                m += 1
                #print("n={0}; m={1}; DataSet={2}".format(n, m, DataSet))
        TempW.AppendHistory("### # Python read from {}.".format(WaveformNames[n]))
        TempW.SetT(T)
        TempW.SetLM(PyGW.MatrixInt(LM))
        TempW.SetMag(PyGW.MatrixDouble(Mag))
        TempW.SetArg(PyGW.MatrixDouble(Arg))
        TempW.SetArealRadius(ArealRadius)
        TempW.RescaleMagForRadius(CoordRadius*ChMass)
        TempW.SetTimeFromAverageLapse(AverageLapse, InitialAdmEnergy)
        TempW.TortoiseRetard(InitialAdmEnergy)
        if(ChMass != 1.0) : TempW.SetTotalMassToOne(ChMass)
        TempW.MakeTimeMonotonic()
        TempW.ConvertReImToMagArg()
        for i,type in enumerate(TempW.Types) :
            if(File.find(type)>-1) :
                TempW.SetTypeIndex(i)
                break
        Ws[n] = TempW
    f.close()
    Ws.AppendHistory("### PyGW.ReadFiniteRadiusData(ChMass={0}, Dir='{1}', File='{2}')".format(ChMass, Dir, File))
    return Ws,InitialAdmEnergy

  %}
