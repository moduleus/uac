classdef IGroup < uac.Object
  properties (Access = ?urx.Object)
    destinationsStd
  end
  properties (Access = ?urx.Object)
    hwConfigPtr uac.HwConfig
  end
  properties (Access = public, SetObservable, GetObservable)
    timeOffset double
    triggerIn uac.TriggerIn {urx.Validator.mustBeScalarOrEmpty, urx.Validator.optionalInCpp}
    triggerOut uac.TriggerOut {urx.Validator.mustBeScalarOrEmpty, urx.Validator.optionalInCpp}
    repetitionCount uint32
    destinations(1,:) uac.DestinationLink {urx.Validator.rawInCpp}
    period double
    hwConfig struct {urx.Validator.rawInCpp}
  end

  methods
    function this = IGroup(varargin)
      this@uac.Object(varargin{:});
    end
  end
end
