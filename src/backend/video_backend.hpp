#ifndef VIDEO_BACKEND_HPP
#define VIDEO_BACKEND_HPP

#include <gst/gst.h>
#include <string>

class VideoBackend {
private:
    GstElement *pipeline;

public:
    VideoBackend();
    ~VideoBackend();
    std::string get_direct_url(const std::string &youtube_url);
    void play_video(const std::string &youtube_url);
};

#endif // VIDEO_BACKEND_HPP
