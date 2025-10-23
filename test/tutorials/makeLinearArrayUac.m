function Probe = makeLinearArrayUac(nElements, pitch, width, height, description)
    %makeLinearArrayUac.m Creates a linear array probe in uac format
    % Inputs:
    %   nElements       : number of elements (transducers) in the probe 
    %   pitch           : distance between the center of adjacent elements [m] 
    %   width           : size of the element along the x-axis [m] 
    %   height          : size of the element along the y-axis [m] 
    %   description     : name or description of the probe
    %
    % Outputs:
    %   Probe (uac.Probe) : built as a linear array along the x-axis
    %  
    % Drawing: Linear array (nElements = 12)
    %                                          __ width
    %       ____ x    __ __ __ __ __ __ __ __ __ __ __ __
    %      /|        / // // // // // // // // // // // /    /             
    %     / |       /_//_//_//_//_//_//_//_//_//_//_//_/    / height      
    %    y   z                 ___ pitch                                 
    % 
    %      +--+       perimeter of and element with + as elements edges
    %     /  /       defined either clockwise or counterclockwise 
    %    +--+     
    %
    % For more information see : https://github.com/moduleus/uac/wiki
    % 
    % Requirements:     urx 1.2.0 toolbox
    %                   uac 1.2.0 toolbox
    

    %% Element's geometry : all the elements share the same geometry
    edges =  [  -width/2,  -width/2,   width/2,   width/2; ...      % x, y z coordinates (dimension 1) of the 4 edges (dimensions 2) 
               -height/2,  height/2,  height/2, -height/2; ...                                                                     
                  0,    0,    0,    0];                                                                                             
    
    ElementGeometry = uac.ElementGeometry();
    ElementGeometry.perimeter = [ uac.Vector3D(edges(1,1), edges(2,1), edges(3,1)),...      % perimeter of one element
                     uac.Vector3D(edges(1,2), edges(2,2), edges(3,2)),...
                     uac.Vector3D(edges(1,3), edges(2,3), edges(3,3)),...   
                     uac.Vector3D(edges(1,4), edges(2,4), edges(3,4))];   

    %% (Optional) Impulse response : all the elements share the same impulse response
    ImpulseResponse = uac.ImpulseResponse();
    ImpulseResponse.samplingFrequency = 50e6;                                           % sampling frequency of the impulse response data [Hz]
    ImpulseResponse.timeOffset = 0;                                                     % delay before impulse reponse starts [s]
    ImpulseResponse.units = "N/A";                                                      % unit of the impulse response
    ImpulseResponse.data = [  0.0000,  0.0467,  0.1641,  0.2780,  0.2521,  0.0000,...   % impulse response depending on time
        -0.4160, -0.7869, -0.8756, -0.5759, -0.0000,  0.5759,...
         0.8756,  0.7869,  0.4160,  0.0000, -0.2521, -0.2780];
    
    %% Build the elements' array
    Elements = uac.Element();                                           % defines the first element ...
    xmin = -pitch * (nElements - 1) / 2;                                % first element center position
    Elements.transform.translation = uac.Vector3D(xmin, 0, 0);          % set in the transform translation 
    % Elements.transform.rotation = uac.Vector3D();                     % default value
    Elements.elementGeometry = ElementGeometry;                         % set element's Geometry
    Elements.impulseResponse = ImpulseResponse;                         % set element's impulse response (optional)

    
    % Loop on the other elements                                    % ... then increments this element with a loop
    for i = 2:nElements                                             
        x = xmin + (i - 1) * pitch;                                      % computes all the position element 
        Element = uac.Element();                                         
        Element.transform.translation = uac.Vector3D(x, 0, 0);           % elements are only along x-axis
        % Element.transform.rotation = uac.Vector3D();                   % default value, rotation of the element
        Element.elementGeometry = ElementGeometry;                       % shared geometry between elements
        Element.impulseResponse = ImpulseResponse;                       % shared impulse response between elements
        Elements = [Elements, Element];                                  % appends elements to the array
    end
    
    %% Build the probe
    Probe = uac.Probe();
    Probe.description = description;
    Probe.type = uac.Probe.ProbeType.LINEAR;                % 0: LINEAR, 1: CURVILINEAR, 2: RCA, 3: MATRIX, 4: SPARSE, -1: UNDEFINED 
    % Probe.transform.rotation = uac.Vector3D();            % default value, 
    % Probe.transform.translation = uac.Vector3D();         % default value
    Probe.impulseResponses = ImpulseResponse;               % could be an array in case of different impulse responses
    Probe.elementGeometries = ElementGeometry;              % could be an array in case of different elements geometries
    Probe.elements = Elements;                              % set all the elements once to avoid repetitive call to Probe
end
