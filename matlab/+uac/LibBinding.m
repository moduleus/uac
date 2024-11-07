classdef LibBinding < urx.LibBinding
  methods (Static)
    function this = getInstance(libraryPath, headerPath, includepaths, envpaths)
      persistent instance
      if isempty(instance)
        setenv("HDF5_DISABLE_VERSION_CHECK", "1");
        if nargin < 4
          scriptPath = fileparts(mfilename('fullpath'));
          % Default path for toolbox
          if ispc()
            instance = urx.LibBinding([scriptPath '/../UacMatlabBinding.dll'], ...
              [scriptPath '/../include/uac/matlab/bindings.h'], ...
              {[scriptPath '/../include'], [scriptPath '/../../urx/include']}, {});
          else
            instance = urx.LibBinding([scriptPath '/../libUacMatlabBinding.so'], ...
              [scriptPath '/../include/uac/matlab/bindings.h'], ...
              {[scriptPath '/../include'], [scriptPath '/../../urx/include']}, {});
          end
        else
          instance = urx.LibBinding(libraryPath, headerPath, includepaths, envpaths);
        end
      end
      this = instance;
    end
  end
end
