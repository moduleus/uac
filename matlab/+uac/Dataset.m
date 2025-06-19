classdef Dataset < uac.Object
  properties (Access = public, SetObservable, GetObservable)
    version uac.Version {urx.Validator.mustBeScalarOrEmpty, urx.Validator.rawInCpp}
    acquisition uac.Acquisition {urx.Validator.mustBeScalarOrEmpty, urx.Validator.rawInCpp}
  end

  methods
    function this = Dataset(varargin)
      this@uac.Object(varargin{:});
    end

    function saveToFile(this, filename)
      warning('uac.Dataset.saveTofile is depreciated. Use static method uac.saveToFile(filename, dataset).');
      this.libBindingRef.call('uac_save_to_file', filename, this.id);
    end

    function res = toUrx(this)
      warning('uac.Dataset.toUrx is depreciated. Use static method uac.toUrx(dataset).');
      dataset_ptr = this.libBindingRef.call('uac_to_urx', this.id);
      res = urx.Dataset(dataset_ptr, urx.PtrType.SHARED, []);
    end
  end

  methods(Static)
    function this = loadFromFile(filename)
      warning('uac.Dataset.loadFromFile is depreciated. Use static method uac.loadFromFile(filename).');
      libBinding = uac.LibBinding.getInstance();
      dataset_ptr = libBinding.call('uac_load_from_file', filename);
      this = uac.Dataset(dataset_ptr, urx.PtrType.SHARED, []);
    end
  end
end
