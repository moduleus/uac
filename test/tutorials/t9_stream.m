
UacFilename = "./t8-matlab.uac";
UrxFilename = "./t9-matlab.urx";

% Generate urx without GroupData
UacDataset = uac.loadFromFile(UacFilename);
UrxDataset = uac.toUrx(UacDataset);

% The urx file must exist and be saved before using stream.
urx.saveToFile(UrxFilename, UrxDataset, false, false, false);

GroupIdx = 1;

SequenceSize = sum(arrayfun(@(event) length(event.receiveSetup.activeElements) * event.receiveSetup.numberSamples, UrxDataset.acquisition.groups(GroupIdx).sequence));

RawDataVectorShort = urx.RawData_int16_t_real(SequenceSize);
%for n = 1:SequenceSize
%    RawDataVectorShort.data(1, n) = n;
%end

Stream = urx.Stream(UrxFilename, UrxDataset);
GroupData = Stream.createGroupData(UrxDataset.acquisition.groups(GroupIdx), 2.0);

% It's not mandatory to write the full sequence at once.
GroupData.append(RawDataVectorShort, 9.0, [8.0]);

% Stream and GroupDataStream keep an HDF5 object. So destroy it to release lock on file.
clear GroupData
clear Stream

DatasetLoaded = urx.Dataset();
Stream = urx.Stream(UrxFilename, DatasetLoaded);
Stream.setReaderOptions(urx.RawDataLoadPolicy.STREAM);
Stream.loadFromFile();

GroupDataIdx = 1;

% Buffer must reserve memory.
Buffer = urx.RawData_int16_t_real(SequenceSize);

% It's not mandatory to write the full sequence at once.
Stream.readRawData(...
    GroupDataIdx,...
    Buffer,...
    1,...  % Index of the sequence in the buffer.
    1,...  % Index of the sequence in the urx file.
    1);  % Count of sequence to read.

assert(isequal(RawDataVectorShort, Buffer));