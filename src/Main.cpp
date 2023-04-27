#include "Audio.hpp"
#include "SmoothLimiter.hpp"
#include "Stereo.hpp"
#include <boost/lockfree/spsc_queue.hpp>
#include <iostream>
#include <portaudiocpp/PortAudioCpp.hxx>
#include <vector>

int main( void )
{
    portaudio::AutoSystem autoSys;
    portaudio::System &sys = portaudio::System::instance();

    std::cout << "Searching for PulseAudio device..." << std::endl;
    sfx::listDevices( sys );
    portaudio::Device *pulse = sfx::findDeviceByName( sys, "pulse" );
    if ( !pulse )
        throw std::runtime_error( "pulse device not found" );

    std::cout << "Starting stream" << std::endl;
    auto streamParams = sfx::getFullDuplexStreamParameters( *pulse, sfx::FRAMES_PER_BUFFER, sfx::SAMPLE_RATE );
    sfx::SmoothLimiter limiter( {
        .threshold = 0.01,
        .rmsWindow_ms = 500,
    } );
    auto stream = portaudio::MemFunCallbackStream( streamParams, limiter, &sfx::SmoothLimiter::callback );
    auto streamInputGuard = sfx::StreamGuard( stream );

    std::cout << "(press enter to exit)";
    std::cin.get();

    return 0;
}
