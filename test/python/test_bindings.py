import os
import sys
import unittest

import ultrasound_acquisition_configuration as uac

# CMake must be configure before launching this script.
with open(os.path.dirname(__file__) + "/urx_python_path.txt") as f:
    sys.path.append(f.readline())

from test_bindings_acquisition import test_acquisition
from test_bindings_clone import test_clone_dataset
from test_bindings_dataset import test_dataset
from test_bindings_destination_link import test_destination_link
from test_bindings_double_nan import test_double_nan
from test_bindings_element import test_element
from test_bindings_element_geometry import test_element_geometry
from test_bindings_event import test_event
from test_bindings_excitation import test_excitation
from test_bindings_group import test_group
from test_bindings_hw_config import test_hw_config
from test_bindings_impulse_response import test_impulse_response
from test_bindings_probe import test_probe
from test_bindings_receive_setup import test_receive_setup
from test_bindings_std_vector import (test_vec_element,
                                      test_vec_element_geometry_ptr,
                                      test_vec_event, test_vec_excitation_ptr,
                                      test_vec_float64,
                                      test_vec_impulse_response_ptr,
                                      test_vec_uint32, test_vec_vec_float64,
                                      test_vec_vec_uint32, test_vec_vector3d)
from test_bindings_super_group import test_super_group
from test_bindings_transform import test_transform
from test_bindings_transmit_setup import test_transmit_setup
from test_bindings_trigger import test_trigger_in, test_trigger_out
from test_bindings_utils import test_utils_io
from test_bindings_vector import test_vector2d, test_vector3d
from test_bindings_version import test_version
from test_bindings_wave import test_wave


class TestBindings(unittest.TestCase):
    default_more_event = [
        uac.DoubleNan(0),
        uac.TriggerIn(),
        uac.TriggerOut(),
        uac.HwConfig(),
    ]
    default_more_excitation = [
        uac.HwConfig(),
    ]
    default_more_group = [
        uac.DoubleNan(0),
        uac.TriggerIn(),
        uac.TriggerOut(),
        0,
        uac.VecDestinationLink(),
        uac.DoubleNan(0),
        uac.HwConfig(),
    ]
    default_more_receive_setup = [
        uac.HwConfig(),
    ]
    default_more_transmit_setup = [
        uac.HwConfig(),
    ]

    def test_acquisition_run(self):
        test_acquisition(
            self,
            acq_constructor=lambda: uac.Acquisition(),
            acq_copy=lambda copy: uac.Acquisition(copy),
            double_nan_constructor=lambda: uac.DoubleNan(),
            double_nan_copy=lambda copy: uac.DoubleNan(copy),
            probe_constructor=lambda: uac.Probe(),
            probe_copy=lambda copy: uac.Probe(copy),
            excitation_constructor=lambda: uac.Excitation(),
            excitation_copy=lambda copy: uac.Excitation(copy),
            group_constructor=lambda: uac.Group(),
            group_copy=lambda copy: uac.Group(copy),
            group_data_constructor=None,
            group_data_copy=None,
            enum_probe=lambda: uac.ProbeType,
            enum_sampling=lambda: uac.SamplingType,
        )

        initial_group = uac.Group()
        # acq =
        uac.Acquisition(
            "Authors",
            "Description",
            "fr_FR",
            "FR",
            "System",
            123,
            [uac.Probe()],
            [uac.Excitation()],
            [initial_group],
            [uac.SuperGroup()],
            initial_group,
            456,
            uac.TriggerIn(),
            uac.TriggerOut(),
            uac.HwConfig(),
        )

    def test_dataset_run(self):
        test_dataset(
            self,
            dataset_constructor=lambda: uac.Dataset(),
            dataset_copy=lambda copy: uac.Dataset(copy),
            dataset_constructor_args=lambda acquisition, version: uac.Dataset(acquisition, version),
            acq_constructor=lambda: uac.Acquisition(),
            version_copy=lambda: uac.Version(),
        )

    def test_destination_link_run(self):
        test_destination_link(
            self,
            destination_link_constructor=lambda: uac.DestinationLink(),
            destination_link_copy=lambda copy: uac.DestinationLink(copy),
            destination_link_args=lambda trigger_in, destination: uac.DestinationLink(
                trigger_in, destination
            ),
            trigger_in_constructor=lambda: uac.TriggerIn(),
            group_constructor=lambda: uac.Group(),
        )

    def test_double_nan_run(self):
        test_double_nan(
            self,
            double_nan_constructor=lambda: uac.DoubleNan(),
            double_nan_copy=lambda copy: uac.DoubleNan(copy),
        )

    def test_element_geometry_run(self):
        test_element_geometry(
            self,
            vector3d_constructor=lambda: uac.Vector3D(),
            vector3d_args=lambda x, y, z: uac.Vector3D(x, y, z),
            element_geometry_constructor=lambda: uac.ElementGeometry(),
            element_geometry_copy=lambda copy: uac.ElementGeometry(copy),
            element_geometry_args=lambda perimeter: uac.ElementGeometry(perimeter),
            vec_vector3d_args=lambda data: uac.VecVector3D(data),
        )

    def test_element_run(self):
        test_element(
            self,
            vector3d_constructor=lambda: uac.Vector3D(),
            vector3d_args=lambda x, y, z: uac.Vector3D(x, y, z),
            element_constructor=lambda: uac.Element(),
            element_copy=lambda copy: uac.Element(copy),
            element_args=lambda transform, element_geometry, impulse_response: uac.Element(
                transform, element_geometry, impulse_response
            ),
            transform_constructor=lambda: uac.Transform(),
            transform_args=lambda rotation, translation: uac.Transform(rotation, translation),
            element_geometry_constructor=lambda: uac.ElementGeometry(),
            element_geometry_args=lambda perimeter: uac.ElementGeometry(perimeter),
            impulse_response_constructor=lambda: uac.ImpulseResponse(),
            impulse_response_args=lambda sampling_frequency, time_offset, units, data: uac.ImpulseResponse(
                sampling_frequency, time_offset, units, data
            ),
        )

    def test_event_run(self):
        test_event(
            self,
            event_constructor=lambda: uac.Event(),
            event_copy=lambda copy: uac.Event(copy),
            event_args=lambda transmit_setup, receive_setup: uac.Event(
                transmit_setup, receive_setup, *self.default_more_event
            ),
            transmit_setup_constructor=lambda: uac.TransmitSetup(),
            transmit_setup_args=lambda probe, wave, active_elements, excitations, delays, probe_transform, time_offset: uac.TransmitSetup(
                probe,
                wave,
                active_elements,
                excitations,
                delays,
                probe_transform,
                time_offset,
                *self.default_more_transmit_setup,
            ),
            receive_setup_constructor=lambda: uac.ReceiveSetup(),
            receive_setup_args=lambda probe, probe_transform, sampling_frequency, number_samples, active_elements, tgc_profile, tgc_sampling_frequency, modulation_frequency, time_offset: uac.ReceiveSetup(
                probe,
                probe_transform,
                sampling_frequency,
                number_samples,
                active_elements,
                tgc_profile,
                tgc_sampling_frequency,
                modulation_frequency,
                time_offset,
                *self.default_more_receive_setup,
            ),
            probe_constructor=lambda: uac.Probe(),
            wave_constructor=lambda: uac.Wave(),
            transform_constructor=lambda: uac.Transform(),
        )

    def test_excitation_run(self):
        test_excitation(
            self,
            excitation_constructor=lambda: uac.Excitation(),
            excitation_copy=lambda copy: uac.Excitation(copy),
            excitation_args=lambda pulse_shape, transmit_frequency, sampling_frequency, waveform: uac.Excitation(
                pulse_shape,
                transmit_frequency,
                sampling_frequency,
                waveform,
                *self.default_more_excitation,
            ),
            double_nan_constructor=lambda: uac.DoubleNan(),
            double_nan_copy=lambda copy: uac.DoubleNan(copy),
            vec_float64_args=lambda data: uac.VecFloat64(data),
        )

    def test_group_run(
        self,
    ):
        test_group(
            self,
            event_constructor=lambda: uac.Event(),
            event_args=lambda transmit_setup, receive_setup: uac.Event(
                transmit_setup, receive_setup, *self.default_more_event
            ),
            vec_event_constructor=lambda: uac.VecEvent(),
            enum_sampling=lambda: uac.SamplingType,
            enum_data=lambda: uac.DataType,
            group_constructor=lambda: uac.Group(),
            group_copy=lambda copy: uac.Group(copy),
            group_args=lambda sampling_type, data_type, description, sound_speed, sequence: uac.Group(
                sampling_type,
                data_type,
                description,
                sound_speed,
                sequence,
                *self.default_more_group,
            ),
            receive_setup_args=lambda probe, probe_transform, sampling_frequency, number_samples, active_elements, tgc_profile, tgc_sampling_frequency, modulation_frequency, time_offset: uac.ReceiveSetup(
                probe,
                probe_transform,
                sampling_frequency,
                number_samples,
                active_elements,
                tgc_profile,
                tgc_sampling_frequency,
                modulation_frequency,
                time_offset,
                *self.default_more_receive_setup,
            ),
            transmit_setup_args=lambda probe, wave, active_elements, excitations, delays, probe_transform, time_offset: uac.TransmitSetup(
                probe,
                wave,
                active_elements,
                excitations,
                delays,
                probe_transform,
                time_offset,
                *self.default_more_transmit_setup,
            ),
            probe_constructor=lambda: uac.Probe(),
            wave_constructor=lambda: uac.Wave(),
            transform_constructor=lambda: uac.Transform(),
            double_nan_constructor=lambda: uac.DoubleNan(),
            double_nan_copy=lambda copy: uac.DoubleNan(copy),
        )

    def test_super_group_run(self):
        test_super_group(
            self,
            super_group_constructor=lambda: uac.SuperGroup(),
            super_group_copy=lambda copy: uac.SuperGroup(copy),
            super_group_args=lambda time_offset, trigger_in, trigger_out, repetition_count, destinations, period, hw_config, initial_group, description: uac.SuperGroup(
                time_offset,
                trigger_in,
                trigger_out,
                repetition_count,
                destinations,
                period,
                hw_config,
                initial_group,
                description,
            ),
            trigger_in_constructor=lambda: uac.TriggerIn(),
            trigger_out_constructor=lambda: uac.TriggerOut(),
            vec_destination_link_constructor=lambda: uac.VecDestinationLink(),
            hw_config_constructor=lambda: uac.HwConfig(),
            double_nan_constructor=lambda: uac.DoubleNan(),
        )

    def test_impulse_response_run(
        self,
    ):
        test_impulse_response(
            self,
            impulse_response_constructor=lambda: uac.ImpulseResponse(),
            impulse_response_copy=lambda copy: uac.ImpulseResponse(copy),
            impulse_response_args=lambda sampling_frequency, time_offset, units, data: uac.ImpulseResponse(
                sampling_frequency, time_offset, units, data
            ),
            double_nan_constructor=lambda: uac.DoubleNan(),
            double_nan_args=lambda arg1: uac.DoubleNan(arg1),
            vec_float64_args=lambda data: uac.VecFloat64(data),
        )

    def test_probe_run(
        self,
    ):
        test_probe(
            self,
            vector3d_constructor=lambda: uac.Vector3D(),
            vector3d_args=lambda x, y, z: uac.Vector3D(x, y, z),
            probe_constructor=lambda: uac.Probe(),
            probe_copy=lambda copy: uac.Probe(copy),
            probe_args=lambda description, type, transform, element_geometries, impulse_responses, elements: uac.Probe(
                description,
                type,
                transform,
                element_geometries,
                impulse_responses,
                elements,
            ),
            transform_constructor=lambda: uac.Transform(),
            transform_args=lambda rotation, translation: uac.Transform(rotation, translation),
            element_geometry_args=lambda perimeter: uac.ElementGeometry(perimeter),
            impulse_response_constructor=lambda: uac.ImpulseResponse(),
            impulse_response_args=lambda sampling_frequency, time_offset, units, data: uac.ImpulseResponse(
                sampling_frequency, time_offset, units, data
            ),
            vec_element_geometry_ptr_constructor=lambda: uac.VecElementGeometryPtr(),
            vec_element_geometry_ptr_args=lambda data: uac.VecElementGeometryPtr(data),
            vec_impulse_response_ptr_constructor=lambda: uac.VecImpulseResponsePtr(),
            vec_impulse_response_ptr_args=lambda data: uac.VecImpulseResponsePtr(data),
            element_constructor=lambda: uac.Element(),
            element_args=lambda transform, element_geometry, impulse_response: uac.Element(
                transform, element_geometry, impulse_response
            ),
            vec_element_constructor=lambda: uac.VecElement(),
            vec_element_args=lambda data: uac.VecElement(data),
            enum_probe=lambda: uac.ProbeType,
        )

    def test_receive_setup_run(
        self,
    ):
        test_receive_setup(
            self,
            receive_setup_constructor=uac.ReceiveSetup,
            receive_setup_copy=uac.ReceiveSetup,
            receive_setup_args=lambda probe, probe_transform, sampling_frequency, number_samples, active_elements, tgc_profile, tgc_sampling_frequency, modulation_frequency, time_offset: uac.ReceiveSetup(
                probe,
                probe_transform,
                sampling_frequency,
                number_samples,
                active_elements,
                tgc_profile,
                tgc_sampling_frequency,
                modulation_frequency,
                time_offset,
                *self.default_more_receive_setup,
            ),
            probe_constructor=lambda: uac.Probe(),
            probe_copy=lambda copy: uac.Probe(copy),
            transform_constructor=lambda: uac.Transform(),
            transform_args=lambda rotation, translation: uac.Transform(rotation, translation),
            double_nan_constructor=lambda: uac.DoubleNan(),
            double_nan_args=lambda data: uac.DoubleNan(data),
            vec_vec_uint32_constructor=lambda: uac.VecVecUInt32(),
            vec_float64_constructor=lambda: uac.VecFloat64(),
            vector3d_constructor=lambda: uac.Vector3D(),
            vector3d_args=lambda x, y, z: uac.Vector3D(x, y, z),
        )

    def test_vec_float64_run(
        self,
    ):
        test_vec_float64(
            self,
            vec_float64_constructor=lambda: uac.VecFloat64(),
            vec_float64_args=lambda data: uac.VecFloat64(data),
        )

    def test_vec_uint32_run(
        self,
    ):
        test_vec_uint32(
            self,
            vec_uint32_constructor=lambda: uac.VecUInt32(),
            vec_uint32_args=lambda data: uac.VecUInt32(data),
        )

    def test_vec_vec_uint32_run(
        self,
    ):
        test_vec_vec_uint32(
            self,
            vec_vec_uint32_constructor=lambda: uac.VecVecUInt32(),
            vec_vec_uint32_args=lambda data: uac.VecVecUInt32(data),
        )

    def test_vec_vec_float64_run(
        self,
    ):
        test_vec_vec_float64(
            self,
            vec_vec_float64_constructor=lambda: uac.VecVecFloat64(),
            vec_vec_float64_args=lambda data: uac.VecVecFloat64(data),
        )

    def test_vec_vector3d_run(
        self,
    ):
        test_vec_vector3d(
            self,
            vector3d_constructor=lambda: uac.Vector3D(),
            vector3d_copy=lambda copy: uac.Vector3D(copy),
            vector3d_args=lambda x, y, z: uac.Vector3D(x, y, z),
            vec_vector3d_args=lambda data: uac.VecVector3D(data),
        )

    def test_vec_element_geometry_ptr_run(
        self,
    ):
        test_vec_element_geometry_ptr(
            self,
            vector3d_constructor=lambda: uac.Vector3D(),
            vector3d_args=lambda x, y, z: uac.Vector3D(x, y, z),
            element_geometry_args=lambda perimeter: uac.ElementGeometry(perimeter),
            vec_element_geometry_ptr_args=lambda data: uac.VecElementGeometryPtr(data),
        )

    def test_vec_impulse_response_ptr_run(
        self,
    ):
        test_vec_impulse_response_ptr(
            self,
            impulse_response_constructor=lambda: uac.ImpulseResponse(),
            impulse_response_copy=lambda copy: uac.ImpulseResponse(copy),
            impulse_response_args=lambda sampling_frequency, time_offset, units, data: uac.ImpulseResponse(
                sampling_frequency, time_offset, units, data
            ),
            vec_impulse_response_ptr_args=lambda data: uac.VecImpulseResponsePtr(data),
        )

    def test_vec_element_run(
        self,
    ):
        test_vec_element(
            self,
            vector3d_constructor=lambda: uac.Vector3D(),
            vector3d_args=lambda x, y, z: uac.Vector3D(x, y, z),
            element_constructor=lambda: uac.Element(),
            element_copy=lambda copy: uac.Element(copy),
            transform_args=lambda rotation, translation: uac.Transform(rotation, translation),
            vec_element_args=lambda data: uac.VecElement(data),
        )

    def test_vec_event_run(
        self,
    ):
        test_vec_event(
            self,
            transmit_setup_args=lambda probe, wave, active_elements, excitations, delays, probe_transform, time_offset: uac.TransmitSetup(
                probe,
                wave,
                active_elements,
                excitations,
                delays,
                probe_transform,
                time_offset,
                *self.default_more_transmit_setup,
            ),
            receive_setup_args=lambda probe, probe_transform, sampling_frequency, number_samples, active_elements, tgc_profile, tgc_sampling_frequency, modulation_frequency, time_offset: uac.ReceiveSetup(
                probe,
                probe_transform,
                sampling_frequency,
                number_samples,
                active_elements,
                tgc_profile,
                tgc_sampling_frequency,
                modulation_frequency,
                time_offset,
                *self.default_more_receive_setup,
            ),
            probe_constructor=lambda: uac.Probe(),
            wave_constructor=lambda: uac.Wave(),
            transform_constructor=lambda: uac.Transform(),
            event_constructor=lambda: uac.Event(),
            event_copy=lambda copy: uac.Event(copy),
            event_args=lambda transmit_setup, receive_setup: uac.Event(
                transmit_setup, receive_setup, *self.default_more_event
            ),
            vec_event_args=lambda data: uac.VecEvent(data),
        )

    def test_vec_excitation_ptr_run(
        self,
    ):
        test_vec_excitation_ptr(
            self,
            excitation_copy=lambda copy: uac.Excitation(copy),
            excitation_args=lambda pulse_shape, transmit_frequency, sampling_frequency, waveform: uac.Excitation(
                pulse_shape,
                transmit_frequency,
                sampling_frequency,
                waveform,
                *self.default_more_excitation,
            ),
            vec_excitation_ptr_args=lambda args: uac.VecExcitationPtr(args),
            double_nan_args=lambda arg1: uac.DoubleNan(arg1),
        )

    def test_trigger_in_run(self):
        test_trigger_in(
            self,
            trigger_in_constructor=lambda: uac.TriggerIn(),
            trigger_in_copy=lambda copy: uac.TriggerIn(copy),
            trigger_in_args=lambda channel, edge: uac.TriggerIn(channel, edge),
            enum_edge=lambda: uac.Edge,
        )

    def test_trigger_out_run(self):
        test_trigger_out(
            self,
            trigger_out_constructor=lambda: uac.TriggerOut(),
            trigger_out_copy=lambda copy: uac.TriggerOut(copy),
            trigger_out_args=lambda channel, time_offset, pulse_duration, polarity: uac.TriggerOut(
                channel, time_offset, pulse_duration, polarity
            ),
            enum_polarity=lambda: uac.Polarity,
        )

    def test_transform_run(
        self,
    ):
        test_transform(
            self,
            transform_constructor=lambda: uac.Transform(),
            transform_copy=lambda copy: uac.Transform(copy),
            transform_args=lambda rotation, translation: uac.Transform(rotation, translation),
            vector3d_constructor=lambda: uac.Vector3D(),
            vector3d_args=lambda x, y, z: uac.Vector3D(x, y, z),
        )

    def test_transmit_setup_run(
        self,
    ):
        test_transmit_setup(
            self,
            transmit_setup_constructor=lambda: uac.TransmitSetup(),
            transmit_setup_copy=lambda copy: uac.TransmitSetup(copy),
            transmit_setup_args=lambda probe, wave, active_elements, excitations, delays, probe_transform, time_offset: uac.TransmitSetup(
                probe,
                wave,
                active_elements,
                excitations,
                delays,
                probe_transform,
                time_offset,
                *self.default_more_transmit_setup,
            ),
            vec_vec_uint32_constructor=lambda: uac.VecVecUInt32(),
            vec_excitation_ptr_constructor=lambda: uac.VecExcitationPtr(),
            vec_float64_constructor=lambda: uac.VecFloat64(),
            transform_constructor=lambda: uac.Transform(),
            transform_args=lambda rotation, translation: uac.Transform(rotation, translation),
            double_nan_args=lambda arg1: uac.DoubleNan(arg1),
            vector3d_constructor=lambda: uac.Vector3D(),
            vector3d_args=lambda x, y, z: uac.Vector3D(x, y, z),
            excitation_copy=lambda copy: uac.Excitation(copy),
            excitation_args=lambda pulse_shape, transmit_frequency, sampling_frequency, waveform: uac.Excitation(
                pulse_shape,
                transmit_frequency,
                sampling_frequency,
                waveform,
                *self.default_more_excitation,
            ),
            probe_constructor=lambda: uac.Probe(),
            probe_copy=lambda copy: uac.Probe(copy),
            wave_constructor=lambda: uac.Wave(),
            wave_copy=lambda copy: uac.Wave(copy),
            enum_wave=lambda: uac.WaveType,
        )

    def test_utils_io_run(
        self,
    ):
        test_utils_io(self)

    def test_utils_run(
        self,
    ):
        testName = "Utils"
        print("\n--Test %s BEGIN--" % testName)

        dataset_uac = uac.Dataset()
        dataset_uac.acquisition.description = "Hello"
        dataset_urx = uac.toUrx(dataset_uac)
        self.assertEqual(dataset_urx.acquisition.description, dataset_uac.acquisition.description)

        print("\n--Test %s END--" % testName)

    def test_vector2d_run(
        self,
    ):
        test_vector2d(
            self,
            vector2d_constructor=lambda: uac.Vector2D(),
            vector2d_copy=lambda copy: uac.Vector2D(copy),
            vector2d_args=lambda x, y: uac.Vector2D(x, y),
        )

    def test_vector3d_run(
        self,
    ):
        test_vector3d(
            self,
            vector3d_constructor=lambda: uac.Vector3D(),
            vector3d_copy=lambda copy: uac.Vector3D(copy),
            vector3d_args=lambda x, y, z: uac.Vector3D(x, y, z),
        )

    def test_version_run(
        self,
    ):
        test_version(
            self,
            version_constructor=lambda: uac.Version(),
            version_copy=lambda copy: uac.Version(copy),
            version_args=lambda major, minor, patch: uac.Version(major, minor, patch),
            default_major=uac.UAC_VERSION_MAJOR,
            default_minor=uac.UAC_VERSION_MINOR,
            default_patch=uac.UAC_VERSION_PATCH,
        )

    def test_wave_run(
        self,
    ):
        test_wave(
            self,
            wave_constructor=lambda: uac.Wave(),
            wave_copy=lambda copy: uac.Wave(copy),
            wave_args=lambda type, time_zero, time_zero_reference_point, parameters: uac.Wave(
                type, time_zero, time_zero_reference_point, parameters
            ),
            enum_wave=lambda: uac.WaveType,
            double_nan_constructor=lambda: uac.DoubleNan(),
            double_nan_args=lambda arg1: uac.DoubleNan(arg1),
            vector3d_constructor=lambda: uac.Vector3D(),
            vec_float64_constructor=lambda: uac.VecFloat64(),
        )

    def test_hw_config_run(
        self,
    ):
        test_hw_config(
            self,
            hwconfig_constructor=lambda: uac.HwConfig(),
            vec_float64_args=lambda data: uac.VecFloat64(data),
        )

    def test_clone_run(
        self,
    ):
        test_clone_dataset(self)


if __name__ == "__main__":
    unittest.main()
