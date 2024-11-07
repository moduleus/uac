classdef Element < uac.Object
  properties (Access = public, SetObservable, GetObservable)
    transform uac.Transform {mustBeScalarOrEmpty, urx.Validator.rawInCpp}
    
    elementGeometry uac.ElementGeometry {mustBeScalarOrEmpty, urx.Validator.weakPtrInCpp}
    impulseResponse uac.ImpulseResponse {mustBeScalarOrEmpty, urx.Validator.weakPtrInCpp}
  end

  methods
    function this = Element(varargin)
      this@uac.Object(varargin{:});
    end
  end
end
