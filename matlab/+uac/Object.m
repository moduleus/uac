classdef Object < urx.Object
  methods
    function this = Object(varargin)
      this@urx.Object(varargin{:});
    end

    function res = getInstance(this)
      res = uac.LibBinding.getInstance();
    end
  end
end
