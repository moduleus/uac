import pathlib

import matplotlib.pyplot as plt
import ultrasound_rawdata_exchange as urx


def getRfFromGroupsData(groupData: urx.GroupData):

    groupDataReader = urx.GroupDataReader(groupData)

    Nsamples = groupDataReader.samplesCount(0)

    Nchannels = groupDataReader.channelsCount(0)

    Nevents = groupDataReader.eventsCount()

    Nsequences = groupDataReader.sequencesCount()

    return groupData.raw_data.reshape(Nsamples, Nchannels, Nevents, Nsequences)


def run():
    # read the input file
    dataset = urx.loadFromFile(
        str(pathlib.Path(__file__).parent.resolve()) + "/../data/single_int16_rf.urx"
    )

    # groups_data identifier, if there are multiples in the same URX.
    gid = 0

    # size(rf) = [Nsamples, Nchannels, Nevents, Nsequences]
    rf = getRfFromGroupsData(dataset.acquisition.groups_data[gid])
    fig = plt.figure()
    plt.plot(rf[:, 0, 0, 0])
    return fig


if __name__ == "__main__":
    run()
