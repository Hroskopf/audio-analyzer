#include <SFML/Audio.hpp>
#include "components/windowController.hpp"
#include "components/frequencyAnalyzer.hpp"
#include <iostream>


int main(int argc, char* argv[]) {

    // get path to audio file from a argument line
    std::vector<std::string> args(argv, argv + argc);
    if(args.size() == 1) {
        std::cerr << "Error: Path to audio isn't provided" << std::endl;
        exit(0);
    }   
    auto path_to_audio = argv[1];

    // load sound buffer
    sf::SoundBuffer buffer;
    if(!buffer.loadFromFile(path_to_audio)) {  
        std::cerr << "Error: Invalid file path or unsupported file extension" << std::endl;
        exit(0);
    }

    // load audio from buffer
    sf::Sound audio(buffer);

    // get audio samples
    auto samples = buffer.getSamples();
    std::vector<int>audioData(samples, samples + buffer.getSampleCount());

    // create an spectral fft analyzer
    FrequencyAnalyzer analyzer(audioData, buffer.getSampleRate(), 66, buffer.getChannelCount());

    //create a controller for all the components that are shown on the window 
    WindowController window;

    const auto audioDuration = buffer.getDuration().asMilliseconds();
    while (window.isOpen())
    {
        // proccessing of all the events in current time moment
        WindowController::EventType event = window.getEvent(); 
        while(event != WindowController::None) {

            //change of the status of song (play to stop or vice versa)
            if(event == WindowController::PlayButtonClicked || event == WindowController::SpaceClicked) { 
                if(audio.getStatus() == sf::SoundSource::Status::Playing) {
                    audio.pause();
                }
                else {
                    audio.play();
                }
            }

            // rewind the audio by five seconds backwards
            if(event == WindowController::LeftArrowClicked || event == WindowController::LeftRewindButtonClicked) {
                audio.setPlayingOffset(sf::Time(max(std::chrono::milliseconds(0), std::chrono::milliseconds(audio.getPlayingOffset().asMilliseconds() - 5000))));
            }

            // rewind the audio by five seconds forward
            if(event == WindowController::RightArrowClicked || event == WindowController::RightRewindButtonClicked) {
                audio.setPlayingOffset(sf::Time(min(std::chrono::milliseconds(audioDuration), std::chrono::milliseconds(audio.getPlayingOffset().asMilliseconds() + 5000))));
            }
            
            // change the offset of the audio from the scroll bar update.
            if(event == WindowController::ScrollBarChange) {
                auto bar_position = window.getBarPosition();
                audio.setPlayingOffset(sf::Time(std::chrono::milliseconds((int)(bar_position * audioDuration / 100))));
            }

            // update the current event
            event = window.getEvent();
        }

        // update the scroll bar with the current audio offset
        int currentOffset = audio.getPlayingOffset().asMilliseconds();
        window.setBarPosition(currentOffset * 100 / audioDuration);
        
        // update the playing button with current audio status
        window.setPlayingButtonState(audio.getStatus() == sf::SoundSource::Status::Playing);

        // get current frequency magnitudes from an analyzer
        auto frequencyMagnitudes = analyzer.get_frequency_magnitudes(currentOffset);

        // display everything on the screen
        window.drawAndDisplay(frequencyMagnitudes);
    }
}