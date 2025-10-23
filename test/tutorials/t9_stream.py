import numpy as np
import ultrasound_acquisition_configuration as uac
import ultrasound_rawdata_exchange as urx


def compare_complex_arrays(arr1, arr2, rtol=1e-05, atol=1e-08):
    def is_real_array(arr):
        return arr.dtype != complex and not (arr.ndim == 2 and arr.shape[1] == 2)

    if is_real_array(arr1) and is_real_array(arr2):
        return np.allclose(arr1, arr2, rtol=rtol, atol=atol)

    def to_complex(arr):
        if arr.dtype == complex:
            return arr
        elif arr.ndim == 2 and arr.shape[1] == 2:
            return arr[:, 0] + 1j * arr[:, 1]
        else:
            return arr.astype(complex)

    complex1 = to_complex(arr1)
    complex2 = to_complex(arr2)

    return np.allclose(complex1, complex2, rtol=rtol, atol=atol)


def run():
    uac_filename = "./t8-python.uac"
    urx_filename = "./t9-python.urx"

    # Generate urx without group_data
    uac_dataset = uac.loadFromFile(uac_filename)
    urx_dataset = uac.toUrx(uac_dataset)

    # The urx file must exist and be saved before using stream.
    urx.saveToFile(urx_filename, urx_dataset, urx.WriterOptions(False, False, False))

    group_idx = 0

    sequence_size = sum(
        len(event.receive_setup.active_elements) * event.receive_setup.number_samples
        for event in urx_dataset.acquisition.groups[group_idx].sequence
    )

    raw_data_vector_short = np.arange(sequence_size, dtype=np.int16)

    stream = urx.Stream(urx_filename, urx_dataset)
    stream.writerOptions().clean_unusable_data = True
    group_data = stream.createGroupData(urx_dataset.acquisition.groups[group_idx], 2.0)

    # It's not mandatory to write the full sequence at once.
    group_data.append(raw_data_vector_short, 9.0, [8.0])

    # Stream and GroupDataStream keep an HDF5 object. So destroy it to release lock on file.
    del group_data
    del stream

    dataset_loaded = urx.Dataset()
    stream = urx.Stream(urx_filename, dataset_loaded)
    stream.readerOptions().raw_data_load_policy = urx.RawDataLoadPolicy.STREAM
    stream.loadFromFile()

    group_data_idx = 0

    # Buffer must reserve memory.
    buffer = np.ndarray((sequence_size), dtype=np.int16)

    # It's not mandatory to write the full sequence at once.
    stream.readRawData(
        group_data_idx,
        buffer,
        0,  # Index of the sequence in the buffer.
        0,  # Index of the sequence in the urx file.
        1,  # Count of sequence to read.
    )

    if not compare_complex_arrays(raw_data_vector_short, buffer):
        raise Exception("Something went wrong")


if __name__ == "__main__":
    run()
