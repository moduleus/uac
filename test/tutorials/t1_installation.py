import ultrasound_acquisition_configuration as uac
import ultrasound_rawdata_exchange as urx


def run():
    print(f"UAC v{uac.UAC_VERSION_MAJOR}.{uac.UAC_VERSION_MINOR}.{uac.UAC_VERSION_PATCH}")
    print(f"URX v{urx.URX_VERSION_MAJOR}.{urx.URX_VERSION_MINOR}.{urx.URX_VERSION_PATCH}")


if __name__ == "__main__":
    run()
