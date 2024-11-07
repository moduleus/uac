classdef TriggerOut < uac.Object
  properties (Access = public, SetObservable, GetObservable)
    channel char
    timeOffset double
    pulseDuration double
    polarity(1,1) int32
  end

  methods
    function this = TriggerOut(varargin)
      this@uac.Object(varargin{:});
    end
  end
end
