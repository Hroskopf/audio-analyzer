# AudioAnalyzer

A C++ desktop app that plays an audio file and renders its **live frequency spectrum** in sync with playback — an FFT-based visualizer built with [SFML 3.0](https://www.sfml-dev.org/) and [FFTW](https://www.fftw.org/).

Point it at a song and you get a real-time dB plot of the spectrum (0–22 kHz, log-scaled), with playback controls: play/pause, ±5 s rewind, and a draggable progress bar for seeking.

## How it works

The DSP pipeline (see [`frequencyAnalyzer.hpp`](src/components/frequencyAnalyzer.hpp)) runs once, up front:

1. The audio samples are split into **2048-sample blocks**.
2. Each block is normalized, multiplied by a **Hann window** (to reduce spectral leakage), and zero-padded to **8192 samples** for finer frequency resolution.
3. FFTW computes a **real-to-complex DFT** per block.
4. Magnitudes are converted to **dB** (clamped to [-100, 0]).
5. Frequencies are grouped into **66 exponentially sized bins**, so the musically dense low range gets most of the horizontal resolution.

Because every FFT is precomputed at load, playback, seeking, and scrubbing stay real-time — rendering a frame is just a lookup of the current block's spectrum.

The GUI is hand-built on SFML 3.0: each widget (play button, rewind buttons, scroll bar, graph area) is its own component under [`GUIcomponents/`](src/components/GUIcomponents/), composed by a [`WindowController`](src/components/windowController.hpp) that owns the window and translates raw SFML events into app events; [`main.cpp`](src/main.cpp) wires the controller to the audio playback and the analyzer.

## Build and run

Requires a C++17 compiler and the SFML (≥ 3.0) and FFTW libraries.

### macOS / Linux

```sh
cd src
make install-libraries   # brew on macOS; apt/dnf/pacman on Linux
make
./audioanalyzer samples/music.mp3
```

### Windows

```bat
cd src
install-deps.bat         :: installs SFML + FFTW via vcpkg
build.bat                :: builds with CMake
build\Release\audioanalyzer.exe samples\music.mp3
```

Any audio format SFML can decode works (WAV, MP3, OGG, FLAC). The whole file is loaded and analyzed up front, so keep it to roughly song length (~5 min).

## Controls

| Action | Mouse | Keyboard |
| --- | --- | --- |
| Play / pause | play button | Space |
| Rewind ±5 s | rewind buttons | ← / → |
| Seek | drag the progress bar | — |

## Try the samples

[`src/samples/`](src/samples/) contains test files with known spectra — a pure 440 Hz sine (single peak at A4), 440 + 880 Hz (two peaks), a repeated drum kick, a pure-sine C-major scale, and a regular song.

## Documentation

- [User documentation](docs/usersDocumentation.md) — installation and usage in detail
- [Programmer documentation](docs/programmersDocumentation.md) — architecture and the analysis pipeline

## Tech stack

C++17 · SFML 3.0 (audio + GUI) · FFTW 3 · Make (macOS/Linux) · CMake + vcpkg (Windows)
