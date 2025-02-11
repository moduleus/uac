classdef DestinationLink < uac.Object
  properties (Access = public, SetObservable, GetObservable)
    trigger uac.TriggerIn {urx.Validator.mustBeScalarOrEmpty, urx.Validator.optionalInCpp}
    destination uac.IGroup {urx.Validator.mustBeScalarOrEmpty, urx.Validator.weakPtrInCpp}
  end

  methods
    function this = DestinationLink(varargin)
      this@uac.Object(varargin{:});
    end
  end
end
