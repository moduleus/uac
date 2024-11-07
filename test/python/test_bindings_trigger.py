import math


def test_trigger_in(self, trigger_in_constructor, trigger_in_copy, trigger_in_args, enum_edge):
    test_name = "TriggerIn binding"
    print("\n--Test %s BEGIN--" % test_name)

    # Check default CTOR
    trigger_in = trigger_in_constructor()
    self.assertEqual(trigger_in.channel, "")
    self.assertEqual(trigger_in.edge, enum_edge().UNDEFINED)

    # Check copy CTOR and referencing object
    trigger_in_2 = trigger_in_copy(trigger_in)
    self.assertEqual(trigger_in, trigger_in_2)
    trigger_in_2.edge = enum_edge().FALLING
    self.assertNotEqual(trigger_in, trigger_in_2)
    acq_ref = trigger_in
    acq_ref.edge = enum_edge().FALLING
    self.assertEqual(trigger_in, acq_ref)

    trigger_in = trigger_in_args("Channel1", enum_edge().RISING)
    self.assertEqual(trigger_in.channel, "Channel1")
    self.assertEqual(trigger_in.edge, enum_edge().RISING)

    print("--Test %s END--" % test_name)


def test_trigger_out(
    self, trigger_out_constructor, trigger_out_copy, trigger_out_args, enum_polarity
):
    test_name = "TriggerOut binding"
    print("\n--Test %s BEGIN--" % test_name)

    # Check default CTOR
    trigger_out = trigger_out_constructor()
    self.assertEqual(trigger_out.channel, "")
    self.assertTrue(math.isnan(trigger_out.time_offset.value))
    self.assertTrue(math.isnan(trigger_out.pulse_duration.value))

    # Check copy CTOR and referencing object
    trigger_out_2 = trigger_out_copy(trigger_out)
    self.assertEqual(trigger_out, trigger_out_2)
    trigger_out_2.channel = "Channel2"
    self.assertNotEqual(trigger_out, trigger_out_2)
    acq_ref = trigger_out
    acq_ref.channel = "Channel2"
    self.assertEqual(trigger_out, acq_ref)

    trigger_out = trigger_out_args("Channel1", 1.2, 2.3, enum_polarity().POSITIVE)
    self.assertEqual(trigger_out.channel, "Channel1")
    self.assertEqual(trigger_out.time_offset, 1.2)
    self.assertEqual(trigger_out.pulse_duration, 2.3)

    print("--Test %s END--" % test_name)
