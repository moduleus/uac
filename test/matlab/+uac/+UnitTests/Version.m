classdef Version < matlab.unittest.TestCase
  properties (MethodSetupParameter)
    libraryPath = {''}
    headerPath = {''}
    includepaths = {''}
    envpaths = {''}
    libraryPathUrx = {''}
    headerPathUrx = {''}
    includepathsUrx = {''}
    envpathsUrx = {''}
  end

  methods(TestMethodSetup)
    function initTests(~, libraryPath, headerPath, includepaths, envpaths, ...
        libraryPathUrx, headerPathUrx, includepathsUrx, envpathsUrx)
      feature('DefaultCharacterSet','UTF-8');

      urx.LibBinding.getInstance(libraryPathUrx, headerPathUrx, includepathsUrx, envpathsUrx);
      uac.LibBinding.getInstance(libraryPath, headerPath, includepaths, envpaths);
    end
  end

  methods(Test)
    function testVersion(testcase)
      libBinding = uac.LibBinding.getInstance();

      version = uac.Version();

      % Check that only needed properties are visible
      testcase.verifyEqual(numel(properties(version)), 3);

      % Modify object directly
      version.major = 852;
      version.minor = 111;
      version.patch = 963;

      % Check value. Should read data from C++.
      testcase.verifyEqual(version.major, uint16(852));
      testcase.verifyEqual(version.minor, uint16(111));
      testcase.verifyEqual(version.patch, uint16(963));

      % Modify data without using urx.Object.
      ptrMajor = libBinding.call('uac_Version_shared_major', version);
      ptrMajor.setdatatype('uint16Ptr', 1, 1);
      ptrMinor = libBinding.call('uac_Version_shared_minor', version);
      ptrMinor.setdatatype('uint16Ptr', 1, 1);
      ptrPatch = libBinding.call('uac_Version_shared_patch', version);
      ptrPatch.setdatatype('uint16Ptr', 1, 1);

      % Check that pointer is valid.
      testcase.verifyEqual(ptrMajor.Value, uint16(852));
      testcase.verifyEqual(ptrMinor.Value, uint16(111));
      testcase.verifyEqual(ptrPatch.Value, uint16(963));

      % Modify value by using the pointer.
      ptrMajor.Value = 777;
      ptrMinor.Value = 666;
      ptrPatch.Value = 555;

      % Check that version really read data from C++.
      testcase.verifyEqual(version.major, uint16(777));
      testcase.verifyEqual(version.minor, uint16(666));
      testcase.verifyEqual(version.patch, uint16(555));
    end
  end
end