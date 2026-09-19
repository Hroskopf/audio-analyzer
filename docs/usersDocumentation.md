# AudioAnalyzer: user documentation

## Welcome

This is AudioAnalyzer. The program plays an audio file and visualizes a plot of its frequencies at each moment.

## How to install

First, move to the `src` directory:

```sh
cd src
```

The project uses the [FFTW](https://www.fftw.org/) and [SFML](https://www.sfml-dev.org/) libraries, so before running the program, make sure they are installed:

### Windows

To install the libraries, use this script:

```sh
install-deps.bat
```

After that, create the executable with:

```sh
build.bat
```

This produces `build\Release\audioanalyzer.exe`, which you can run with an audio file path as an argument. For example:

```sh
build\Release\audioanalyzer.exe samples\music.mp3
```

### macOS or Linux

To install the libraries, run:

```sh
make install-libraries
```

This installs the two required libraries — on macOS using `brew`, on Linux using your package manager (`apt`, `dnf`, or `pacman`).

If you want to uninstall the libraries later, run `make uninstall-libraries`.

Once the libraries are installed, create the executable by running:

```sh
make
```

Then run `./audioanalyzer` with the path to the file you want to play (for example, one of the sample audio files):

```sh
./audioanalyzer samples/music.mp3
```

## How to use

The player works with the most common audio file formats (WAV, MP3, etc.), but the audio file needs to be small enough to fit fully in memory (songs up to ~5 minutes are fine).

You will see a window with the following components:

- graph area

- play button

- left and right rewind buttons

- progress bar

To start or stop the song, press the play button or the Space bar on your keyboard.

At each moment you will see the current frequency graph in the plot area. The y-axis is measured in dB with a range of [-100, 0]. The x-axis corresponds to frequencies in [0, 22000] Hz, plotted on a logarithmic scale.

You can seek through the audio using the progress bar: click it, drag the dot where you want, and when you release the mouse the audio will continue playing from the corresponding position.

You can also rewind the audio 5 seconds forward or backward using the left/right rewind buttons, or by pressing the left/right arrow keys on your keyboard.
