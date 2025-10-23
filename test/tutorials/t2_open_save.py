import pathlib

import ultrasound_rawdata_exchange as urx


def run():
    # read the input file
    dataset = urx.loadFromFile(str(pathlib.Path(__file__).parent.resolve()) + "/../data/rca.urx")

    # print values
    print(dataset.acquisition.description)
    print(dataset.acquisition.excitations[0].sampling_frequency)

    # change values
    dataset.acquisition.description = "Learning to modify values ..."

    # save the output file
    urx.saveToFile("rca-python.urx", dataset, urx.WriterOptions(False, False, False))


if __name__ == "__main__":
    run()
