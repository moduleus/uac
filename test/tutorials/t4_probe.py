import ultrasound_acquisition_configuration as uac
from helpers import makeLinearArrayUac


def run():
    # MAIN TUTORIAL

    # Probe parameters
    ProbeDescription = "SL10-2 192 elements"  # description or name of the probe
    NElements = 192  # number of transducers in the ultrasound probe
    Pitch = 300e-6  # pitch (spatial step) of the array [m]
    Width = 280e-6  # width of a single transducer [m]
    Height = 6e-3  # height of a single transducer [m]

    # Probe definition
    Probe = makeLinearArrayUac(NElements, Pitch, Width, Height, ProbeDescription)  # build the probe

    # Saves to file
    dataset = uac.Dataset()
    dataset.acquisition.probes = [Probe]
    uac.saveToFile(
        "./t4-python.uac", dataset, uac.WriterOptions(False, False, False)
    )  # saves the probe in a dataset


if __name__ == "__main__":
    run()
