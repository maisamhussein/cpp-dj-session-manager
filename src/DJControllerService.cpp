#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) 
{
    const std::string& title = track.get_title();
    if (cache.contains(title)) {
    cache.get(title);
    return 1;
    }
    PointerWrapper<AudioTrack> cloned = track.clone();
    if (!cloned) {
        std::cerr << "[ERROR] Track: " << title << " failed to clone" << std::endl;
        return -2;  
    }
    cloned->load();
    cloned->analyze_beatgrid();
    bool evicted = cache.put(std::move(cloned));
    if (evicted) {
        // MISS with eviction
        return -1;
    } else {
        // MISS without eviction
        return 0;
    }
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
        return cache.get(track_title);
}
