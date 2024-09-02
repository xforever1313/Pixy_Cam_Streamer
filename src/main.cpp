//
// Pixy Cam Streamer - Stream a Pixy Camera to a RTMP server
// Copyright (C) 2024 Seth Hendrick
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
// 
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

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

#include "DateVersion.h"
#include "FfmpegException.h"
#include "FfmpegProcessRunner.h"
#include "HttpRequestFactory.h"
#include "PixyCamera.h"
#include "PixyCamStreamConfig.h"
#include "PixyCameraException.h"

std::condition_variable terminateEvent;

void handle_signal( int )
{
    // On CTRL+C - abort
    terminateEvent.notify_all();
}

void PrintUsage( const std::string& exeName )
{
    std::cout << "Start Server: " << exeName << " [config_file_path]" << std::endl;
    std::cout << "\tconfig_file_path is defaulted to /etc/pixy_cam_streamer/pixy_cam_streamer.cfg" << std::endl;
    std::cout << "Print Help: " << exeName << " --help" << std::endl;
    std::cout << "Print Version: " << exeName << " --version" << std::endl;
}

void PrintVersion()
{
    std::cout << "Server Version: " << pixy_cam::DateVersion::getVersionNumber() << std::endl;
    std::cout << "Server Built: " << pixy_cam::DateVersion::getBuildTime() << std::endl;
}

int main( int argc, char* argv[] )
{
    std::string configFile = "/etc/pixy_cam_streamer/pixy_cam_streamer.cfg";

    if( argc <= 1 )
    {
        std::cout << "Invalid Arguments.  See below for usage." << std::endl;
        PrintUsage( argv[0] );
        return 1;
    }
    else if( argc >= 2 )
    {
        if( std::string( "--help" ) == argv[1] )
        {
            PrintUsage( argv[0] );
            return 0;
        }
        else if( std::string( "--version" ) == argv[1] )
        {
            PrintVersion();
            return 0;
        }
        else
        {
            configFile = argv[1];
        }
    }

    // Catch CTRL+C (SIGINT) signals
    signal( SIGINT, handle_signal );
    signal( SIGTERM, handle_signal );

    const uint16_t desiredWidth = 320;
    const uint16_t desiredHeight = 200;

    try
    {
        pixy_cam::PixyCamStreamConfig config( configFile );
        PrintVersion();

        std::cout << "Controller HTTP port: " << config.Web_ListenPort() << std::endl;
        std::cout << std::endl;
        std::cout << "Enable auto white balance: " << config.Camera_AutoWhiteBalance() << std::endl;
        std::cout << "Enable auto exposure compensation: " << config.Camera_AutoExposureCompensation() << std::endl;
        std::cout << "Default Camera Brightness: " << std::to_string( config.Camera_Brightness() ) << std::endl;
        std::cout << std::endl;
        std::cout << "FPS: " << std::to_string( config.Stream_Fps() ) << std::endl;
        std::cout << "FFMPEG Exe Location: " << config.Stream_FfmpegPath() << std::endl;
        std::cout << "RTMP Server: " << config.Stream_RtmpServer() << std::endl;
        std::cout << std::endl;

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

        // Set initial camera settings.
        camera.SetAutoWhiteBalance( config.Camera_AutoWhiteBalance() );
        camera.SetAutoExposureCompensation( config.Camera_AutoExposureCompensation() );
        camera.SetBrightness( config.Camera_Brightness() );

        pixy_cam::FfmpegProcessRunner ffmpeg( camera, config );
        ffmpeg.Init();

        // Poco is apparently smart enough to delete this automatically.
        Poco::Net::HTTPServerParams* serverParams = new Poco::Net::HTTPServerParams();
        serverParams->setMaxQueued( 100 );
        serverParams->setMaxThreads( 1 );
        Poco::Net::ServerSocket socket( config.Web_ListenPort() );
        Poco::Net::HTTPServer server( new pixy_cam::HttpRequestFactory( ffmpeg, camera ), socket, serverParams );

        server.start();
        #ifndef FAKE_CAMERA
        ffmpeg.StartLoop();
        #endif

        std::mutex m;
        std::unique_lock<std::mutex> lock( m );
        terminateEvent.wait( lock );
        std::cout<< "SIGNAL received, terminating." << std::endl;
        server.stop();
        ffmpeg.StopLoop();

        return 0;
    }
    catch( const pixy_cam::PixyCameraException& e )
    {
        std::cout << e.what() << std::endl;
        return e.GetErrorCode();
    }
    catch( const libconfig::ConfigException &e )
    {
        std::cout << "Configuration error:" << std::endl;
        std::cout << e.what() << std::endl;
        return 25;
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
    catch( ... )
    {
        std::cout << "Unknown Exception" << std::endl;
        return 200;
    }
}
