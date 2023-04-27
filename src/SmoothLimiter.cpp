#include "SmoothLimiter.hpp"
#include "Audio.hpp"
#include "Stereo.hpp"
#include <portaudiocpp/PortAudioCpp.hxx>

#include <cmath>


namespace sfx
{

SmoothLimiter::SmoothLimiter( Params params )
    : params( params )
    , mPowerWindow( params.rmsWindow_ms * SAMPLE_RATE / 1000.0 )
    , mTotalPower( 0 )
    , mPowerWindowCount( 0 )
{
}

int SmoothLimiter::callback(
    const void *inputBuffer,
    void *outputBuffer,
    unsigned long numFrames,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags
)
{
    const Stereo<float> *in = static_cast<const Stereo<float> *>( inputBuffer );
    Stereo<float> *out = static_cast<Stereo<float> *>( outputBuffer );
    float threshold = params.threshold;

    // calculate rms amplitude at the end of current window
    float rms;
    { 
        int i = 0;
        // initial (playback begins with empty power window)
        while ( i < numFrames && mPowerWindow.canWrite() ) {
            float sample = in [ i ].left + in [ i ].right;
            float power = sample * sample;
            mPowerWindow.push(power);
            mTotalPower += power;
            i++;
            mPowerWindowCount++;
        }

        // subsequent (after full power window)
        while ( i < numFrames ) {
            float sample =in [ i ].left + in [ i ].right;
            float power = sample * sample;
            float tailPower;
            mPowerWindow.pop(tailPower);
            mPowerWindow.push(power);
            mTotalPower += power - tailPower;
            i++;
        }
        rms = std::sqrt(mTotalPower / mPowerWindow.size());
    }

    // do the volume control
    for ( int i = 0; i < numFrames; i++ ) {
        if (rms > threshold) out[i] = in[i] * threshold / rms;
        else out[i] = in[i];
    }

        return paContinue;
}

} // namespace sfx
