classdef Event < uac.Object % handle
  properties (Access = public, SetObservable, GetObservable)
    transmitSetup uac.TransmitSetup {mustBeScalarOrEmpty, urx.Validator.rawInCpp}
    receiveSetup uac.ReceiveSetup {mustBeScalarOrEmpty, urx.Validator.rawInCpp}
  end
  properties (Access = ?urx.Object)
    hwConfigPtr uac.HwConfig
  end
  properties (Access = public, SetObservable, GetObservable)
    timeOffset double
    triggerIn uac.TriggerIn {mustBeScalarOrEmpty, urx.Validator.optionalInCpp}
    triggerOut uac.TriggerOut {mustBeScalarOrEmpty, urx.Validator.optionalInCpp}
    hwConfig struct {urx.Validator.rawInCpp}
  end

  methods
    function this = Event(varargin)
      this@uac.Object(varargin{:});
    end
  end
end
