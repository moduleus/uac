function res = toUrx(dataset)
  dataset_ptr = uac.LibBinding.getInstance().call('uac_to_urx', dataset.getId());
  res = urx.Dataset(dataset_ptr, urx.PtrType.SHARED, []);
end
