#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

#include "FfmpegRunner.h"
#include "FfmpegException.h"
#include "PixyCameraException.h"

namespace pixy_cam
{
    FfmpegRunner::FfmpegRunner( PixyCamera& camera, const std::string& url ) :
        isRunning( false ),
        keepRunning( false ),
        threadErrorMessage( "" ),
        camera( camera ),
        url( url ),
        thread( nullptr ),
        startStopLock(),

        cameraFrame( nullptr ),
        swsContext( nullptr ),

        outputStream( nullptr ),
        outputFormatContext( nullptr ),
        outputCodecContext( nullptr ),
        outputStreamFrame( nullptr )
    {
        avformat_network_init();
    }

    FfmpegRunner::~FfmpegRunner()
    {
        StopLoop();

        if( this->swsContext != nullptr )
        {
            sws_freeContext( this->swsContext );
            this->swsContext = nullptr;
        }

        if( this->cameraFrame != nullptr )
        {
            av_frame_free( &this->cameraFrame );
            this->cameraFrame = nullptr;
        }

        if( this->outputStreamFrame != nullptr )
        {
            av_frame_free( &this->outputStreamFrame );
            this->outputStreamFrame = nullptr;
        }

        if( this->outputFormatContext != nullptr )
        {
            avformat_free_context( this->outputFormatContext );
            this->outputFormatContext = nullptr;
        }

        if( this->outputCodecContext != nullptr )
        {
            avcodec_free_context( &this->outputCodecContext );
            this->outputCodecContext = nullptr;
        }

        avformat_network_deinit();

        delete this->thread;
        this->thread = nullptr;
    }

    void FfmpegRunner::StartLoop()
    {
        std::lock_guard<std::mutex>( this->startStopLock );
        if( this->isRunning )
        {
            return;
        }
        this->thread = new std::thread( std::bind( &FfmpegRunner::ThreadEntry, this ) );

        this->keepRunning = true;
        this->isRunning = true;
    }

    bool FfmpegRunner::IsHealthy() const
    {
        return this->threadErrorMessage == "";
    }

    void FfmpegRunner::StopLoop()
    {
        std::lock_guard<std::mutex>( this->startStopLock );
        if( this->isRunning == false )
        {
            return;
        }

        this->keepRunning = false;
        this->thread->join();
        this->isRunning = false;
    }

    void FfmpegRunner::ThreadEntry()
    {
        try
        {
            CreateInputStream();
            CreateOutputStream();

            uint16_t actualWidth;
            uint16_t actualHeight;
            std::vector<uint8_t> rawBytes;
            while( this->keepRunning )
            {
                int return_value = this->camera.GetFrame(
                    0x21, // <- Unsure what this does, but only 0x21 works.
                    &actualWidth,
                    &actualHeight,
                    rawBytes
                );

                if( return_value < 0 )
                {
                    throw PixyCameraException( return_value );
                }

                // Copy bayer data to source frame.
                std::copy(
                    rawBytes.begin(),
                    rawBytes.end(),
                    this->cameraFrame->data[0]
                );
                this->cameraFrame->linesize[0] = this->camera.GetWidth();

                // Convert Bayer to YUV420P
                sws_scale(
                    this->swsContext,
                    this->cameraFrame->data,
                    this->cameraFrame->linesize,
                    0,
                    this->camera.GetHeight(),
                    this->outputStreamFrame->data,
                    this->outputStreamFrame->linesize
                );

                // Encode Frame
                return_value = avcodec_send_frame(
                    this->outputCodecContext,
                    this->outputStreamFrame
                );
                if( return_value < 0 )
                {
                    std::cerr << "Error sending frame for encoding: " + std::to_string( return_value ) << std::endl;
                    throw FfmpegException( return_value );
                }
            }
        }
        catch( const std::exception& e )
        {
            std::cerr << e.what() << std::endl;
            this->threadErrorMessage = e.what();
        }
    }

    void FfmpegRunner::CreateInputStream()
    {
        const AVCodec* codec  = avcodec_find_decoder( AV_CODEC_ID_RAWVIDEO );
        if( !codec )
        {
            throw FfmpegException( "Can not find raw video codec." );
        }

        // Setup Frame
        this->cameraFrame = av_frame_alloc();
        if( !this->cameraFrame )
        {
            throw FfmpegException( "Could not allocate camera frame." );
        }
        this->cameraFrame->format = AV_PIX_FMT_BAYER_BGGR8;
        this->cameraFrame->width = this->camera.GetWidth();
        this->cameraFrame->height = this->camera.GetHeight();
        if( av_frame_get_buffer( this->cameraFrame, 0 ) < 0 )
        {
            throw FfmpegException( "Could not allocate camera video frames." );
        }

        this->swsContext = sws_getContext(
            this->camera.GetWidth(),    // Source Width
            this->camera.GetHeight(),   // Source Height
            AV_PIX_FMT_BAYER_BGGR8,     // Source Format
            this->camera.GetWidth(),    // Destination Width
            this->camera.GetHeight(),   // Destination Height
            AV_PIX_FMT_YUV420P,         // Destination Format
            SWS_BILINEAR,               // Flags
            nullptr,                    // Source Filter
            nullptr,                    // Destination Filter
            nullptr                     // Extra Params
        );
        if( !this->swsContext )
        {
            throw FfmpegException( "Could not initialize sws context." );
        }
    }

    void FfmpegRunner::CreateOutputStream()
    {
        const AVCodec* codec = avcodec_find_decoder( AV_CODEC_ID_H264 );
        if( !codec )
        {
            throw FfmpegException( "Can not find H264 codec." );
        }
        int returnCode = avformat_alloc_output_context2( &this->outputFormatContext, nullptr, "rtp", this->url.c_str() );
        if( returnCode < 0 )
        {
            std::cerr << "Error allocating output format context" << std::endl;
            throw FfmpegException( returnCode );
        }

        this->outputStream = avformat_new_stream( this->outputFormatContext, codec );
        if( !this->outputStream )
        {
            throw FfmpegException( "Could not allocate output stream." );
        }

        this->outputCodecContext = avcodec_alloc_context3( codec );
        if( !this->outputCodecContext )
        {
            throw FfmpegException( "Could not allocate output codec context." );
        }

        // Set codec parameters
        this->outputCodecContext->codec_id = AV_CODEC_ID_H264;
        this->outputCodecContext->width = this->camera.GetWidth();
        this->outputCodecContext->height = this->camera.GetHeight();
        this->outputCodecContext->time_base = (AVRational){1, 25};
        this->outputCodecContext->framerate = (AVRational){25, 1};
        this->outputCodecContext->gop_size = 10;
        this->outputCodecContext->max_b_frames = 1;
        this->outputCodecContext->pix_fmt = AV_PIX_FMT_YUV420P;

        returnCode = avcodec_open2( this->outputCodecContext, codec, nullptr );
        if( returnCode < 0 )
        {
            std::cerr << "Could not open output codec" << std::endl;
            throw FfmpegException( returnCode );
        }

        // Setup Frame
        this->outputStreamFrame = av_frame_alloc();
        if( !this->outputStreamFrame )
        {
            throw FfmpegException( "Could not allocate output stream frame." );
        }
        this->outputStreamFrame->format = this->outputCodecContext->pix_fmt;
        this->outputStreamFrame->width = this->camera.GetWidth();
        this->outputStreamFrame->height = this->camera.GetHeight();
        returnCode = av_frame_get_buffer( this->outputStreamFrame, 0 );

        if( returnCode < 0 )
        {
            std::cerr << "Could not allocate output stream video frames." << std::endl;
            throw FfmpegException( returnCode );
        }
    }
}
