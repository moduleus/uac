classdef ImpulseResponse < uac.Object
  properties (Access = ?urx.Object)
    dataStd
  end
  properties (Access = public, SetObservable, GetObservable)
    samplingFrequency(1,1) double
    timeOffset(1,1) double

    units char

    data(1,:) double
  end

  methods
    function this = ImpulseResponse(varargin)
      this@uac.Object(varargin{:});
    end
  end
end
