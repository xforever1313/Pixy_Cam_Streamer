import glob
import SCons.Defaults
import SCons
import os

from multiprocessing import cpu_count

SetOption('num_jobs', cpu_count())

baseDir = os.path.abspath( "." )
installDir = os.path.join( baseDir, "install" )
debDir = os.path.join( installDir, "deb" )

version = "1.0.0"

defines = ["VERSION=" + '"\\"' + version + '\\""']
compileFlags = ["-Wall", "-Wextra", "-std=c++17", "-Wno-unused-parameter"]
includePaths = ["include", "/usr/include/libusb-1.0", "/usr/include/x86_64-linux-gnu"]
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

buildEnv = baseEnv.Clone(
    tools= ["default", "gcc", "g++"],
    CPPDEFINES=defines,
    CPPPATH = includePaths,
    CXXFLAGS = compileFlags,
    LIBS = libs,
    BINDIR = os.path.join( baseDir, "bin" )
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

Alias( "build", exeTarget )
Alias( "build_fake", fakeTarget )
Default( "build" )
