extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

#include "FfmpegRunner.h"
#include "FfmpegException.h"

namespace pixy_cam
{
    FfmpegRunner::FfmpegRunner( PixyCamera& camera ) :
        camera( camera ),
        outputStream( nullptr ),
        outputFormatContext( nullptr ),
        outputCodecContext( nullptr )
    {
        avformat_network_init();
    }

    FfmpegRunner::~FfmpegRunner()
    {
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
    }

    void FfmpegRunner::StartLoop()
    {
        AVCodec* codec  = avcodec_find_decoder( AV_CODEC_ID_RAWVIDEO );
        if( !codec )
        {
            throw FfmpegException( "Can not find raw video codec" );
        }

        CreateOutputStream();
    }

    void FfmpegRunner::CreateOutputStream()
    {
        AVCodec* codec = avcodec_find_decoder( AV_CODEC_ID_H264 );
        if( !codec )
        {
            throw FfmpegException( "Can not find H264 codec." );
        }
        avformat_alloc_output_context2( &this->outputFormatContext, nullptr, "rtp", "rtp://127.0.0.1:5004" );
        this->outputStream = avformat_new_stream( this->outputFormatContext, codec );
        if( !this->outputStream )
        {
            throw FfmpegException( "Could not allocate output stream." );
        }

        this->outputCodecContext = avcodec_alloc_context3( codec );
        if( !this->outputCodecContext )
        {
            throw FfmpegException( "Could not allocate output codec context" );
        }

        // Set codec parameters
        this->outputCodecContext->codec_id = AV_CODEC_ID_H264;
        this->outputCodecContext->width = this->camera.GetWidth();
        this->outputCodecContext->height = this->camera.GetHeight();
        this->outputCodecContext->time_base = (AVRational){1, 25};
        this->outputCodecContext->pix_fmt = AV_PIX_FMT_YUV420P;

        if( avcodec_open2( this->outputCodecContext, codec, nullptr ) < 0 )
        {
            throw FfmpegException( "Could not open output codec" );
        }
    }
}