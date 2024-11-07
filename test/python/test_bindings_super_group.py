def test_super_group(
    self,
    super_group_constructor,
    super_group_copy,
    super_group_args,
    trigger_in_constructor,
    trigger_out_constructor,
    vec_destination_link_constructor,
    hw_config_constructor,
    double_nan_constructor,
):
    test_name = "SuperGroup binding"
    print("\n--Test %s BEGIN--" % test_name)

    # Check default CTOR
    super_group = super_group_constructor()
    self.assertEqual(super_group.time_offset, 0)
    self.assertEqual(super_group.trigger_in, None)
    self.assertEqual(super_group.trigger_out, None)
    self.assertEqual(super_group.repetition_count, 0)
    self.assertEqual(super_group.destinations, vec_destination_link_constructor())
    self.assertEqual(super_group.period, double_nan_constructor())
    self.assertEqual(super_group.hw_config, hw_config_constructor())
    self.assertEqual(super_group.initial_group, None)
    self.assertEqual(super_group.description, "")

    # Check copy CTOR and referencing object
    super_group_2 = super_group_copy(super_group)
    self.assertEqual(super_group, super_group_2)
    super_group_2.repetition_count = 2
    self.assertNotEqual(super_group, super_group_2)
    acq_ref = super_group
    acq_ref.repetition_count = 2
    self.assertEqual(super_group, acq_ref)

    initial_group = super_group_constructor()
    trigger_in = trigger_in_constructor()
    trigger_out = trigger_out_constructor()
    empty_destinations = vec_destination_link_constructor()
    hw_config = hw_config_constructor()
    description = "Hello"
    super_group = super_group_args(
        0, trigger_in, trigger_out, 1, empty_destinations, 2, hw_config, initial_group, description
    )
    self.assertEqual(super_group.time_offset, 0)
    self.assertEqual(super_group.trigger_in, trigger_in_constructor())
    self.assertEqual(super_group.trigger_out, trigger_out_constructor())
    self.assertEqual(super_group.repetition_count, 1)
    self.assertEqual(super_group.destinations, vec_destination_link_constructor())
    self.assertEqual(super_group.period, 2)
    self.assertEqual(super_group.hw_config, hw_config_constructor())
    self.assertEqual(super_group.initial_group, initial_group)
    self.assertEqual(super_group.description, description)

    print("--Test %s END--" % test_name)
