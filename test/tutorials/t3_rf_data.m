function window = t3_rf_data()
  dataset = urx.loadFromFile('../data/single_int16_rf.urx');

  % groups_data identifier, if there are multiples in the same URX.
  gid = 1;

  % size(rf) = [Nsamples, Nchannels, Nevents, Nsequences]
  rf = getRfFromGroupsData(dataset.acquisition.groupsData(gid));
  window = figure;
  plot(rf(:,1,1,1))
end

function rf = getRfFromGroupsData(groupData)
  groupDataReader = urx.GroupDataReader(groupData);

  Nsamples = groupDataReader.samplesCount(1);

  Nchannels = groupDataReader.channelsCount(1);

  Nevents = groupDataReader.eventsCount();

  Nsequences = groupDataReader.sequencesCount();

  rf = reshape(groupData.rawData.data, Nsamples, Nchannels, Nevents, Nsequences);
end
