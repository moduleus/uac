import os

import ultrasound_acquisition_configuration as uac
import unittest


class TestBindingsIo(unittest.TestCase):

    def test_io(self):
        if hasattr(uac, "loadFromFile"):
            with self.assertRaises(RuntimeError):
                uac.loadFromFile("missing_file.uac")
            dataset = uac.Dataset()
            with self.assertRaises(uac.WriteFileException):
                if os.name == "nt":
                    uac.saveToFile("aux", dataset)
                else:
                    uac.saveToFile("/", dataset)


if __name__ == "__main__":
    unittest.main()
