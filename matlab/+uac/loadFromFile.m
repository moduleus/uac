function res = loadFromFile(filename)
  libBinding = uac.LibBinding.getInstance();
  dataset_ptr = libBinding.call('uac_load_from_file', filename);
  res = uac.Dataset(dataset_ptr, urx.PtrType.SHARED, []);
end
