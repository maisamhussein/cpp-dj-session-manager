#include "WAVTrack.h"
#include <iostream>

WAVTrack::WAVTrack(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int sample_rate, int bit_depth)
    : AudioTrack(title, artists, duration, bpm), sample_rate(sample_rate), bit_depth(bit_depth) {

    std::cout << "WAVTrack created: " << sample_rate << "Hz/" << bit_depth << "bit" << std::endl;
}

void WAVTrack::load() {
    std::cout << "[WAVTrack::load] Loading WAV: \"" << title<< "\" at " << sample_rate << "Hz/"<< bit_depth << "bit (uncompressed)...\n";
    long long size = duration_seconds * sample_rate * (bit_depth / 8) * 2;
    std::cout <<"  → Estimated file size: " << size << " bytes\n"; 
    std::cout <<"  → Fast loading due to uncompressed format.\n";
}

void WAVTrack::analyze_beatgrid() {
    std::cout << "[WAVTrack::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    int beats_estimated = (duration_seconds / 60.0)* bpm;
    std::cout << "  → Estimated beats: " << beats_estimated << "  → Precision factor: 1 (uncompressed audio)\n";
}

double WAVTrack::get_quality_score() const {
    double quality_score = 70;
    if(sample_rate >= 44100) {quality_score = quality_score + 10;}
    if(sample_rate >= 96000) {quality_score = quality_score + 5;}
    if(bit_depth >= 16) {quality_score = quality_score + 10;}
    if(bit_depth >= 24) {quality_score = quality_score + 5;}
    if(quality_score > 100){quality_score = 100;}
    std::cout << "[WAVTrack::get_quality_score] \""<< get_title()<< "\" score = " << quality_score << "/100"<< std::endl;
    return quality_score; // Replace with your implementation
}

PointerWrapper<AudioTrack> WAVTrack::clone() const {
    return PointerWrapper<AudioTrack>(new WAVTrack(*this)); 
}