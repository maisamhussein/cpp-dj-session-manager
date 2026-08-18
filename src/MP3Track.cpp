#include "MP3Track.h"
#include <iostream>
#include <cmath>
#include <algorithm>

MP3Track::MP3Track(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int bitrate, bool has_tags)
    : AudioTrack(title, artists, duration, bpm), bitrate(bitrate), has_id3_tags(has_tags) {

    std::cout << "MP3Track created: " << bitrate << " kbps" << std::endl;
}

void MP3Track::load() {
    std::cout << "[MP3Track::load] Loading MP3: \"" << title
              << "\" at " << bitrate << " kbps...\n";

    if(has_id3_tags){
        std::cout <<"  → Processing ID3 metadata (artist info, album art, etc.)...\n";
    }
    else
        std::cout <<"  → NO ID3 tags found\n";
    std::cout <<"  → Decoding MP3 frames...\n";
    std::cout <<"  → Load complete.\n";
}

void MP3Track::analyze_beatgrid() {
     std::cout << "[MP3Track::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    int estimated_beats = static_cast<int>((duration_seconds / 60.0) * bpm);
    double compression_factor = static_cast<double>(bitrate) / 320.0;
    std::cout << "  → Estimated beats: " << estimated_beats<< "  → Compression precision factor: " << compression_factor << "\n";
}
double MP3Track::get_quality_score() const {
    double quality_score = (bitrate / 320.0) * 100.0;
    if(has_id3_tags){ quality_score = quality_score + 5;}
    if(bitrate < 128){ quality_score = quality_score - 10;}
    if(quality_score < 0.0) { quality_score = 0;}
    if(quality_score > 100.0) { quality_score = 100;}
     std::cout << "[MP3Track::get_quality_score] \""  << title << "\" score = " << quality_score << "/100" << std::endl;
    return quality_score; 
}

PointerWrapper<AudioTrack> MP3Track::clone() const {
    return PointerWrapper<AudioTrack>(new MP3Track(*this)); 

}