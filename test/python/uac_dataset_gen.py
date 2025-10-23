import os
import sys
from types import SimpleNamespace

import numpy as np

# CMake must be configure before launching this script.
with open(os.path.dirname(__file__) + "/urx_python_path.txt") as f:
    sys.path.append(f.readline())

from dataset_gen import gen_fake_dataset


def uac_gen_fake_dataset(
    dataset_constructor,
    probe_constructor,
    excitation_constructor,
    group_constructor,
    element_geometry_constructor,
    element_constructor,
    event_constructor,
    impulse_response_constructor,
    transform_args_constructor,
    vector3D_args_constructor,
    enum_probe,
    enum_sampling,
    enum_data,
    enum_wave,
    super_group_constructor,
    trigger_in_constructor,
    trigger_out_constructor,
    destination_link_constructor,
    hwconfig_constructor,
    enum_edge,
    enum_polarity,
):

    dataset = gen_fake_dataset(
        dataset_constructor,
        probe_constructor,
        excitation_constructor,
        group_constructor,
        None,
        element_geometry_constructor,
        element_constructor,
        event_constructor,
        impulse_response_constructor,
        transform_args_constructor,
        vector3D_args_constructor,
        enum_probe,
        enum_sampling,
        enum_data,
        enum_wave,
    )

    trigger_in = trigger_in_constructor()
    trigger_out = trigger_out_constructor()
    super_group = super_group_constructor()

    super_group.time_offset = 1.23
    trigger_in.channel = "Channel1"
    trigger_in.edge = enum_edge().FALLING
    super_group.trigger_in = trigger_in
    trigger_out.channel = "Channel5"
    trigger_out.pulse_duration = 7.41
    trigger_out.time_offset = 92
    super_group.trigger_out = trigger_out
    super_group.repetition_count = 45

    destination_link = destination_link_constructor()
    destination_link.destination = dataset.acquisition.groups[1]
    trigger_in.channel = "Channel6"
    trigger_in.edge = enum_edge().LOW
    destination_link.trigger = trigger_in
    super_group.destinations.append(destination_link)

    destination_link.destination = dataset.acquisition.groups[0]
    trigger_in.channel = "Channel8"
    trigger_in.edge = enum_edge().HIGH
    destination_link.trigger = trigger_in
    super_group.destinations.append(destination_link)

    super_group.period = 11

    super_group.hw_config.values = SimpleNamespace(
        str=["coucoud", "coucoud2"], numb=np.int32(125), vect=[10, 11, 12, 13, 14]
    )

    super_group.initial_group = dataset.acquisition.groups[0]
    super_group.description = "Hello"

    dataset.acquisition.super_groups.append(super_group)
    dataset.acquisition.super_groups.append(super_group_constructor())
    dataset.acquisition.initial_group = super_group
    dataset.acquisition.time_offset = np.nan
    trigger_in.channel = "Channel2"
    trigger_in.edge = enum_edge().RISING
    dataset.acquisition.trigger_in = trigger_in
    trigger_out.channel = "Channel4"
    trigger_out.pulse_duration = 741
    trigger_out.time_offset = 9.2
    trigger_out.polarity = enum_polarity().NEGATIVE
    dataset.acquisition.trigger_out = trigger_out

    dataset.acquisition.hw_config = hwconfig_constructor()
    dataset.acquisition.hw_config.values = SimpleNamespace(
        int8_t=np.int8(1),
        int16_t=np.int16(1),
        int32_t=np.int32(1),
        int64_t=np.int64(1),
        uint8_t=np.uint8(1),
        uint16_t=np.uint16(1),
        uint32_t=np.uint32(1),
        uint64_t=np.uint64(1),
        float=np.float32(1),
        double=np.float64(1),
        string="coucou",
        v_int8_t=np.int8(1),
        v_int16_t=[np.int16(1)],
        v_int32_t=[np.int32(1)],
        v_int64_t=[np.int64(1)],
        v_uint8_t=[np.uint8(1)],
        v_uint16_t=[np.uint16(1)],
        v_uint32_t=[np.uint32(1)],
        v_uint64_t=[np.uint64(1)],
        v_float=[np.float32(1)],
        v_double=[np.float64(1)],
        v_string=["coucou"],
        numb=np.float32(12.5),
        vect=[10, 11, 12, 13, 14],
        data2=[SimpleNamespace(str2="coucou2", vect2=[9, 8, 7, 6])],
    )

    group_ref = dataset.acquisition.groups[1]
    dataset.acquisition.groups.append(group_constructor())

    group_ref.time_offset = -450

    trigger_in.channel = "Channel15"
    trigger_in.edge = enum_edge().FALLING
    group_ref.trigger_in = trigger_in
    trigger_out.channel = "Channel24"
    trigger_out.pulse_duration = 7541
    trigger_out.time_offset = 9.32
    trigger_out.polarity = enum_polarity().POSITIVE
    group_ref.trigger_out = trigger_out
    group_ref.repetition_count = 777
    destination_link.destination = dataset.acquisition.groups[0]
    trigger_in.channel = "Channel8"
    trigger_in.edge = enum_edge().HIGH
    destination_link.trigger = trigger_in
    super_group.destinations.append(destination_link)
    super_group.destinations.append(destination_link)
    super_group.destinations.append(destination_link)
    super_group.destinations.append(destination_link)
    super_group.destinations.append(destination_link)
    super_group.destinations.append(destination_link_constructor())
    super_group.period = 448.3
    super_group.hw_config.values = SimpleNamespace(str="coucou")

    event_ref = group_ref.sequence[0]
    event_ref.time_offset = 741
    trigger_in.channel = "Channel10"
    trigger_in.edge = enum_edge().UNDEFINED
    event_ref.trigger_in = trigger_in
    trigger_out.channel = "Channel11"
    trigger_out.pulse_duration = 354
    trigger_out.time_offset = 95.12
    trigger_out.polarity = enum_polarity().UNDEFINED
    event_ref.trigger_out = trigger_out
    event_ref.hw_config.values = SimpleNamespace(
        num=np.int16(9874), vect=[np.int64(10), np.int64(9994)]
    )
    event_ref.receive_setup.hw_config.values = SimpleNamespace(num=np.uint64(9874))
    event_ref.transmit_setup.hw_config.values = SimpleNamespace(num=np.uint32(9874))

    excitation_ref = dataset.acquisition.excitations[0]
    excitation_ref.hw_config.values = SimpleNamespace(
        num=np.uint16(9874), vect=[np.int16(741), np.int16(-752)]
    )

    return dataset
