%% Transmit parameters 
NTheta = 4;                                 % number of plane waves in the sequence
DTheta = 4;                                 % angular step between plane waves [°]
ThetaMax = (NTheta-1)/2 * DTheta;           % maximum plane wave angle [°]
Thetas = -ThetaMax:DTheta:ThetaMax;         % list of the angles [°]

TransmitFrequency = 8.33e6;                 % transmit frequency of the pulse [Hz]
Voltage = 25;                               % voltage of the excitation Vpeak [V] 
NPeriods = 2;                               % number of transmitted periods
NFrames = 200;                              % number of frames 
FrameRate = 500;                            % frame rate after compound [Hz]
Prf = 10e3;                                 % pulse repetition rate [Hz]

SoundSpeed = 1540;                          % celerity of the sound in the medium [m/s]

%% Receive parameters 
SamplingFrequency = 33.3e6;                 % sampling frequency of the backscattered signal [Hz]
ModulationFrequency = 8.33e6;               % modulation frequency of the backscattered signal [Hz]
NSamples = 800;                             % number of received samples for each transmit event
RxTimeOffset = 1.2e-6;                      % time before the sampling of the received signal [s]

%% Probe parameters
ProbeDescription = 'SL10-2 192 elements';   % description or name of the probe
NElements = 192;                            % number of transducers in the ultrasound probe
Pitch = 300e-6;                             % pitch (spatial step) of the array [m]
Width = 280e-6;                             % width of a single transducer [m]
Height = 6e-3;                              % height of a single transducer [m]
Kerf = Pitch - Width;                       % space between adjacent transducer elements [m]
ChannelMapping = 0:NElements-1;             % order of channels in the interface (start at 0)

%% Probe
Probe = makeLinearArrayUac(NElements, Pitch, Width, Height, ProbeDescription);         % build the probe

%% Group
[Group, Excitation] = makePlaneWavesGroupUac(Thetas, Voltage, NPeriods, Prf, NFrames, FrameRate, TransmitFrequency, SoundSpeed, NSamples, RxTimeOffset, SamplingFrequency, ModulationFrequency, ChannelMapping, Probe);

%% Acquisition
Acquisition = uac.Acquisition();
Acquisition.authors = 'yourName';
Acquisition.description = 'Example of plane waves acquisition for a linear array';
Acquisition.localTime = char(datetime([2025 01 01]), 'yyyy-MM-dd HH:mm:ss.SSSSSS');     % local time of the acquisition
Acquisition.countryCode = 'FR';                                                         % country code of the acquisition
Acquisition.system = 'yourHardware';                                                    % acquisition system description
Acquisition.probes = Probe;                                                             % stores probe here for reuse (shared pointer)
Acquisition.excitations = Excitation;                                                   % stores excitations here for reuse (shared pointer)
Acquisition.groups = Group;                                                             % stores groups 
Acquisition.initialGroup = Group;                                                       % initial group during acquisition

%% Dataset
Dataset = uac.Dataset();
Dataset.acquisition = Acquisition;

%% Save
uac.saveToFile('./t8-matlab.uac',Dataset, false, false, false);      % saves the acquisition in a uac file
