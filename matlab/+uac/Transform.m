classdef Transform < uac.Object
  properties (Access = public, SetObservable, GetObservable)
    rotation uac.Vector3D {urx.Validator.mustBeScalarOrEmpty, urx.Validator.rawInCpp}
    translation uac.Vector3D {urx.Validator.mustBeScalarOrEmpty, urx.Validator.rawInCpp}
  end

  methods
    function this = Transform(varargin)
      this@uac.Object(varargin{:});
    end
  end
end
