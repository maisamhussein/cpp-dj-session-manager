#include "MixingEngineService.h"
#include <iostream>
#include <memory>


MixingEngineService::MixingEngineService()
    : decks{nullptr, nullptr} ,active_deck(0), auto_sync(false),bpm_tolerance(0)
{std::cout << "[MixingEngineService] Initialized with 2 empty decks." << std::endl;}


MixingEngineService::~MixingEngineService()
 {
    std::cout << "[MixingEngineService] Cleaning up decks..." << std::endl;
    for (size_t i = 0; i < 2; i++) {
        if (decks[i] != nullptr) {
            delete decks[i];     
            decks[i] = nullptr;  
        }
    }
}



/**
 * Loads a track into an available deck.
 *
 * @param track Reference to the track to be loaded.
 * @return Index of the deck where the track was loaded, or -1 on failure.
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    std::cout << "\n=== Loading Track to Deck ===" << std::endl;
    const std::string& trackName = track.get_title();
    PointerWrapper<AudioTrack> cloned = track.clone();
    if (!cloned) {
        std::cerr << "[ERROR] Track: \"" << trackName << "\" failed to clone" << std::endl;
        return -1;
    }
    bool decks_empty = (decks[0] == nullptr && decks[1] == nullptr);
    size_t target_deck = decks_empty ? 0 : 1 - active_deck;
    std::cout << "[Deck Switch] Target deck: " << target_deck << std::endl;

    cloned->load();
    cloned->analyze_beatgrid();
    if (auto_sync) {
        sync_bpm(cloned);
    }
    if (decks[target_deck] != nullptr) {
        delete decks[target_deck];
        decks[target_deck] = nullptr;
    }

    AudioTrack* raw_ptr = cloned.release();
    decks[target_deck] = raw_ptr;
    std::cout << "[Load Complete] '" << trackName << "' is now loaded on deck " << target_deck << std::endl;
    active_deck = target_deck;
    std::cout << "[Active Deck] Switched to deck " << active_deck << std::endl;
    return static_cast<int>(target_deck);
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; i++) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * Checks whether two tracks can be mixed based on their BPM difference.
 *
 * @param track Track to check for mixing compatibility.
 * @return true if the BPM difference is within the allowed tolerance,
 *         false otherwise.
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    if (decks[active_deck] == nullptr) { return false;}
    if (!track) { return false;}
    double current_bpm = decks[active_deck]->get_bpm();
    double new_bpm = track->get_bpm();
    double diff = std::abs(current_bpm - new_bpm);
    return diff <= bpm_tolerance;
}

/**
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
     if (!track) {
        std::cout << "[Sync BPM] Cannot sync - track is null." << std::endl;
        return;
    }
    if (decks[active_deck] == nullptr) {
        std::cout << "[Sync BPM] Cannot sync - one of the decks is empty." << std::endl;
        return;
    }
    double original_bpm = track->get_bpm();
    double active_bpm   = decks[active_deck]->get_bpm();
    double diff         = std::abs(original_bpm - active_bpm);
    if (diff <= bpm_tolerance) {
        return;
    }
    int synced_bpm = (original_bpm + active_bpm) / 2;
    track->set_bpm(synced_bpm);
     std::cout << "[Sync BPM] Syncing BPM from "<< original_bpm << " to " << synced_bpm << std::endl;
}
