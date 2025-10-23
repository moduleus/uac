function res = cloneDataset(dataset)
  libBinding = uac.LibBinding.getInstance();
  dataset_ptr = libBinding.call('uac_clone_dataset', dataset);
  res = uac.Dataset(urx.PtrType.SHARED, dataset_ptr, []);
end
