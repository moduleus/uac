classdef Wave < uac.Object
  properties (Access = ?urx.Object)
    parametersStd
  end
  properties (Access = public, SetObservable, GetObservable)

    type(1,1) int32

    timeZero(1,1) double

    timeZeroReferencePoint uac.Vector3D {urx.Validator.mustBeScalarOrEmpty, urx.Validator.rawInCpp}

    parameters(1,:) double
  end

  methods
    function this = Wave(varargin)
      this@uac.Object(varargin{:});
    end
  end
end
