import ultrasound_acquisition_configuration as uac
from uac_dataset_gen import uac_gen_fake_dataset


def generic_clone_test(self, var, copy_ctor, clone):
    var_ref = var
    var_2 = copy_ctor(var)
    var_cloned = clone(var)
    self.assertEqual(var, var_ref)
    self.assertEqual(var, var_2)
    self.assertEqual(var, var_cloned)
    self.assertEqual(hex(id(var)), hex(id(var_ref)))
    self.assertNotEqual(hex(id(var)), hex(id(var_2)))
    self.assertNotEqual(hex(id(var)), hex(id(var_cloned)))


def test_clone_dataset(self):
    testName = "Clone Dataset binding"
    print("\n--Test %s BEGIN--" % testName)

    d = uac_gen_fake_dataset(
        dataset_constructor=lambda: uac.Dataset(),
        probe_constructor=lambda: uac.Probe(),
        excitation_constructor=lambda: uac.Excitation(),
        group_constructor=lambda: uac.Group(),
        element_geometry_constructor=lambda: uac.ElementGeometry(),
        element_constructor=lambda: uac.Element(),
        event_constructor=lambda: uac.Event(),
        impulse_response_constructor=lambda: uac.ImpulseResponse(),
        transform_args_constructor=lambda rotation, translation: uac.Transform(
            rotation, translation
        ),
        vector3D_args_constructor=lambda x, y, z: uac.Vector3D(x, y, z),
        enum_probe=lambda: uac.ProbeType,
        enum_sampling=lambda: uac.SamplingType,
        enum_data=lambda: uac.DataType,
        enum_wave=lambda: uac.WaveType,
        super_group_constructor=lambda: uac.SuperGroup(),
        trigger_in_constructor=lambda: uac.TriggerIn(),
        trigger_out_constructor=lambda: uac.TriggerOut(),
        destination_link_constructor=lambda: uac.DestinationLink(),
        hwconfig_constructor=lambda: uac.HwConfig(),
        enum_edge=lambda: uac.Edge,
        enum_polarity=lambda: uac.Polarity,
    )

    generic_clone_test(self, d, lambda copy: uac.Dataset(copy), uac.clone)

    print("--Test %s END--" % testName)
