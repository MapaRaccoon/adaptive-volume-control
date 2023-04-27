#pragma once

#include <portaudiocpp/PortAudioCpp.hxx>
#include <boost/lockfree/spsc_queue.hpp>
#include "RingBuffer.hpp"

namespace sfx
{


class SmoothLimiter
{
  public:
    const struct Params
    {
        double threshold;
        double rmsWindow_ms;
    } params;

    SmoothLimiter( Params params );

    int callback(
        const void *inputBuffer,
        void *outputBuffer,
        unsigned long numFrames,
        const PaStreamCallbackTimeInfo *timeInfo,
        PaStreamCallbackFlags statusFlags
    );

  private:
    float mTotalPower;
    float mPowerWindowCount;
    
    util::RingBuffer<float> mPowerWindow;

    
};

} // namespace sfx
