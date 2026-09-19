# AudioAnalyzer: programmer documentation

## Overview

This is a program for playing audio and showing a plot of the spectral analysis of the played sound. When it runs, you see a GUI with a plot area for the frequencies, a progress bar, and buttons for basic interaction with the played audio.

## Libraries and installation

The project uses two external libraries: [SFML](https://www.sfml-dev.org/) (>= 3.0.0) for the GUI and audio handling, and [FFTW](https://www.fftw.org/) for the frequency (DFT) analysis.

- On macOS and Linux, the libraries are installed via the system package manager (`brew` on macOS; `apt`, `dnf`, or `pacman` on Linux). To install them, run `make install-libraries`, then create the executable with `make`.

- On Windows, the libraries are installed with `vcpkg` by running `install-deps.bat`. The executable is then created using CMake via the `build.bat` script.

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
├── resources
│   └── fonts
│       ├── DejaVuSans.ttf
│       └── FONT-LICENSE
└── samples
    ├── 440+880HZ.mp3
    ├── 440HZ.mp3
    ├── 880HZ.mp3
    ├── DrumKick.mp3
    ├── music.mp3
    └── PureScale.mp3
```

The `components` directory holds all the header files the program needs. `main.cpp` is the entry point. The `resources` folder contains the font used for on-screen text. The `samples` directory contains sample audio files you can play to see the program in action.

## How it works

Everything the program does can be divided into three parts:

- spectral analysis of the audio file

- a GUI for showing everything in the window

- making the first two parts work together

## Spectral analysis

This is done by the [FrequencyAnalyzer](../src/components/frequencyAnalyzer.hpp) class, using the `FFTW` library.

All the heavy computation (the FFT itself) happens once, when the class is constructed. After that, whenever the spectral data for some moment is needed, it is simply returned from memory.

To analyze the data, we take the following steps:

- Divide the samples into blocks of size 2048.

- Prepare each block for the FFT. In this step the block is expanded to size 8192, its values are normalized, and the Hann window function is applied (for smoother transitions between blocks).

- Run the real-to-complex DFT on each block.

- Convert the frequency magnitudes to dB so they can be shown on screen.

- Finally, group the frequencies into `binsNum` bins whose widths grow exponentially (because most of the useful spectral information is in the lower range). For each bin, the average dB value of its frequencies is computed.

## GUI

The GUI is written using the `SFML 3.0` library.

The main components shown in the window are: a play button, left and right rewind buttons, a scroll bar, and a graph area for the frequency plot. Each component lives in its own header file in the [GUIcomponents](../src/components/GUIcomponents/) folder.

Everything is put together by the [WindowController](../src/components/windowController.hpp). It is responsible for creating the main window, drawing every GUI component with its current values, and processing window events.

For the events we care about, there is an `EventType` enum. When such an event occurs, it is returned by the `getEvent` function and handled in the main file.

## Putting logic and GUI together

This happens in [main.cpp](../src/main.cpp).

First it loads the input audio file and passes it to the `FrequencyAnalyzer`.

Everything else happens in the main loop. Each iteration polls events from the controller and processes them, updates the state of the GUI components, and draws everything in the window (in that order). The loop runs until the window is closed.

## Audio samples

Some audio samples can be found in the [samples](../src/samples/) folder:

- [440HZ.mp3](../src/samples/440HZ.mp3) is a pure sine wave of the A4 note. You should see a single peak on the plot.

- [880HZ.mp3](../src/samples/880HZ.mp3) is an A5 note.

- [440+880HZ.mp3](../src/samples/440+880HZ.mp3) is the two notes played together (there should be two peaks on the plot).

- [DrumKick.mp3](../src/samples/DrumKick.mp3) is a bass drum sound played repeatedly.

- [PureScale.mp3](../src/samples/PureScale.mp3) is a C-major scale played with pure sine waves.

- [music.mp3](../src/samples/music.mp3) is just a sample of a song.
