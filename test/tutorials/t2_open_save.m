% read the input file
dataset = urx.loadFromFile('../data/rca.urx');

% print values
dataset.acquisition.description
dataset.acquisition.excitations(1).samplingFrequency

% change values
dataset.acquisition.description = "Learning to modify values ...";

% save the output file
urx.saveToFile('rca-matlab.urx', dataset, false, false, false)
