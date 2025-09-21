# AudioAnalyzer: programmers' documentation

## Overview

This is programm for playing audio and showing a plot of spectral analysis of played sound. When running, you will see a GUI with a plot area of frequencies, a progress bar and buttons for basic interaction with played audio.

## Libraries and installation

The project uses two external lybraries: [SFML](https://www.sfml-dev.org/) (>= 3.0.0) for a GUI and working with audio and [FFTW](https://www.fftw.org/) for a frequency DFT analysis.

- On MacOS and Linux, the libraries are installed via some built-in package manager (`brew` on MacOS; `apt`, `dnf` or `pacman` on Linux). If you want to install libraries, do `make install-libraries` and then create an executable with `make` command.

- On Winsows, the libraries are installed with `vcpkg`. To do this, `install-deps.bat` should be run. After this the executable file is created using a CMake. This is done by `build.bat` script.

## Project structure

```sh
src
├── components 
│   ├── frequencyAnalyzer.hpp
│   ├── GUIcomponents
│   │   ├── all.hpp
│   │   ├── button.hpp
│   │   ├── graphBox.hpp
│   │   ├── playButton.hpp
│   │   ├── rewindButton.hpp
│   │   └── scrollBar.hpp
│   └── windowController.hpp
├── main.cpp
├── Makefile
├── README.md
├── resources
│   └── fonts
│       └── Calibri.ttf
└── samples
    ├── 440+880HZ.mp3
    ├── 440HZ.mp3
    ├── 880HZ.mp3
    ├── DrumKick.mp3
    ├── music.mp3
    └── PureScale.mp3
```

The `components` directory is for all of header files that are needed for the program. `main.cpp` file is the key file that is executed. In `resources` folder there are a folder with a font that is written on screen. `samples` directory contains some sample audio files that can be run to see the program working. 

## How does it work

Everything that is doen can be divided into three parts:

- spectral analysis of the audio file

- GUI for showing everything on the window

- make first two parts work together

## Spectral analysis

It is done by [FrequencyAnalyzer](../src/components/frequencyAnalyzer.hpp) class. This part uses the `FFTW` library.

All the heavy computations (FFT algorithm) is done once, when the class is created. After that when we need the spectral data at some moment, we just return it from memory.

For analaing data, we do next steps:

- Divide samples into blocks of size 2048.

- Prepare each block for the FFT. In this step we expand the block to size 8192, normalize values and apply the Hann windowing function (for smoother transitions between blocks)

- Then we do the real-to-complex DFT on each block

- After that we convert frequency magnitudes into dB, so that it can be shown on screen.

- Finally, we need to get `binsNum` bins, s.t. number of frequencies on each bin decrease exponentally (because all the useful spectral data is in the lower range). For each bin we count the average dB value of frequencies.

## GUI

GUI is written using a `SFML 3.0` library. 

The main components shown on the window are: play button, left and right rewind buttons, scroll bar and graph area for a frequency plot. Each such component is created in corresponding header file in [GUIcomponents](../src/components/GUIcomponents/) folder.

Everything is put together by [WindowController](../src/components/windowController.hpp). It is responsible for creating the main window, draw every gui component with corresponding values and proccess window events.

For 'interesting' events, the `EventType` enum created. When some such event occurs it is returned by `getEvent` function and needs to be proccesed in main file.

## Putting logic and GUI together

It is done by [main.cpp](../src/main.cpp) file.

Firstly it uploads the input audio file, and pass it to the `FrequencyAnalyzer`. 

Then everything is done in main loop. At each iteration it needs to poll the events from the controller and procces them, update the state of gui components and draw everything on the window (in this order). This loop will be working untill the the window is closed.

## Audio samples

Some audio samples can be found in [samples](../src/samples/) folder:

- [440HZ.mp3](../src/samples/440HZ.mp3) is a pure sine wave of the A4 note. You should see a single peak on the plot.

- [880HZ.mp3](../src/samples/880HZ.mp3) is an A5 note.

- [440+880HZ.mp3](../src/samples/440+880HZ.mp3) is two notes played together (there should be two peaks on the plot)

- [DrumKick.mp3](../src/samples/DrumKick.mp3) is a sound bass drum played repeteadly.

- [PureScale.mp3](../src/samples/PureScale.mp3) is a C-major scale played with pure sine waves.

- [music.mp3](../src/samples/music.mp3) is just a sample of some song.