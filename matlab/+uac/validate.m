function res = validate(dataset)
  libBinding = uac.LibBinding.getInstance();
  res = libBinding.call('uac_validate_dataset', dataset);
end
