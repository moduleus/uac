classdef ReceiveSetup < uac.Object
  properties (Access = ?urx.Object)
    activeElementsStd
    tgcProfileStd
  end
  properties (Access = public, SetObservable, GetObservable)
    probe uac.Probe {mustBeScalarOrEmpty, urx.Validator.weakPtrInCpp}

    probeTransform uac.Transform {mustBeScalarOrEmpty, urx.Validator.rawInCpp}

    samplingFrequency(1,1) double

    numberSamples(1,1) uint32

    activeElements(1,:) cell

    tgcProfile(1,:) double
    tgcSamplingFrequency(1,1) double

    modulationFrequency(1,1) double
    timeOffset(1,1) double
  end
  properties (Access = ?urx.Object)
    hwConfigPtr uac.HwConfig
  end
  properties (Access = public, SetObservable, GetObservable)
    hwConfig struct {urx.Validator.rawInCpp}
  end

  methods
    function this = ReceiveSetup(varargin)
      this@uac.Object(varargin{:});
      this.activeElementsStd = uac.StdVector('uint32', 2, urx.PtrType.RAW, this);
    end
  end
end
