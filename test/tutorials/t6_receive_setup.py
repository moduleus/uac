from helpers import makeLinearArrayUac, makeReceiveSetupUac


def run():
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

    # Receive
    # receive_setup =
    makeReceiveSetupUac(
        n_samples, rx_time_offset, sampling_frequency, modulation_frequency, channel_mapping, probe
    )  # build the receive setup


if __name__ == "__main__":
    run()
