#pragma once
#include <string>

enum class PlaybackState {
    Idle,
    Playing,
    Paused
};

class VideoModel {
public:
    void set_url(const std::string& url) {
        video_url = url;
    }

    const std::string& get_url() const {
        return video_url;
    }

    void set_state(PlaybackState s) {
        state = s;
    }

    PlaybackState get_state() const {
        return state;
    }

private:
    std::string video_url;
    PlaybackState state { PlaybackState::Idle };
};
