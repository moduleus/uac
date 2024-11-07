classdef StdVector < urx.StdVector
  methods
    function this = StdVector(varargin)
      this@urx.StdVector(varargin{:});
    end

    function res = getInstance(this)
      res = uac.LibBinding.getInstance();
    end
  end
end
