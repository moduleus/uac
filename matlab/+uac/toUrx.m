function res = toUrx(dataset)
  libBinding = uac.LibBinding.getInstance();
  dataset_ptr = libBinding.call('uac_to_urx', dataset);
  res = urx.Dataset(urx.PtrType.SHARED, dataset_ptr, []);
end
