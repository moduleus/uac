classdef Dataset < uac.Object
  properties (Access = public, SetObservable, GetObservable)
    version uac.Version {mustBeScalarOrEmpty, urx.Validator.rawInCpp}
    acquisition uac.Acquisition {mustBeScalarOrEmpty, urx.Validator.rawInCpp}
  end

  methods
    function this = Dataset(varargin)
      this@uac.Object(varargin{:});
    end

    function saveToFile(this, filename)
      arguments
        this
        filename char
      end

      this.libBindingRef.call('uac_save_to_file', filename, this.id);
    end

    function res = toUrx(this)
      dataset_ptr = this.libBindingRef.call('uac_to_urx', this.id);
      res = urx.Dataset(dataset_ptr, urx.PtrType.SHARED, []);
    end
  end

  methods(Static)
    function this = loadFromFile(filename)
      arguments
        filename char
      end

      dataset_ptr = uac.LibBinding.getInstance().call('uac_load_from_file', filename);
      this = uac.Dataset(dataset_ptr, urx.PtrType.SHARED, []);
    end
  end
end
