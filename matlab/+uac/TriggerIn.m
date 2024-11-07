classdef TriggerIn < uac.Object
  properties (Access = public, SetObservable, GetObservable)
    channel char
    edge(1,1) int32
  end

  methods
    function this = TriggerIn(varargin)
      this@uac.Object(varargin{:});
    end
  end
end
