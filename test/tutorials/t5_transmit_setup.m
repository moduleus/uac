%% Transmit parameters 
Theta = 4;                                  % angle of the plane wave [°]
ThetaMax = 6;                               % maximum plane wave angle of the sequence [°]
TransmitFrequency = 8.33e6;                 % transmit frequency of the pulse [Hz]
Voltage = 25;                               % voltage of the excitation Vpeak [V]
NPeriods = 2;                               % number of transmitted periods
SoundSpeed = 1540;                          % celerity of the sound in the medium [m/s]

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

%% Transmit
MaxDelay = -min(sind(ThetaMax) * (-(NElements-1)/2:(NElements-1)/2) * Pitch / SoundSpeed);
[TransmitSetup, Excitation] = makePlaneWaveTransmitSetupUac(Theta, Voltage, NPeriods, TransmitFrequency, MaxDelay, SoundSpeed, ChannelMapping, Probe);

disp(TransmitSetup)
disp(Excitation)
