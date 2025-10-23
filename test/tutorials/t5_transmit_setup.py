import numpy as np
from helpers import makeLinearArrayUac, makePlaneWaveTransmitSetupUac


def run():
    # Transmit parameters
    theta = 4.0  # angle of the plane wave [°]
    theta_max = 6.0  # maximum plane wave angle of the sequence [°]
    transmit_frequency = 8.33e6  # transmit frequency of the pulse [Hz]
    voltage = 25.0  # voltage of the excitation Vpeak [V]
    n_periods = 2  # number of transmitted periods
    sound_speed = 1540.0  # celerity of the sound in the medium [m/s]

    # probe parameters
    probe_description = "SL10-2 192 elements"  # description or name of the probe
    n_elements = 192  # number of transducers in the ultrasound probe
    pitch = 300e-6  # pitch (spatial step) of the array [m]
    width = 280e-6  # width of a single transducer [m]
    height = 6e-3  # height of a single transducer [m]
    # kerf = pitch - width  # space between adjacent transducer elements [m]
    channel_mapping = [
        [i] for i in range(n_elements)
    ]  # order of channels in the interface (start at 0)

    # probe
    probe = makeLinearArrayUac(
        n_elements, pitch, width, height, probe_description
    )  # build the probe

    # Transmit
    max_delays = -min(
        np.sin(np.deg2rad(theta_max))
        * (np.arange(-(n_elements - 1) / 2, (n_elements - 1) / 2 + 1))
        * pitch
        / sound_speed
    )
    # [transmit_setup, excitation] =
    makePlaneWaveTransmitSetupUac(
        theta,
        voltage,
        n_periods,
        transmit_frequency,
        max_delays,
        sound_speed,
        channel_mapping,
        probe,
    )


if __name__ == "__main__":
    run()
