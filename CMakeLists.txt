cmake_minimum_required(VERSION 3.12)
project(granular_synthesizer C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(granular_synthesizer
        Adc.c
        Adc.h
        Dac.c
        Dac.h
        DisplayDraw.c
        DisplayDraw.h
        DisplayWrite.c
        DisplayWrite.h
        FilterCoeffs.h
        FixedPoint.c
        FixedPoint.h
        heap.c
        heap.h
        KeyboardInterface.c
        KeyboardInterface.h
        Lab11Main.c
        LcdInterface.c
        LcdInterface.h
        Logging.c
        Logging.h
        MicInterface.c
        MicInterface.h
        Music.c
        Music.h
        PLL.c
        PLL.h
        PortA.c
        PortA.h
        PortB.c
        PortB.h
        PortC.c
        PortC.h
        PortD.c
        PortD.h
        PortE.c
        PortE.h
        PortF.c
        PortF.h
        PortUtils.c
        PortUtils.h
        PotInterface.c
        PotInterface.h
        SoundArrays.c
        SoundArrays.h
        ST7735.c
        ST7735.h
        startup.c
        startup.h
        Switches.c
        Switches.h
        SynchSerialInterface.c
        SynchSerialInterface.h
        TimerA.c
        TimerA.h
        Utils.c
        Utils.h)
