#include "Audio.hpp"

#include <boost/lockfree/spsc_queue.hpp>
#include <portaudiocpp/PortAudioCpp.hxx>

#include "Stereo.hpp"

namespace sfx
{

portaudio::Device *findDeviceByName( portaudio::System &sys, const std::string &name )
{
    // loop through devices found on the system
    for ( auto &&devIter = sys.devicesBegin(); devIter != sys.devicesEnd(); devIter++ ) {
        // find PulseAudio device, called "pulse"
        if ( devIter->name() == name ) {
            // Dereference iterator into Device, then get the address
            // This device is owned by the system
            return &*devIter;
        }
    }

    // if pulse device not found, return nothing
    return nullptr;
}

void listDevices( portaudio::System &sys )
{
    // loop through devices found on the system
    for ( auto &&dev = sys.devicesBegin(); dev != sys.devicesEnd(); dev++ ) {
        std::string inputType = dev->isInputOnlyDevice()    ? "input-only"
                                : dev->isOutputOnlyDevice() ? "output-only"
                                : dev->isFullDuplexDevice() ? "full-duplex"
                                                            : "unknown-input-type";
        std::cout << "(" << dev->index() << ") " << dev->name() << "; " << inputType << std::endl;
    }
}

portaudio::StreamParameters getFullDuplexStreamParameters(
    portaudio::Device &device,
    unsigned long framesPerBuffer,
    double sampleRate
)
{
    // Set up the parameters required to open a (Callback)Stream:
    portaudio::DirectionSpecificStreamParameters inParams( //
        device,                                            // device
        2,                                                 // numChannels
        portaudio::FLOAT32,                                // format
        true,                                              // interleaved
        device.defaultLowInputLatency(),                   // suggestedLatency
        NULL                                               // hostApiSpecifiStreamInfo
    );

    portaudio::DirectionSpecificStreamParameters outParams( //
        device,                                             // device
        2,                                                  // numChannels
        portaudio::FLOAT32,                                 // format
        true,                                               // interleaved
        device.defaultLowOutputLatency(),                   // suggestedLatency
        NULL                                                // hostApiSpecifiStreamInfo
    );

    return portaudio::StreamParameters( //
        inParams,                       // inputParameters
        outParams,                      // outpuParameters
        sampleRate,                     // sampleRate
        framesPerBuffer,                // framesPerBuffer
        paClipOff                       // flags
    );
}

StreamGuard::StreamGuard( portaudio::Stream &stream ) : stream_( stream )
{
    stream_.start();
}
StreamGuard::~StreamGuard()
{
    stream_.stop();
}

} // namespace sfx
