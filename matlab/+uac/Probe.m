classdef Probe < uac.Object
  properties (Access = ?urx.Object)
    elementGeometriesStd
    impulseResponsesStd
    elementsStd
  end
  properties (Access = public, SetObservable, GetObservable)
    description char

    type(1,1) int32

    transform uac.Transform {urx.Validator.mustBeScalarOrEmpty, urx.Validator.rawInCpp}

    elementGeometries(1,:) uac.ElementGeometry {urx.Validator.sharedPtrInCpp}
    impulseResponses(1,:) uac.ImpulseResponse {urx.Validator.sharedPtrInCpp}

    elements(1,:) uac.Element {urx.Validator.rawInCpp}
  end

  methods
    function this = Probe(varargin)
      this@uac.Object(varargin{:});
    end
  end
end
