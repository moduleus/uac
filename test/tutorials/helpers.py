import numpy as np
import numpy.typing as npt
import ultrasound_acquisition_configuration as uac


def makeLinearArrayUac(
    n_elements: int, pitch: float, width: float, height: float, description: str
) -> uac.Probe:
    """
    makeLinearArrayUac.py creates a linear array probe in uac format
    Inputs:
        n_elements       : number of elements (transducers) in the probe
        pitch           : distance between the center of adjacent elements [m]
        width           : size of the element along the x-axis [m]
        height          : size of the element along the y-axis [m]
        description     : name or description of the probe
    Outputs:
        probe (uac.Probe) : built as a linear array along the x-axis
    Drawing:
        Linear array (n_elements = 12)
                                   ___ pitch     __ width
           o--->x    __ __ __ __ __ __ __ __ __ __ __ __
          /|        / // // // //+//+// // // // // // /    /
         / |       /_//_//_//_//_//_//_//_//_//_//_//_/    / height
        y   z

          +--+       perimeter of and element with + as elements' edges
         /  /       defined either clockwise or counterclockwise
        +--+
    Requirements:
        uac 1.2.0 toolbox
     More information:
         see : https://github.com/moduleus/uac/wiki
    """

    # Element's geometry : all the elements share the same impulse response
    edges = np.array(
        [  # x, y z coordinates (dimension 1) of the 4 edges (dimensions 2)
            [-width / 2, -width / 2, width / 2, width / 2],
            [-height / 2, height / 2, height / 2, -height / 2],
            [0, 0, 0, 0],
        ]
    )

    element_geometry = uac.ElementGeometry()
    element_geometry.perimeter = [  # perimeter of one element
        uac.Vector3D(edges[0, 0], edges[1, 0], edges[2, 0]),
        uac.Vector3D(edges[0, 1], edges[1, 1], edges[2, 1]),
        uac.Vector3D(edges[0, 2], edges[1, 2], edges[2, 2]),
        uac.Vector3D(edges[0, 3], edges[1, 3], edges[2, 3]),
    ]

    # (Optional) Impulse response : all the elements share the same impulse response
    impulse_response = uac.ImpulseResponse()
    impulse_response.sampling_frequency = (
        50e6  # sampling frequency of the impulse response data [Hz]
    )
    impulse_response.time_offset = 0  # delay before impulse reponse starts [s]
    impulse_response.units = "N/A"  # unit of the impulse response
    impulse_response.data = [  # impulse response depending on time
        0.0000,
        0.0467,
        0.1641,
        0.2780,
        0.2521,
        0.0000,
        -0.4160,
        -0.7869,
        -0.8756,
        -0.5759,
        -0.0000,
        0.5759,
        0.8756,
        0.7869,
        0.4160,
        0.0000,
        -0.2521,
        -0.2780,
    ]

    # Build elements' array
    elements = uac.VecElement()
    xmin = -pitch * (n_elements - 1) / 2  # first element center position
    for i in range(0, n_elements):  # loop on the elements
        x = xmin + i * pitch  # computes all the position element
        Element = uac.Element()
        Element.transform.translation = uac.Vector3D(x, 0, 0)  # elements are only along x-axis
        Element.element_geometry = element_geometry  # shared geometry between elements
        Element.impulse_response = impulse_response  # shared impulse response between elements
        elements.append(Element)  # adds elements to the array

    # Build the probe
    probe = uac.Probe()
    probe.description = description
    probe.type = (
        uac.ProbeType.LINEAR
    )  # 0: LINEAR, 1: CURVILINEAR, 2: MATRIX, 3: RCA, 4: SPARSE, -1: UNDEFINED
    probe.impulse_responses = [impulse_response]  # could store different impulse responses
    probe.element_geometries = [element_geometry]  # could store different element's geometry
    probe.elements = elements  # set all the elements once to avoid repetitive call to probe

    return probe


def makePlaneWaveTransmitSetupUac(
    theta: float,
    voltage: float,
    n_periods: int,
    transmit_frequency: float,
    max_delay: float,
    sound_speed: float,
    channel_mapping: uac.VecVecUInt32 | list[list[int]] | npt.NDArray[np.uint32],
    probe: uac.Probe,
    excitation: uac.Excitation | None = None,
):
    """
    makePlaneWaveTransmitSetupUac creates a plane waves transmit setup with a linear array in uac format
    Inputs:
        theta                       : plane wave angle of the event [°]
        voltage                     : voltage of the excitation [Vpeak]
        n_periods                    : number of periods of transmitted signal
        transmit_frequency           : central frequency of the transmitted signal [Hz]
        max_delay                    : maximum delay of the plane wave sequence [s]
        sound_speed                  : celerity of the sound in the medium [m/s]
        channel_mapping              : order of channels in the interface (start at 0)
        probe (uac.Probe)           : ultrasound probe used for the transmit event
        excitation (uac.Excitation) : (optional) excitation reused from another transmit event
    Outputs:
        TransmitSetup (uac.TransmitSetup)   : built for plane wave transmit event with a linear array probe
        excitation (uac.Excitation)         : excitation to be used in another transmit event
    Requirements:
        uac 1.2.0 toolbox
    More information:
        see : https://github.com/moduleus/uac/wiki
    """

    # Checks
    if probe.type != uac.ProbeType.LINEAR:
        raise ValueError("Wrong type of probe")

    # Excitation parameters
    excitation_post_clamp_duration = 900e-9
    # clamp after excitation to avoid reflexion on the electronics [s]
    excitation_sampling_frequency = 200e6
    # sampling frequency of the excitation waveform [Hz]

    # Delay
    n_elements = len(probe.elements)  # number of probe's elements
    pitch = (
        probe.elements[1].transform.translation.x - probe.elements[0].transform.translation.x
    )  # pitch of the probe
    delays = (
        np.sin(np.deg2rad(theta))
        * (np.arange(-(n_elements - 1) / 2, (n_elements - 1) / 2 + 1))
        * pitch
        / sound_speed
    )  # linear delay along the element to make the plane wave

    # Wave
    wave = uac.Wave()
    wave.type = (
        uac.WaveType.PLANE_WAVE
    )  # 0: CONVERGING_WAVE, 1: DIVERGING_WAVE, 2:PLANE_WAVE, 3:CYLINDRICAL_WAVE, -1: UNDEFINED
    wave.parameters = [
        np.sin(np.deg2rad(theta)),
        0.0,
        np.cos(np.deg2rad(theta)),
    ]  # defines the plane wave with the normal vector
    wave.time_zero = (
        max_delay + n_periods / transmit_frequency / 2
    )  # default value, reference point for the time_zero

    # Excitation
    if excitation is None:  # if no excitation input to be reused, definition of the excitation
        excitation = uac.Excitation()
        excitation.pulse_shape = "square wave"  # description of the pulse shape
        excitation.transmit_frequency = transmit_frequency
        excitation.sampling_frequency = excitation_sampling_frequency
        n_samples_excitation = round(
            excitation_sampling_frequency / transmit_frequency / 2
        )  # number of samples for a half period of excitation
        if n_samples_excitation != int(
            n_samples_excitation
        ):  # excitation_sampling_frequency may not allow transmit_frequency so it is corrected
            print(
                f"(Warning) Real transmit frequency: {excitation_sampling_frequency / n_samples_excitation / 2} Hz instead of {transmit_frequency}Hz"
            )  # warn if frequency has been changed
        n_post_clamp = round(excitation_sampling_frequency * excitation_post_clamp_duration)
        waveform = voltage * np.concatenate(
            (
                np.tile(
                    np.concatenate((np.ones(n_samples_excitation), -np.ones(n_samples_excitation))),
                    n_periods,
                ),
                np.zeros(n_post_clamp),
            )
        )

        excitation.waveform = uac.VecFloat64(waveform.ravel())  # square excitation waveform

    # TransmitSetup
    transmit_setup = uac.TransmitSetup()
    transmit_setup.wave = wave
    # active elements (arrays dimension) per excitations (cells dimension)
    transmit_setup.active_elements = channel_mapping
    transmit_setup.excitations = [excitation] * n_elements  # shared excitation for all the elements
    transmit_setup.delays = uac.VecFloat64(delays.ravel()) + max_delay
    transmit_setup.probe = probe

    return transmit_setup, excitation


def makeReceiveSetupUac(
    n_samples: int,
    rx_time_offset: float,
    sampling_frequency: float,
    modulation_frequency: float,
    channel_mapping: uac.VecVecUInt32 | list[list[int]] | npt.NDArray[np.uint32],
    probe: uac.Probe,
):
    """
    makeReceiveSetupUac.py creates a receive setup in uac format
    Inputs:
        n_samples                : number of samples per channel per reception event
        rx_time_offset            : time before the sampling of the received signal [s]
        sampling_frequency       : sampling frequency of the backscattered signal [Hz]
        modulation_frequency     : frequency used for the demodulation of received signal in IQ [Hz]
        channel_mapping          : order of channels in the interface (start at 0)
        probe (uac.Probe)       : ultrasound probe used for the reception event
    Outputs:
        ReceiveSetup (uac.ReceiveSetup) : built for reception event with all the elements of the probe
    Requirements:
        uac 1.2.0 toolbox
    More information:
        see : https://github.com/moduleus/uac/wiki
    """

    # Probe information
    # n_elements = len(probe.elements)

    # ReceiveSetup
    receive_setup = uac.ReceiveSetup()
    receive_setup.sampling_frequency = sampling_frequency
    receive_setup.modulation_frequency = modulation_frequency
    receive_setup.number_samples = n_samples
    receive_setup.active_elements = channel_mapping
    receive_setup.tgc_profile = []
    receive_setup.tgc_sampling_frequency = 0
    receive_setup.time_offset = rx_time_offset
    receive_setup.probe = probe

    return receive_setup


def makePlaneWavesGroupUac(
    thetas: list[float] | npt.NDArray[np.float32],
    voltage: float,
    n_periods: int,
    prf: float,
    n_frames: int,
    frame_rate: float,
    transmit_frequency: float,
    sound_speed: float,
    n_samples: int,
    rx_time_offset: float,
    sampling_frequency: float,
    modulation_frequency: float,
    channel_mapping: uac.VecVecUInt32 | list[list[int]] | npt.NDArray[np.uint32],
    probe: uac.Probe,
):
    """
    makePlaneWavesSequenceUac.py creates a plane waves sequence with a linear array in uac format
    Inputs:
       thetas                      : plane wave angles of the sequence [°]
       voltage                     : voltage of the excitation [Vpeak]
       nPeriods                    : number of periods of transmitted signal
       prf                         : pulse repetition frequency [Hz]
       nFrames                     : number of repetition of the sequence
       frameRate                   : repetition frequency of the sequence [Hz]
       transmitFrequency           : central frequency of the transmitted signal [Hz]
       soundSpeed                  : celerity of the sound in the medium [m/s]
       nSamples                    : number of samples per channel per reception event
       rxTimeOffset                : time before the sampling of the received signal [s]
       samplingFrequency           : sampling frequency of the backscattered signal [Hz]
       modulationFrequency         : frequency used for the demodulation of received signal in IQ [Hz]
       channelMapping              : order of channels in the interface (start at 0)
       Probe (uac.Probe)           : ultrasound probe used for the transmit/reception event
    Outputs:
        Group (uac.Group)           : built for plane wave transmit/reception event with a linear array probe
        Excitation (uac.Excitation) : excitation to be used in another transmit event
    Requirements:
        urx 1.2.0 toolbox
        uac 1.2.0 toolbox
    More information:
        see : https://github.com/moduleus/urx/wiki
    """

    # Maximum negative delay of the sequence
    n_elements = len(probe.elements)  # number of probe's elements
    pitch = (
        probe.elements[1].transform.translation.x - probe.elements[0].transform.translation.x
    )  # pitch of the probe [m]
    theta_max: float = max(abs(thetas))  # maximum angle [°]
    max_delays = -min(
        np.sin(np.deg2rad(theta_max))
        * (np.arange(-(n_elements - 1) / 2, (n_elements - 1) / 2 + 1))
        * pitch
        / sound_speed
    )  # maximum negative delay of the plane wave sequence [s]

    # Build the sequence of events
    sequence = uac.VecEvent()
    event = uac.Event()  # defines the first event and excitation of the sequence ...
    transmit_setup, excitation = makePlaneWaveTransmitSetupUac(
        thetas[0],
        voltage,
        n_periods,
        transmit_frequency,
        max_delays,
        sound_speed,
        channel_mapping,
        probe,
    )
    receive_setup = makeReceiveSetupUac(
        n_samples, rx_time_offset, sampling_frequency, modulation_frequency, channel_mapping, probe
    )
    event.transmit_setup = transmit_setup
    event.receive_setup = receive_setup
    sequence.append(event)
    for i in range(
        1, len(thetas)
    ):  # ... then increment the events of this sequence with the loop and reuse the excitation
        event = uac.Event()
        transmit_setup, excitation = makePlaneWaveTransmitSetupUac(
            thetas[i],
            voltage,
            n_periods,
            transmit_frequency,
            max_delays,
            sound_speed,
            channel_mapping,
            probe,
            excitation,
        )
        receive_setup = makeReceiveSetupUac(
            n_samples,
            rx_time_offset,
            sampling_frequency,
            modulation_frequency,
            channel_mapping,
            probe,
        )
        event.transmit_setup = transmit_setup
        event.receive_setup = receive_setup
        event.time_offset = i / prf
        sequence.append(event)

    # Group
    group = uac.Group()  # defines the group of the sequence
    group.description = "Group of plane waves"  # group name or description
    group.sound_speed = sound_speed
    group.sequence = (
        sequence  # set all the events of the sequence once to avoid repetitive call to Group
    )
    group.time_offset = (
        0  # test 0, normally must be bigger than the  Rx Analog reconf (ex :  20e-6)
    )
    group.repetition_count = n_frames
    group.period = 1 / frame_rate
    group.data_type = uac.DataType.INT16  # 0: INT16, 1: INT32, 2:FLOAT ,3:DOUBLE ,-1: UNDEFINED
    group.sampling_type = uac.SamplingType.RF  # 0: RF, 1: IQ, -1: UNDEFINED

    return group, excitation
