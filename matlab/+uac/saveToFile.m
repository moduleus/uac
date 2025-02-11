function saveToFile(filename, dataset)
  uac.LibBinding.getInstance().call('uac_save_to_file', filename, dataset.getId());
end
