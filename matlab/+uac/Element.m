classdef Element < uac.Object
  properties (Access = public, SetObservable, GetObservable)
    transform uac.Transform {urx.Validator.mustBeScalarOrEmpty, urx.Validator.rawInCpp}
    
    elementGeometry uac.ElementGeometry {urx.Validator.mustBeScalarOrEmpty, urx.Validator.weakPtrInCpp}
    impulseResponse uac.ImpulseResponse {urx.Validator.mustBeScalarOrEmpty, urx.Validator.weakPtrInCpp}
  end

  methods
    function this = Element(varargin)
      this@uac.Object(varargin{:});
    end
  end
end
