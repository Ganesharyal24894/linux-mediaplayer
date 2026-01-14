#ifndef VIDEO_CONTROLLER_HPP
#define VIDEO_CONTROLLER_HPP

#include "../backend/video_backend.hpp"

class VideoController {
private:
    VideoBackend backend;

public:
    void play(const std::string &url) {
        backend.play_video(url);
    }
};

#endif // VIDEO_CONTROLLER_HPP
