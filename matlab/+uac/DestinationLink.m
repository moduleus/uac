classdef DestinationLink < uac.Object
  properties (Access = public, SetObservable, GetObservable)
    trigger uac.TriggerIn {mustBeScalarOrEmpty, urx.Validator.optionalInCpp}
    destination uac.IGroup {mustBeScalarOrEmpty, urx.Validator.weakPtrInCpp}
  end

  methods
    function this = DestinationLink(varargin)
      this@uac.Object(varargin{:});
    end
  end
end
