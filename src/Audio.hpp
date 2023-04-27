#pragma once

#include "Stereo.hpp"
#include <boost/lockfree/spsc_queue.hpp>
#include <portaudiocpp/PortAudioCpp.hxx>

namespace sfx
{

const double SAMPLE_RATE = 44100.0;
const int FRAMES_PER_BUFFER = 1024;
const double SAMPLES_PER_MS = SAMPLE_RATE / 1000.0;

portaudio::Device *findDeviceByName( portaudio::System &sys, const std::string &name );
void listDevices( portaudio::System &sys );
portaudio::StreamParameters getFullDuplexStreamParameters(
    portaudio::Device &device,
    unsigned long framesPerBuffer,
    double sampleRate
);
portaudio::FunCallbackStream createFullDuplexStream(
    portaudio::Device &device,
    unsigned long framesPerBuffer,
    double sampleRate
);

class StreamGuard
{
  public:
    StreamGuard( portaudio::Stream & );
    ~StreamGuard();
    StreamGuard( const StreamGuard & ) = delete;
    StreamGuard operator=( const StreamGuard & ) = delete;
    StreamGuard( const StreamGuard && ) = delete;
    StreamGuard operator=( const StreamGuard && ) = delete;

  private:
    portaudio::Stream &stream_;
};

} // namespace sfx
