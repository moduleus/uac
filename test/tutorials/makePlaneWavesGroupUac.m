function [Group, Excitation] = makePlaneWavesGroupUac(thetas, voltage, nPeriods, prf, nFrames, frameRate, transmitFrequency, soundSpeed, nSamples, rxTimeOffset, samplingFrequency, modulationFrequency, channelMapping, Probe)
    %makePlaneWavesGroupUac.m Creates a plane waves sequence with a linear array in uac format
    % Intputs:
    %   thetas                      : plane wave angles of the sequence [°]
    %   voltage                     : voltage of the excitation [Vpeak]
    %   nPeriods                    : number of periods of transmitted signal
    %   prf                         : pulse repetition frequency [Hz]
    %   nFrames                     : number of repetition of the sequence  
    %   frameRate                   : repetition frequency of the sequence [Hz]
    %   transmitFrequency           : central frequency of the transmitted signal [Hz]
    %   soundSpeed                  : celerity of the sound in the medium [m/s]
    %   nSamples                    : number of samples per channel per reception event
    %   rxTimeOffset                : time before the sampling of the received signal [s]
    %   samplingFrequency           : sampling frequency of the backscattered signal [Hz]
    %   modulationFrequency         : frequency used for the demodulation of received signal in IQ [Hz]
    %   channelMapping              : order of channels in the interface (start at 0)
    %   Probe (uac.Probe)           : ultrasound probe used for the transmit/reception event
    %
    % Outputs:
    %   Group (uac.Group)           : built for plane wave transmit/reception event with a linear array probe
    %   Excitation (uac.Excitation) : excitation to be used in another transmit event
    %
    % For more information see : https://github.com/moduleus/urx/wiki
    %
    % Requirements: uac 1.2.0 toolbox
    
    %% Maximum negative delay of the sequence
    nElements = length(Probe.elements);                                                             % number of probe's elements
    pitch = Probe.elements(2).transform.translation.x-Probe.elements(1).transform.translation.x;    % pitch of the probe [m]
    thetaMax = max(abs(thetas));                                                                    % maximum angle [°]
    maxDelays = -min(sind(thetaMax) * (-(nElements-1)/2:(nElements-1)/2) * pitch / soundSpeed);     % maximum negative delay of the plane wave sequence [s]
    
    %% Build the sequence of events
    Sequence = uac.Event();                         % defines the first event of the sequence ...
    [TransmitSetup, Excitation] = makePlaneWaveTransmitSetupUac(thetas(1), voltage, nPeriods, transmitFrequency, maxDelays, soundSpeed, channelMapping, Probe);    % build its transmit setup
    ReceiveSetup = makeReceiveSetupUac(nSamples, rxTimeOffset, samplingFrequency, modulationFrequency, channelMapping, Probe);                                                                                % build its receive setup
    Sequence.transmitSetup = TransmitSetup;
    Sequence.receiveSetup = ReceiveSetup;
    Sequence.timeOffset = 0;                          % delay before the event starts [s]
    % Sequence.triggerIn;                             % not used yet, store acquisition trigger in
    % Sequence.triggerOut;                            % not used yet, store acquisition trigger out
    % Sequence.hwConfig;                              % not used yet, store hardware configuration of the event
    
    % Loop on the other events                      % ... then increments this sequence with a loop
    for i=2:length(thetas)
        Event=uac.Event();
        [TransmitSetup, Excitation] = makePlaneWaveTransmitSetupUac(thetas(i), voltage, nPeriods, transmitFrequency, maxDelays, soundSpeed, channelMapping, Probe, Excitation);
        ReceiveSetup = makeReceiveSetupUac(nSamples, rxTimeOffset, samplingFrequency, modulationFrequency, channelMapping, Probe);
        Event.transmitSetup = TransmitSetup;
        Event.receiveSetup = ReceiveSetup;
        Event.timeOffset = (i-1)/prf;
        % Event.triggerIn
        % Event.triggerOut
        % Event.hwConfig
        Sequence = [Sequence, Event];                   % append the event to the sequence
    end
    
    %% Group
    Group = uac.Group();                                % defines the group of the sequence
    Group.description = 'Group of plane waves';         % group name or description
    Group.soundSpeed = soundSpeed;
    Group.sequence = Sequence;                          % set all the events of the sequence once to avoid repetitive call to Group
    Group.timeOffset = 0;                               % test 0, normally must be bigger than the  Rx Analog reconf (ex :  20e-6)
        % Group.triggerIn      
        % Group.triggerOut                          
    Group.repetitionCount = nFrames;
        % Group.destinations  
    Group.period = 1/frameRate;
    Group.dataType = uac.Group.DataType.INT16;          % 0: INT16, 1: INT32, 2:FLOAT ,3:DOUBLE ,-1: UNDEFINED
    Group.samplingType = uac.Group.SamplingType.RF;     % 0: RF, 1: IQ, -1: UNDEFINED
end
