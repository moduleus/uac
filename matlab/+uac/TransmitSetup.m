classdef TransmitSetup < uac.Object
  properties (Access = public)
    activeElementsStd
    excitationsStd
    delaysStd
  end
  properties (Access = public, SetObservable, GetObservable)
    probe uac.Probe {mustBeScalarOrEmpty, urx.Validator.weakPtrInCpp}
    wave uac.Wave {mustBeScalarOrEmpty, urx.Validator.rawInCpp}

    activeElements(1,:) cell

    excitations(1,:) uac.Excitation {urx.Validator.weakPtrInCpp}

    delays(1,:) double

    probeTransform uac.Transform {mustBeScalarOrEmpty, urx.Validator.rawInCpp}

    timeOffset(1,1) double
  end
  properties (Access = ?urx.Object)
    hwConfigPtr uac.HwConfig
  end
  properties (Access = public, SetObservable, GetObservable)
    hwConfig struct {urx.Validator.rawInCpp}
  end

  methods
    function this = TransmitSetup(varargin)
      this@uac.Object(varargin{:});
      this.activeElementsStd = uac.StdVector('uint32', 2, urx.PtrType.RAW, this);
    end
  end
end