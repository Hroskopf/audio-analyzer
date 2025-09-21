# AudioAnalyzer: users' documentation

## Welcome

This is a AudioAnalyzer. The program is created for playing a audio file and visualizing the plot of frequencies at each moment.

## How to install

Firstly, move to `src` directory:

```sh
cd src
```

The project uses [FFTW](https://www.fftw.org/) and [SFML](https://www.sfml-dev.org/) libraries, so before running the program, make sure to have them installed:

### Windows

For library installation, use this script:

```sh
install-deps.bat
```

After that, you need to create an executable by:

```sh
build.bat
```

After that you will have the `build\Release\audioanalyzer.exe` file and you can run it with a audio file path as an argument. For example:

```sh
build\Release\audioanalyzer.exe samples\music.mp3
```

### MacOS or Linux

For installation, you can run:

```sh
make install-libraries
```

It will install two needed libraries on MacOs (using `brew`), on Linux (using some package manager) and on Windows (using a `vcpkg`).

If you want to uninstall the libraries, run `make uninstall-libraries`.

When all the libraries are downloaded, you can create and execurable file. For this just run:

```sh
make
```

Then to run the program type `./audioanalyzer` with a path to file you want to run (for example you can run some of the sample audios):

```sh
./audioanalyzer samples/music.mp3
```

## How to use

The player works with most of common audio file formats (wav, mp3, etc.) but the audio file needs to be small enought to be fully uploaded to memory (up to ~5 min. songs would be fine).

Then you will see a window with the next components:

- graph area

- play button

- left and right rewind buttons

- progress bar

To start or stop playing the song, you can press the play button or Space bar on your keyboard. 

At each moments of time you will see the current graph of frequencies on the plot area. The y-value of the graph is measured in dBs with a range [-100, 0]. The x-scale corresponds to frequencoes in [0, 22000] HZ, which are plotted inlogarythmic scale.

You can rewind the audio using a progress bar. You can click on it move the dot where you want and then after releasing the mouse, the audio will start playing from the corresponding position.

You can also rewind audio by 5 seconds forward or backwards using a left/right ewind button. The sam result will be if clicking left/right arrows on your keyboard.