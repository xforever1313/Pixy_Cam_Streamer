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

#ifndef FFMPEG_PROCESS_RUNNER_H_
#define FFMPEG_PROCESS_RUNNER_H_

#include <mutex>
#include <thread>

#include "PixyCamera.h"
#include "PixyCamStreamConfig.h"

namespace pixy_cam
{
    class FfmpegProcessRunner
    {
        public:
            FfmpegProcessRunner( PixyCamera& camera, const PixyCamStreamConfig& config );
            ~FfmpegProcessRunner();

            void Init();
            void StartLoop();
            void StopLoop();

            std::string GetStatus() const;

        private:
            FfmpegProcessRunner() = delete;

            void StdOutThreadEntry();
            void StdErrThreadEntry();
            void StdInThreadEntry();
            void ReadLoop( int file, std::ostream& outFile );
            void SetStatus( const std::string& str );

            PixyCamera& camera;
            const PixyCamStreamConfig& config;
            mutable std::mutex startStopLock;

            pid_t ffmpegProcessId;
            int stdinFile;
            int stdoutFile;
            int stderrFile;

            std::thread* stdoutThread;
            std::thread* stderrThread;
            std::thread* stdinThread;
            std::mutex printLock;

            bool isRunning;
            volatile bool keepProcessing;
            
            std::string statusString;
            mutable std::mutex statusLock;
    };
}

#endif // FFMPEG_PROCESS_RUNNER_H_