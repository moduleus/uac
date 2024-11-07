classdef SuperGroup < uac.IGroup
  properties (Access = public, SetObservable, GetObservable)
    initialGroup uac.IGroup {mustBeScalarOrEmpty, urx.Validator.weakPtrInCpp}
    description char
  end

  methods
    function this = SuperGroup(varargin)
      this@uac.IGroup(varargin{:});
    end
  end
end
