function res = loadFromFile(filename)
  dataset_ptr = uac.LibBinding.getInstance().call('uac_load_from_file', filename);
  res = uac.Dataset(dataset_ptr, urx.PtrType.SHARED, []);
end
