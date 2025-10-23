function res = loadFromFile(filename, raw_data_load_policy)
  libBinding = uac.LibBinding.getInstance();
  if nargin == 1
    dataset_ptr = libBinding.call('uac_load_from_file', filename);
  else
    dataset_ptr = libBinding.call('uac_load_from_file_options', filename, raw_data_load_policy);
  end
  res = uac.Dataset(urx.PtrType.SHARED, dataset_ptr, []);
end
