#ifndef FREQUENCYANALYZER
#define FREQUENCYANALYZER

#include <cmath>
#include <vector>
#include <complex>
#include <algorithm>
#include <fftw3.h>

constexpr double PI = 3.14159265358979323846;

// class for spectral sound analysis. It gets audio samples and computes a frequency spectrum at each given moment.
class FrequencyAnalyzer { 
    
private:

    const int block_size = 2048; // whole audio data is divided into chunks of given size
    const int fft_size = 8192; // each block then is padded to some bigger size before running fft
    const int block_num; // number of blocks in whole audio
    const int sampleRate; // number of samples on each second of a sound
    const int binsNum; // number of output frequency bins 
    const int numberOfChannels; // number of channels of audio data

    const int minFreq = 0, maxFreq; // range of frequencies of the audio
    
    const std::vector<int>input_data; // array of input samples

    std::vector<std::vector<double>>frequencies_dB; // output array. for each block -> frequency magnitudes of the audio divided into binsNum bins

    // converts interleaved multi-channel samples (L,R,L,R,...) into mono by averaging each frame.
    // Analyzing the interleaved stream directly would mirror low-frequency energy up near Nyquist.
    static std::vector<int> downmix_to_mono(const std::vector<int>& data, int channels) {
        if(channels <= 1) return data;
        std::vector<int> mono(data.size() / channels);
        for(size_t i = 0;i < mono.size();i++) {
            long long sum = 0;
            for(int c = 0;c < channels;c++) {
                sum += data[i * channels + c];
            }
            mono[i] = (int)(sum / channels);
        }
        return mono;
    }

    // splits input data into blocks of given size
    std::vector<std::vector<int>>split_to_blocks(const std::vector<int>&data, const int block_size_) const {

        std::vector<std::vector<int>>blocks(((int)data.size() + block_size_ - 1) / block_size_);
        for(size_t i = 0;i < data.size();i++) {
            blocks[i / block_size_].push_back(data[i]);
        }   
        return blocks;
    }

    // prepares one block for applying FFT. Changes its size to given size, normalizes it and applies a Hann window function (for smoothering data)
    std::vector<double> prepare_data(const std::vector<int>& data, size_t size) const {

        double max_data_value = 32768; // for normalization
        std::vector<double>result(size);
        for(size_t i = 0;i < data.size();i++) {
            double window = 0.5 * (1 - cos(2 * PI * i / data.size())); // window spans the real data, not the zero padding
            result[i] = ((double)data[i] / max_data_value) * window;
        }
        return result;
    }

    // provides a DFT algorithm on given block. Returns a array of complex numbers.
    std::vector<std::complex<double>> get_fft(const std::vector<double>& in) const {

        double* fft_input = fftw_alloc_real(in.size());
        fftw_complex* fft_output = fftw_alloc_complex(in.size() / 2 + 1);
        fftw_plan plan = fftw_plan_dft_r2c_1d(in.size(),fft_input, fft_output, FFTW_MEASURE);
        for(size_t i = 0;i < in.size();i++) {
            fft_input[i] = in[i];
        }
        fftw_execute(plan);
        std::vector<std::complex<double>>result(in.size() / 2 + 1);
        for(size_t i = 0;i < in.size() / 2 + 1;i++) {
            result[i] = {fft_output[i][0], fft_output[i][1]};
        }
        return result;
    }

    // gets an output array from a DFT transform. Counts the magnitudes and converts it to dBs
    // `in` is already the half-spectrum of a real FFT of length 2 * (in.size() - 1)
    std::vector<double> to_dB(const std::vector<std::complex<double>>& in) const {
        const double n = 2.0 * (in.size() - 1); // original FFT size
        std::vector<double> result(in.size());
        for(size_t i = 0;i < result.size();i++) {
            double magnitude = std::abs(in[i]);
            result[i] = 20 * log10(std::max(magnitude * 4.0 / n, 1e-10));

        }
        return result;
    }

    // applies an logarythmic scaling on x axis. Divides data into given number of bins
    std::vector<double> to_logarythmic_scale(const std::vector<double>& input_freqs_values, const std::vector<double>& input_freqs, int number_of_bins, double base) const {

        std::vector<double> output_freqs(number_of_bins);
        std::vector<double>frequencies(number_of_bins);
        
        frequencies[0] = 0;
        double bn = pow(base, number_of_bins);
        double d1 = maxFreq * (base - 1) / (bn - 1);
        for(int i = 1;i < number_of_bins;i++) {
            frequencies[i] = frequencies[i - 1] + d1;
            d1 *= base;
        }

        // for each bins, counts an average value of all the frequencies that are in this bin
        int current_freq = 0;
        std::vector<int>cnts(number_of_bins);
        std::vector<double>sums(number_of_bins);
        for(int j = 0;j < input_freqs_values.size();j++) {
            while(current_freq + 1 < number_of_bins && input_freqs[j] > frequencies[current_freq + 1]) {
                current_freq++;
            }
            cnts[current_freq]++;
            sums[current_freq] += input_freqs_values[j];
        }

        for(int j = 0;j < number_of_bins;j++) {
            if(cnts[j] == 0) {
                output_freqs[j] = output_freqs[j - 1];
            }
            else {
                output_freqs[j] = sums[j] / (double)cnts[j];
            }
        }
        return output_freqs;
        
    }

    // returns an array of frequencies (x-axis) for given size of FFT and sampleRate
    std::vector<double> get_frequencies(const int N, const double sampleRate) const {
        std::vector<double>result(N / 2 + 1);
        for(int i = 0;i < N / 2 + 1;i++) {
            result[i] = sampleRate * i / N;
        }
        return result;
    }


public:

    // data_ is an array of samples of whole audio. sampleRate is number of samples per each second. binsNum_ is a needed number of bins of output frequencies 
    FrequencyAnalyzer(const std::vector<int>& data_, int sampleRate_, int binsNum_, int numberOfChannels_):
        input_data(downmix_to_mono(data_, numberOfChannels_)), sampleRate(sampleRate_), binsNum(binsNum_), block_num(((data_.size() / std::max(numberOfChannels_, 1) + block_size - 1) / block_size)), maxFreq(sampleRate_ / 2), numberOfChannels(numberOfChannels_) {
            
        auto blocks = split_to_blocks(input_data, block_size);

        frequencies_dB.resize(block_num);

        auto frequencies = get_frequencies(fft_size, sampleRate); // magnitudes come from the padded FFT, so the axis must too

        for(int i = 0;i < block_num;i++) {
            auto block = prepare_data(blocks[i], fft_size);
            auto fft_res = get_fft(block);
            auto in_dB = to_dB(fft_res);
            frequencies_dB[i] = to_logarythmic_scale(in_dB, frequencies, binsNum, 1.1);
        }

    }

    // given a millisecond of a sound, returns an array of numBins elements -> dBs of each frequency.
    std::vector<double> get_frequency_magnitudes(long long millisecond) {
        // the data is mono after downmixing, so the block index no longer depends on the channel count
        long long idx = millisecond * sampleRate / (1000 * block_size);
        idx = std::min(idx, (long long)block_num - 1); // the last millisecond maps just past the final block
        return frequencies_dB[std::max(idx, 0LL)];
    }

};


#endif