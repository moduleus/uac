function saveToFile(filename, dataset, chunk_group_data, clean_unusable_data, check_data)
  libBinding = uac.LibBinding.getInstance();
  if nargin == 2
    libBinding.call('uac_save_to_file', filename, dataset);
  else
    libBinding.call('uac_save_to_file_options', filename, dataset, chunk_group_data, clean_unusable_data, check_data);
  end
end
