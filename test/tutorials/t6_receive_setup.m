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

%% Receive
ReceiveSetup = makeReceiveSetupUac(NSamples, RxTimeOffset, SamplingFrequency, ModulationFrequency, ChannelMapping, Probe);        % build the receive setup

disp(ReceiveSetup)
