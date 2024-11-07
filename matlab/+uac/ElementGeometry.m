classdef ElementGeometry < uac.Object
  properties (Access = ?urx.Object)
    perimeterStd
  end
  properties (Access = public, SetObservable, GetObservable)
    perimeter(1,:) uac.Vector3D {urx.Validator.rawInCpp}
  end

  methods
    function this = ElementGeometry(varargin)
      this@uac.Object(varargin{:});
    end
  end
end
