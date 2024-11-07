classdef Acquisition < uac.Object
  properties (Access = ?urx.Object)
    probesStd
    excitationsStd
    groupsStd
    groupsDataStd
    superGroupsStd
  end
  properties (Access = public, SetObservable, GetObservable)
    authors char
    description char
    localTime char
    countryCode char
    system char
    
    timestamp(1,1) double
    
    probes(1,:) uac.Probe {urx.Validator.sharedPtrInCpp}
    excitations(1,:) uac.Excitation {urx.Validator.sharedPtrInCpp}
    groups(1,:) uac.Group {urx.Validator.sharedPtrInCpp}
  end
  properties (Access = ?urx.Object)
    hwConfigPtr uac.HwConfig
  end
  properties (Access = public, SetObservable, GetObservable)
    superGroups(1,:) uac.SuperGroup {urx.Validator.sharedPtrInCpp}
    initialGroup uac.IGroup {mustBeScalarOrEmpty, urx.Validator.weakPtrInCpp}
    timeOffset(1,1) double
    triggerIn uac.TriggerIn {mustBeScalarOrEmpty, urx.Validator.optionalInCpp}
    triggerOut uac.TriggerOut {mustBeScalarOrEmpty, urx.Validator.optionalInCpp}
    hwConfig struct {urx.Validator.rawInCpp}
  end

  methods
    function this = Acquisition(varargin)
      this@uac.Object(varargin{:});
    end
  end
end
