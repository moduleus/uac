classdef Version < uac.Object
  properties (Access = public, SetObservable, GetObservable)
    major (1,1) uint16
    minor (1,1) uint16
    patch (1,1) uint16
  end

  methods
    function this = Version(varargin)
      this@uac.Object(varargin{:});
    end
  end
end
