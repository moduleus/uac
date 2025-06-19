function saveToFile(filename, dataset)
  libBinding = uac.LibBinding.getInstance();
  libBinding.call('uac_save_to_file', filename, dataset);
end
