//
//  fhn_rk4.cpp
//  rk4
//
//  Created by Jeremy Bai on 17/07/2023.
//

#include <iostream>
#include "SFML/Audio.hpp"
#include "fhn_rk4.hpp"
#include <cmath>

double square(double t, double frequency, double duty_cycle) {
    double period = 1.0 / frequency;
    double t_mod = fmod(t, period);
    return (t_mod < duty_cycle * period) ? 1.0 : -1.0;
}

double sawtooth(double t, double frequency) {
    double period = 1.0 / frequency;
    double t_mod = fmod(t, period);
    return (2.0 * t_mod / period) - 1.0;
}

int main() {
    double samplerate = 44100.0;
    double input_frequency = 220;
    double t_end = 10.0;
    double k = 20000.0;
    int total_samples = static_cast<int>(t_end * samplerate);

    FhnSolver solver(samplerate);
    solver.setTemporalScale(k);

    // This vector will hold the output samples
    std::vector<sf::Int16> output(total_samples);
    
    // Solve the model for each sample
    for (int i = 0; i < total_samples; ++i) {
        const double t_input = i / samplerate;
        const double input = square(t_input, input_frequency, 0.01);
        solver.processSystem(input);  // Assuming an input of 1
        output[i] = static_cast<sf::Int16>(solver.getCurrentState() * 32767.5 - 0.5);  // Scale to the range of sf::Int16
    }

    // Create an sf::SoundBuffer to hold the audio data
    sf::SoundBuffer buffer;
    if (!buffer.loadFromSamples(&output[0], output.size(), 1, samplerate)) {
        std::cerr << "Loading samples into SoundBuffer failed!" << std::endl;
        return 1;
    }

    // Save the buffer to a .wav file
    if (!buffer.saveToFile("output.wav")) {
        std::cerr << "Saving SoundBuffer to file failed!" << std::endl;
        return 1;
    }

    return 0;
}
