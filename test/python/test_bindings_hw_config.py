from types import SimpleNamespace

import numpy as np


def test_hw_config(
    self,
    hwconfig_constructor,
    vec_float64_args,
):
    test_name = "HwConfig binding"
    print("\n--Test %s BEGIN--" % test_name)

    # Check default CTOR
    hwconfig = hwconfig_constructor()

    data = SimpleNamespace()
    data.field_u8 = np.uint8(150)
    data.field_i8 = [np.int8(50), np.int8(51)]
    data.field_u16 = np.uint16(151)
    data.field_i16 = [np.int16(60), np.int16(61)]
    data.field_u32 = np.uint32(152)
    data.field_i32 = [np.int32(70), np.int32(71)]
    data.field_u64 = np.uint64(153)
    data.field_i64 = [np.int64(80), np.int64(81)]
    data.field_float = np.float32(154)
    data.field_double = [np.float64(90), np.float64(91)]
    data.field2 = "coucuo"
    data.field3 = ["coucuo", "coucuo2"]

    data2 = SimpleNamespace()
    data2.field1 = "coucou2"
    data2.field2 = vec_float64_args([1.23, 2.34])

    data.field_hwconfig = data2

    data3 = SimpleNamespace()
    data3.field1 = "coucou3"
    data3.field2 = vec_float64_args([123, 456])

    data4 = SimpleNamespace()
    data4.field1 = "coucou4"
    data4.field2 = vec_float64_args([147, 258])

    data.field_hwconfig2 = [data3, data4]

    hwconfig.values = data

    check = hwconfig.values

    self.assertEqual(data.field_u8, check.field_u8)
    self.assertEqual(data.field_i8, check.field_i8)
    self.assertEqual(data.field_u16, check.field_u16)
    self.assertEqual(data.field_i16, check.field_i16)
    self.assertEqual(data.field_u32, check.field_u32)
    self.assertEqual(data.field_i32, check.field_i32)
    self.assertEqual(data.field_u64, check.field_u64)
    self.assertEqual(data.field_i64, check.field_i64)
    self.assertEqual(data.field_float, check.field_float)
    self.assertEqual(data.field_double, check.field_double)
    self.assertEqual(data.field2, check.field2)
    self.assertEqual(data.field3, check.field3)
    self.assertEqual(data.field_hwconfig, check.field_hwconfig)
    self.assertEqual(data.field_hwconfig2, check.field_hwconfig2)

    self.assertEqual(data, check)

    print("--Test %s END--" % test_name)
