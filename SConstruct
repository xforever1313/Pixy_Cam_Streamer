##
## Pixy Cam Streamer - Stream a Pixy Camera to a RTMP server
## Copyright (C) 2024 Seth Hendrick
## 
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU Affero General Public License as published
## by the Free Software Foundation, either version 3 of the License, or
## any later version.
## 
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Affero General Public License for more details.
## 
## You should have received a copy of the GNU Affero General Public License
## along with this program.  If not, see <https://www.gnu.org/licenses/>.
##

import glob
import SCons.Defaults
import SCons
import os

from multiprocessing import cpu_count

SetOption('num_jobs', cpu_count())

baseDir = os.path.abspath( "." )
installDir = os.path.join( baseDir, "install" )
debDir = os.path.join( installDir, "deb" )
libPixyFolder = os.path.join( baseDir, "libpixy" )
pixyCheckout = os.path.join( libPixyFolder, "pixy" )

version = "1.0.0"

defines = ["VERSION=" + '"\\"' + version + '\\""']
compileFlags = ["-Wall", "-Wextra", "-std=c++17", "-Wno-unused-parameter"]
includePaths = [
    "include",
    os.path.join( pixyCheckout, "src", "common", "inc" ),
    os.path.join( pixyCheckout, "src", "host", "libpixyusb" ),
    "/usr/include/libusb-1.0",
    "/usr/include/x86_64-linux-gnu",
]
libs = [
    "pixyusb",
    "boost_thread",
    "boost_chrono",
    "boost_system",
    "usb-1.0",
    "PocoNet",
    "PocoFoundation",
    "PocoUtil",
    "pthread"
]
sourceFiles = glob.glob( os.path.join( "src", "*.cpp" ) )

exe = os.path.join( baseDir, "bin", "pixy_cam" )
fakeExe = os.path.join( baseDir, "bin", "fake_pixy_cam" )

baseEnv = Environment(
    BASE_DIR = baseDir,
    INSTALL_DIR = installDir,
    DEBIAN_INSTALL_DIR = debDir,
    SOFTWARE_VERSION = version
)

libPixyEnv = baseEnv.Clone(
    LIBPIXY_DIR=libPixyFolder,
    PIXY_CHECKOUT=pixyCheckout
)

buildEnv = baseEnv.Clone(
    tools=["default", "gcc", "g++"],
    CPPDEFINES=defines,
    CPPPATH = includePaths,
    CXXFLAGS = compileFlags,
    LIBS = libs,
    BINDIR = os.path.join( baseDir, "bin" ),
    LIBPATH = os.path.join( libPixyEnv["LIBPIXY_DIR"], "bin" )
)

installEnv = baseEnv.Clone(
    PACKAGE_NAME = "pixy-cam-streamer",
    MAINTAINER = "Seth Hendrick",
    DESCRIPTION = "Steams output from a Pixy Camera to a RTMP URL",
    HOMEPAGE = "https://shendrick.net"
)

realEnv = buildEnv.Clone(
    OBJPREFIX = os.path.join( baseDir, "obj", "real" ) + "/",
)
realEnv.Append( 
    CXXFLAGS=["-O2"]
)

fakeEnv = buildEnv.Clone(
    OBJPREFIX = os.path.join( baseDir, "obj", "fake" ) + "/",
)
fakeEnv.Append(
    CPPDEFINES = ["FAKE_CAMERA"],
    CXXFLAGS = ["-g", "-O0"]
)

exeTarget = realEnv.Program( target=exe, source=sourceFiles )
Clean( exeTarget, [os.path.join(realEnv['BINDIR']), os.path.join(realEnv['OBJPREFIX'])])

fakeTarget = fakeEnv.Program( target=fakeExe, source=sourceFiles )
Clean( fakeTarget, [os.path.join(fakeEnv['BINDIR']), os.path.join(fakeEnv['OBJPREFIX'])])

debianEnv = installEnv.Clone(
    EXE_TARGET = exeTarget
)

SConscript( [os.path.join( debDir, "SConscript" ) ], exports='debianEnv' )
libTarget = SConscript( [os.path.join( libPixyEnv["LIBPIXY_DIR"], "SConscript" ) ], exports='libPixyEnv' )

Alias( "build_lib_pixy", libTarget )
Depends( exeTarget, libTarget )
Depends( fakeTarget, libTarget )

Alias( "build", exeTarget )
Alias( "build_fake", fakeTarget )
Default( "build" )
