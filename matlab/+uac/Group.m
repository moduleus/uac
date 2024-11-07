classdef Group < uac.IGroup
  properties (Access = ?urx.Object)
    sequenceStd
  end
  properties (Access = public, SetObservable, GetObservable)
    samplingType(1,1) int32
    dataType(1,1) int32

    description char

    soundSpeed(1,1) double

    sequence(1,:) uac.Event {urx.Validator.rawInCpp}
  end

  methods
    function this = Group(varargin)
      this@uac.IGroup(varargin{:});
    end
  end
end
