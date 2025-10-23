import os

import matplotlib.pyplot as plt
import t1_installation
import t2_open_save
import t3_rf_data
import t4_probe
import t5_transmit_setup
import t6_receive_setup
import t7_event
import t8_acquisition
import t9_stream

if __name__ == "__main__":
    t1_installation.run()
    t2_open_save.run()
    os.remove("rca-python.urx")
    plt.close(t3_rf_data.run())
    t4_probe.run()
    t5_transmit_setup.run()
    t6_receive_setup.run()
    t7_event.run()
    t8_acquisition.run()
    t9_stream.run()
