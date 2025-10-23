function ReceiveSetup = makeReceiveSetupUac(nSamples, rxTimeOffset, samplingFrequency, modulationFrequency, channelMapping, Probe)
    %makeReceiveSetupUac.m Creates a receive setup in uac format
    % Inputs:
    %   nSamples                : number of samples per channel per reception event 
    %   rxTimeOffset            : time before the sampling of the received signal [s]
    %   samplingFrequency       : sampling frequency of the backscattered signal [Hz]
    %   modulationFrequency     : frequency used for the demodulation of received signal in IQ [Hz]   
    %   channelMapping          : order of channels in the interface (start at 0)
    %   Probe (uac.Probe)       : ultrasound probe used for the reception event
    %
    % Outputs:
    %   ReceiveSetup (uac.ReceiveSetup) : built for reception event with all the elements of the probe
    %
    % For more information see : https://github.com/moduleus/uac/wiki
    % 
    % Requirements:     uac 1.2.0 toolbox
    
    %% ReceiveSetup
    ReceiveSetup = uac.ReceiveSetup();
    ReceiveSetup.samplingFrequency = samplingFrequency;
    ReceiveSetup.modulationFrequency = modulationFrequency;
    ReceiveSetup.numberSamples = nSamples;
    ReceiveSetup.activeElements = num2cell(channelMapping);      % active elements (arrays dimension) per excitations (cells dimension)  
    ReceiveSetup.probe = Probe;
    % ReceiveSetup.probeTransform.translation = uac.Vector3D();  % default value, position of the probe for the reception event
    % ReceiveSetup.probeTransform.rotation = uac.Vector3D();     % default value, rotation of the probe for the reception event
    ReceiveSetup.tgcProfile = [];                                % Time Gain Compensation profile 
    ReceiveSetup.tgcSamplingFrequency = 0;                       % sampling of the TGC profile [Hz]
    ReceiveSetup.timeOffset = rxTimeOffset;                      % delay before sampling [s]
    % ReceiveSetup.hwConfig                                      % configuration of the hardware
end
