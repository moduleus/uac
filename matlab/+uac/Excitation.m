classdef Excitation < uac.Object
  properties (Access = ?urx.Object)
    waveformStd
  end
  properties (Access = public, SetObservable, GetObservable)
    pulseShape char

    transmitFrequency(1,1) double
    samplingFrequency(1,1) double

    waveform(1,:) double
  end
  properties (Access = ?urx.Object)
    hwConfigPtr uac.HwConfig
  end
  properties (Access = public, SetObservable, GetObservable)
    hwConfig struct {urx.Validator.rawInCpp}
  end

  methods
    function this = Excitation(varargin)
      this@uac.Object(varargin{:});
    end
  end
end
