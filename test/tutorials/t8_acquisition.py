import datetime

import numpy as np
import ultrasound_acquisition_configuration as uac
from helpers import makeLinearArrayUac, makePlaneWavesGroupUac


def run():
    # Transmit parameters
    n_theta = 4  # number of plane waves in the sequence
    d_theta = 4  # angular step between plane waves [°]
    theta_max = (n_theta - 1) / 2 * d_theta  # maximum plane wave angle [°]
    thetas = np.arange(-theta_max, theta_max + d_theta, d_theta)  # list of the angles [°]

    transmit_frequency = 8.33e6  # transmit frequency of the pulse [Hz]
    voltage = 25  # voltage of the excitation Vpeak [V]
    n_periods = 2  # number of transmitted periods
    n_frames = 200  # number of frames
    frame_rate = 500  # frame rate after compound [Hz]
    prf = 10e3  # pulse repetition rate [Hz]

    sound_speed = 1540  # celerity of the sound in the medium [m/s]

    # Receive parameters
    sampling_frequency = 33.3e6  # sampling frequency of the backscattered signal [Hz]
    modulation_frequency = 8.33e6  # modulation frequency of the backscattered signal [Hz]
    n_samples = 800  # number of received samples for each transmit event
    rx_time_offset = 1.2e-6  # time before the sampling of the received signal [s]

    # Probe parameters
    probe_description = "SL10-2 192 elements"  # description or name of the probe
    n_elements = 192  # number of transducers in the ultrasound probe
    pitch = 300e-6  # pitch (spatial step) of the array [m]
    width = 280e-6  # width of a single transducer [m]
    height = 6e-3  # height of a single transducer [m]
    # kerf = pitch - width  # space between adjacent transducer elements [m]
    channel_mapping = [
        [i] for i in range(n_elements)
    ]  # order of channels in the interface (start at 0)

    # Probe
    probe = makeLinearArrayUac(
        n_elements, pitch, width, height, probe_description
    )  # build the probe

    # Group
    [group, excitation] = makePlaneWavesGroupUac(
        thetas,
        voltage,
        n_periods,
        prf,
        n_frames,
        frame_rate,
        transmit_frequency,
        sound_speed,
        n_samples,
        rx_time_offset,
        sampling_frequency,
        modulation_frequency,
        channel_mapping,
        probe,
    )  # build the sequence of events

    # Acquisition
    acquisition = uac.Acquisition()
    acquisition.authors = "yourName"
    acquisition.description = "Example of plane waves acquisition for a linear array"
    acquisition.local_time = str(
        datetime.datetime.strptime("202501010000000000", "%Y%m%d%H%M%S%f").strftime(
            "%Y-%m-%d %H:%M:%S.%f"
        )
    )
    # local time of the acquisition
    acquisition.country_code = "FR"  # country code of the acquisition
    acquisition.system = "yourHardware"  # acquisition system description
    acquisition.probes = [probe]  # stores probe here for reuse (shared pointer)
    acquisition.excitations = [excitation]  # stores excitations here for reuse (shared pointer)
    acquisition.groups = [group]  # stores groups
    acquisition.initial_group = group

    # Dataset
    dataset = uac.Dataset()
    dataset.acquisition = acquisition
    uac.saveToFile(
        "./t8-python.uac", dataset, uac.WriterOptions(False, False, False)
    )  # saves the acquisition in a dataset.uac


if __name__ == "__main__":
    run()
