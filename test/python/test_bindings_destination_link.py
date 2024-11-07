def test_destination_link(
    self,
    destination_link_constructor,
    destination_link_copy,
    destination_link_args,
    trigger_in_constructor,
    group_constructor,
):
    test_name = "DestinationLink binding"
    print("\n--Test %s BEGIN--" % test_name)

    # Check default CTOR
    destination_link = destination_link_constructor()
    self.assertEqual(destination_link.trigger, None)
    self.assertEqual(destination_link.destination, None)

    # Check copy CTOR and referencing object
    destination_link_2 = destination_link_copy(destination_link)
    self.assertEqual(destination_link, destination_link_2)
    empty_group = group_constructor()
    destination_link_2.destination = empty_group
    self.assertNotEqual(destination_link, destination_link_2)
    acq_ref = destination_link
    empty_group_2 = group_constructor()
    acq_ref.destination = empty_group_2
    self.assertEqual(destination_link, acq_ref)

    group = group_constructor()
    trigger_in = trigger_in_constructor()
    destination_link = destination_link_args(trigger_in, group)
    self.assertEqual(destination_link.trigger, trigger_in)
    self.assertEqual(destination_link.destination, group)

    destination_link.trigger = None
    destination_link.trigger = trigger_in

    print("--Test %s END--" % test_name)
