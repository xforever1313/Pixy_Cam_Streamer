#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <fstream>
#include <iostream>
#include <jpeglib.h>
#include <string>
#include <unistd.h>
#include "pixy.h"

void handle_SIGINT(int unused)
{
    // On CTRL+C - abort //
    std::cout<< "SIGINT received, terminating." << std::endl;
    exit(0);
}

int CaptureFrame( const std::string& outputFile )
{
    unsigned char current_frame[72000]; // ~largest possible given current hardware
    unsigned char *pixels;  //returned pointer to video frame buffer
    int32_t response, fourcc;
    int8_t renderflags;
    int return_value, res;
    uint16_t width, height;
    uint32_t  numPixels;

    //  stop blob processing
    return_value = pixy_command( "stop", END_OUT_ARGS, &response, END_IN_ARGS );
    std::cout << "STOP returned " << return_value << " w/response " << response << std::endl;

    return_value = pixy_cam_set_auto_white_balance( 1 );
    std::cout << "Enable white balance returned " << return_value << std::endl;
    if( return_value < 0 )
    {
        return return_value;
    }

    response = 0;
    return_value = pixy_command(
        "cam_getFrame",  // String id for remote procedure
        0x01, 0x21,      // mode 0 = 1280x800 25 fps.  1 byte
        0x02,   0,       // xoffset - 2 bytes
        0x02,   0,       // yoffset - 2 bytes
        0x02, 320,       // width   - 2 bytes
        0x02, 200,       // height  - 2 bytes
        0,               // separator
        &response,       // pointer to mem address for return value
        &fourcc,         //contrary to docs, the next 5 args are needed
        &renderflags,
        &width,
        &height,
        &numPixels,
        &pixels,        // pointer to mem address for returned frame
        0
    );

    std::cout << "getFrame returned " << return_value << " w/response " << response << std::endl;
    std::cout << "returned width: " << width << ", height: " << height << ", num pixels: " << numPixels << std::endl;

    // quit now if not successful:
    if( return_value < 0 )
    {
        return return_value;
    }

    // save this block
    memcpy( &current_frame, pixels, numPixels );

    std::ofstream file( outputFile, std::ios::binary );

    if ( file.is_open() == false )
    {
        std::cout << "Failed to open file for writing: " << outputFile << std::endl;
        return 200;
    }

    file.write( reinterpret_cast<const char*>( current_frame ), numPixels );

    if ( file.fail() )
    {
        std::cout << "Failed to write data to file: " << outputFile << std::endl;
        file.close();
        return 300;
    }

    file.close();
    return 0;
}

int main( int argc, char * argv[] )
{
    if( argc <= 1 )
    {
        std::cout << "Usage: <output_file.jpg>" << std::endl;
        return 1;
    }

    int pixy_init_status;

    // Catch CTRL+C (SIGINT) signals
    signal( SIGINT, handle_SIGINT );

    // Connect to Pixy
    pixy_init_status = pixy_init();
    std::cout << "Initialized Pixy - " << pixy_init_status << std::endl;

    if( pixy_init_status != 0 )
    {
        // Error initializing Pixy
        pixy_error( pixy_init_status );
        pixy_close();
        return pixy_init_status;
    }

    int exitCode = CaptureFrame( std::string( argv[1] ) );
    if( exitCode < 0 )
    {
        pixy_error( exitCode );
    }
    pixy_close();
    return exitCode;
}
