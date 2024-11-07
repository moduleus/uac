classdef Vector3D < uac.Object
  properties (Access = public, SetObservable, GetObservable)
    x(1,1)
    y(1,1)
    z(1,1)
  end

  methods
    function this = Vector3D(varargin)
      this@uac.Object(varargin{:});
    end
  end
end
