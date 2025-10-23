#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <urx/probe.h>

#include <uac/excitation.h>
#include <uac/group.h>
#include <uac/probe.h>
#include <uac/receive_setup.h>
#include <uac/transmit_setup.h>

// makeLinearArrayUac Creates a linear array probe in uac format
// Inputs:
//   n_elements       : number of elements (transducers) in the probe
//   pitch           : distance between the center of adjacent elements [m]
//   width           : size of the element along the x-axis [m]
//   height          : size of the element along the y-axis [m]
//   description     : name or description of the probe
//
// Outputs:
//   Probe (uac.Probe) : built as a linear array along the x-axis
//
// Drawing: Linear array (n_elements = 12)
//                                          __ width
//       ____ x    __ __ __ __ __ __ __ __ __ __ __ __
//      /|        / // // // // // // // // // // // /    /
//     / |       /_//_//_//_//_//_//_//_//_//_//_//_/    / height
//    y   z                 ___ pitch
//
//      +--+       perimeter of and element with + as elements edges
//     /  /       defined either clockwise or counterclockwise
//    +--+
//
// For more information see : https://github.com/moduleus/uac/wiki
//
// Requirements:     uac 1.2.0 toolbox
std::shared_ptr<uac::Probe> makeLinearArrayUac(size_t n_elements, double pitch, double width,
                                               double height, const std::string& description);

/*
makePlaneWaveTransmitSetupUac creates a plane waves transmit setup with a linear array in uac format
Inputs:
    theta                       : plane wave angle of the event [°]
    voltage                     : voltage of the excitation [Vpeak]
    n_periods                    : number of periods of transmitted signal
    transmit_frequency           : central frequency of the transmitted signal [Hz]
    max_delay                    : maximum delay of the plane wave sequence [s]
    sound_speed                  : celerity of the sound in the medium [m/s]
    probe (uac.Probe)           : ultrasound probe used for the transmit event
    excitation (uac.Excitation) : (optional) excitation reused from another transmit event
Outputs:
    TransmitSetup (uac.TransmitSetup)   : built for plane wave transmit event with a linear array probe
    Excitation (uac.Excitation)         : excitation to be used in another transmit event
Requirements:
    uac 1.2.0 toolbox
More information:
    see : https://github.com/moduleus/uac/wiki
*/
std::pair<uac::TransmitSetup, std::shared_ptr<uac::Excitation>> makePlaneWaveTransmitSetupUac(
    double theta, double voltage, size_t n_periods, double transmit_frequency, double max_delay,
    double sound_speed, const std::vector<std::vector<uint32_t>>& channel_mapping,
    const std::shared_ptr<uac::Probe>& probe,
    const std::shared_ptr<uac::Excitation>& excitation_input = {});

/*
makeReceiveSetupUac Creates a receive setup in uac format
Inputs:
  nSamples                : number of samples per channel per reception event 
  rxTimeOffset            : time before the sampling of the received signal [s]
  samplingFrequency       : sampling frequency of the backscattered signal [Hz]
  modulationFrequency     : frequency used for the demodulation of received signal in IQ [Hz]   
  channelMapping          : order of channels in the interface (start at 0)
  Probe (uac.Probe)       : ultrasound probe used for the reception event

Outputs:
  ReceiveSetup (uac.ReceiveSetup) : built for reception event with all the elements of the probe

For more information see : https://github.com/moduleus/uac/wiki

Requirements:     uac 1.2.0 toolbox
*/
uac::ReceiveSetup makeReceiveSetupUac(uint32_t n_samples, double rx_time_offset,
                                      double sampling_frequency, double modulation_frequency,
                                      const std::vector<std::vector<uint32_t>>& channel_mapping,
                                      const std::shared_ptr<uac::Probe>& probe);

/*
    makePlaneWavesGroupUac creates a plane waves sequence with a linear array in uac format
    Inputs:
       thetas                      : plane wave angles of the sequence [°]
       voltage                     : voltage of the excitation [Vpeak]
       nPeriods                    : number of periods of transmitted signal
       prf                         : pulse repetition frequency [Hz]
       nFrames                     : number of repetition of the sequence
       frameRate                   : repetition frequency of the sequence [Hz]
       transmitFrequency           : central frequency of the transmitted signal [Hz]
       soundSpeed                  : celerity of the sound in the medium [m/s]
       nSamples                    : number of samples per channel per reception event
       rxTimeOffset                : time before the sampling of the received signal [s]
       samplingFrequency           : sampling frequency of the backscattered signal [Hz]
       modulationFrequency         : frequency used for the demodulation of received signal in IQ [Hz]
       channelMapping              : order of channels in the interface (start at 0)
       Probe (uac.Probe)           : ultrasound probe used for the transmit/reception event
    Outputs:
        Group (uac.Group)           : built for plane wave transmit/reception event with a linear array probe
        Excitation (uac.Excitation) : excitation to be used in another transmit event
    Requirements:
        urx 1.2.0 toolbox
        uac 1.2.0 toolbox
    More information:
        see : https://github.com/moduleus/urx/wiki
*/
std::pair<std::shared_ptr<uac::Group>, std::shared_ptr<uac::Excitation>> makePlaneWavesGroupUac(
    const std::vector<double>& thetas, double voltage, size_t n_periods, double prf,
    uint32_t n_frames, double frame_rate, double transmit_frequency, double sound_speed,
    uint32_t n_samples, double rx_time_offset, double sampling_frequency,
    double modulation_frequency, const std::vector<std::vector<uint32_t>>& channel_mapping,
    const std::shared_ptr<uac::Probe>& probe);
