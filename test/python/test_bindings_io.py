import os
import unittest

import ultrasound_acquisition_configuration as uac


class TestBindingsIo(unittest.TestCase):

    def test_io(self):
        if hasattr(uac, "loadFromFile"):
            with self.assertRaises(RuntimeError):
                uac.loadFromFile("missing_file.uac")
            dataset = uac.Dataset()
            with self.assertRaises(RuntimeError):
                uac.validate(dataset)
            with self.assertRaises(uac.WriteFileException):
                options = uac.WriterOptions()
                if os.name == "nt":
                    self.assertRaises(RuntimeError, uac.saveToFile, "aux", dataset)
                    options.check_data = False

                    uac.saveToFile("aux", dataset, options)
                else:
                    self.assertRaises(RuntimeError, uac.saveToFile, "/", dataset)
                    options.check_data = False

                    uac.saveToFile("/", dataset, options)


if __name__ == "__main__":
    unittest.main()
