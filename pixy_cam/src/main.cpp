#include <condition_variable>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <fstream>
#include <iostream>
#include <Poco/Net/HTTPServer.h>
#include <string>
#include <unistd.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
}

#include "DateVersion.h"
#include "FfmpegException.h"
#include "FfmpegRunner.h"
#include "HttpRequestFactory.h"
#include "PixyCamera.h"
#include "PixyCameraException.h"

std::condition_variable terminateEvent;

void handle_signal( int )
{
    // On CTRL+C - abort
    terminateEvent.notify_all();
}

int main( int argc, char* argv[] )
{
    // Catch CTRL+C (SIGINT) signals
    signal( SIGINT, handle_signal );
    signal( SIGTERM, handle_signal );

    const uint16_t desiredWidth = 320;
    const uint16_t desiredHeight = 200;
    uint16_t port = 10013;

    if( argc >= 2 )
    {
        unsigned long value = std::stoul( argv[1] );
        if( value > std::numeric_limits<uint16_t>::max() )
        {
            std::cout << "Passed in port is too big.  Must be less than " << std::numeric_limits<uint16_t>::max() << std::endl;
        }
        port = static_cast<uint16_t>( value );
    }

    try
    {
        std::cout << "Server Version: " << pixy_cam::DateVersion::getVersionNumber() << std::endl;
        std::cout << "Server Built: " << pixy_cam::DateVersion::getBuildTime() << std::endl;

        pixy_cam::PixyCamera camera( desiredWidth, desiredHeight );
        camera.Init();

        std::cout << "Camera Version: " << camera.GetVersion() << std::endl;

        int32_t stopResponse;
        int stopReturn = camera.StopProgram( &stopResponse );
        if( stopReturn < 0 )
        {
            pixy_cam::PixyCameraException ex( stopReturn );
            std::cout << "STOP returned " << ex.what() << " w/response " << stopResponse << std::endl;
        }

        // Default these to true.
        camera.SetAutoWhiteBalance( true );
        camera.SetAutoExposureCompensation( true );

        // Poco is apparently smart enough to delete this automatically.
        Poco::Net::HTTPServerParams* serverParams = new Poco::Net::HTTPServerParams();
        serverParams->setMaxQueued( 100 );
        serverParams->setMaxThreads( 1 );
        Poco::Net::ServerSocket socket( port );
        Poco::Net::HTTPServer server( new pixy_cam::HttpRequestFactory( camera ), socket, serverParams );
        server.start();

        pixy_cam::FfmpegRunner ffmpeg( camera );
        ffmpeg.StartLoop();

        std::mutex m;
        std::unique_lock lock(m);
        terminateEvent.wait( lock );
        std::cout<< "SIGNAL received, terminating." << std::endl;
        server.stop();

        return 0;
    }
    catch( const pixy_cam::PixyCameraException& e )
    {
        std::cout << e.what() << std::endl;
        return e.GetErrorCode();
    }
    catch( const pixy_cam::FfmpegException& e )
    {
        std::cout << e.what() << std::endl;
        return 50;
    }
    catch( const std::exception& e )
    {
        std::cout << "FATAL" << e.what() << std::endl;
        return 100;
    }
}
