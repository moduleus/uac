function [TransmitSetup, Excitation] = makePlaneWaveTransmitSetupUac(theta, voltage, nPeriods, transmitFrequency, maxDelay, soundSpeed, channelMapping, Probe, Excitation)
    %makePlaneWaveTransmitSetupUac.m Creates a plane waves transmit setup with a linear array in uac format
    % Intputs:
    %   theta                       : plane wave angle of the event [°]
    %   voltage                     : voltage of the excitation [Vpeak]
    %   nPeriods                    : number of periods of transmitted signal
    %   transmitFrequency           : central frequency of the transmitted signal [Hz]
    %   maxDelay                    : maximum delay of the plane wave sequence [s]
    %   soundSpeed                  : celerity of the sound in the medium [m/s]   
    %   channelMapping              : order of channels in the interface (start at 0)
    %   Probe (uac.Probe)           : ultrasound probe used for the transmit event
    %   Excitation (uac.Excitation) : (optional) excitation reused from another transmit event
    % 
    % Outputs:
    %   TransmitSetup (uac.TransmitSetup)   : built for plane wave transmit event with a linear array probe
    %   Excitation (uac.Excitation)         : excitation to be used in another transmit event
    %
    % For more information see : https://github.com/moduleus/uac/wiki
    % 
    % Requirements:     urx 1.2.0 toolbox
    %                   uac 1.2.0 toolbox
    
    %% Excitation parameters 
    excitationPostClampDuration = 900e-9;           % clamp after excitation to avoid reflexion on the electronics [s]
    excitationSamplingFrequency = 200e6;            % sampling frequency of the excitation waveform [Hz]

    %% Checks 
    if Probe.type ~= uac.Probe.ProbeType.LINEAR
        error('Wrong type of probe')
    end

    %% Plane wave delay
    nElements = length(Probe.elements);                                                             % number of probe's elements
    pitch = Probe.elements(2).transform.translation.x-Probe.elements(1).transform.translation.x;    % pitch of the probe
    delays = sind(theta) * (-(nElements-1)/2:(nElements-1)/2) * pitch / soundSpeed;                 % linear delay along the element to make the plane wave

    %% Wave
    Wave = uac.Wave();                          
    Wave.type = uac.Wave.WaveType.PLANE_WAVE;                       % 0: CONVERGING_WAVE, 1: DIVERGING_WAVE, 2:PLANE_WAVE, 3:CYLINDRICAL_WAVE, -1: UNDEFINED        
    Wave.parameters = [sind(theta), 0, cosd(theta)];                % defines the plane wave with the normal vector
    Wave.timeZero = maxDelay + nPeriods/transmitFrequency/2;        % delay to avoid negative time in the plane waves' delays                     
    % Wave.timeZeroReferencePoint = uac.Vector3D();                 % default value, reference point for the timeZero

    %% Excitation
    if ~exist('Excitation','var')                                                                                 % if no Excitation input to be reused, definition of the Excitation
        Excitation = uac.Excitation();                                  
        Excitation.pulseShape = "square wave";                                                                    % description of the pulse shape
        Excitation.transmitFrequency = transmitFrequency;                  
        Excitation.samplingFrequency = excitationSamplingFrequency;
        nExcitation = round(excitationSamplingFrequency/transmitFrequency/2);                                     % number of samples for a half period of excitation 
        if nExcitation ~= floor(nExcitation)                                                                      % excitationSamplingFrequency may not allow transmitFrequency so it is corrected
            warning(['Real transmit frequency:', num2str(excitationSamplingFrequency/nExcitation/2), 'Hz instead of ', num2str(transmitFrequency), 'Hz']);            % warn if frequency has been changed
        end
        nPostClamp = round(excitationSamplingFrequency*excitationPostClampDuration);
        Excitation.waveform = [voltage * repmat([ones(1,nExcitation), - ones(1,nExcitation)], [1,nPeriods]), zeros(1, nPostClamp)];       % square excitation waveform, times voltage ? 
    end

    %% TransmitSetup
    TransmitSetup = uac.TransmitSetup();
    TransmitSetup.wave = Wave;                                              
    TransmitSetup.activeElements = num2cell(channelMapping);                % active elements (arrays dimension) per excitations (cells dimension), starting at 0
    TransmitSetup.excitations = repmat(Excitation, [1, nElements]);         % shared excitation for all the elements
    TransmitSetup.delays = delays + maxDelay;                               % set minimum delay to 0 (avoid negative delays)
    TransmitSetup.probe = Probe;                                            
    % TransmitSetup.probeTransform.translation = uac.Vector3D();            % default value, position of the probe for this transmit event
    % TransmitSetup.probeTransform.rotation = uac.Vector3D();               % default value, rotation of the probe for this transmit event
    TransmitSetup.timeOffset = 0;                                     
    % TransmitSetup.hwConfig;                                               % not used here, store hardware configuration
end
