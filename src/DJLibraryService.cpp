#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist) ,library() {}

// the library is resposple to delet the tracks -> destructor -> rule of 3 (we put thim = delete )

DJLibraryService::~DJLibraryService() {
    for (AudioTrack* t : library)
        delete t;
    library.clear();
}

/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    for (AudioTrack* t : library) {
        delete t;
    }
    library.clear();
    library.reserve(library_tracks.size());

    for (const auto& info : library_tracks) {
        AudioTrack* track = nullptr;
        if (info.type == "MP3") {
            bool has_tags = (info.extra_param2 != 0);
            track = new MP3Track(info.title, info.artists, info.duration_seconds, info.bpm, info.extra_param1, has_tags);
        }
        else if (info.type == "WAV") {
            track = new WAVTrack(info.title,info.artists,info.duration_seconds,info.bpm,info.extra_param1,info.extra_param2);
        }
        else {
            std::cerr << "[ERROR] Unknown track type '" << info.type << "' for track \"" << info.title << "\"\n";
            continue; 
        }
        library.push_back(track);
    }
    std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded" << std::endl;
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    return playlist;
}

AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    AudioTrack* found = playlist.find_track(track_title);
    if (found) {
        return found;  
    }
    return nullptr; 
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, const std::vector<int>& track_indices) {
    std::cout << "[INFO] Loading playlist: " << playlist_name << std::endl;
    playlist = Playlist(playlist_name);
    for (int trackIndex  : track_indices) {
        if (trackIndex <= 0 || static_cast<size_t>(trackIndex) > library.size()) {
            std::cerr << "[WARNING] Invalid track index: " << trackIndex  << std::endl;
            continue;
        }
        AudioTrack* sourceTrack  = library[trackIndex  - 1];
        PointerWrapper<AudioTrack> cloned = sourceTrack ->clone();
        if (!cloned) {
            std::cerr << "[ERROR] Failed to clone track: " << sourceTrack ->get_title() << std::endl;
            continue;
        }
        cloned->load();
        cloned->analyze_beatgrid();
        playlist.add_track(cloned.release());
    }
    std::cout << "[INFO] Playlist loaded: " << playlist_name<< " (" << playlist.get_track_count() << " tracks)" << std::endl;
}

std::vector<std::string> DJLibraryService::getTrackTitles() const {
    std::vector<std::string> titles;
    std::vector<AudioTrack*> tracks = playlist.getTracks();
    for (auto it = tracks.rbegin(); it != tracks.rend(); it++) {
        AudioTrack* track = *it;
        if (track != nullptr) {
            titles.push_back(track->get_title());
        }
    }
    return titles;
}