#include "video_backend.hpp"
#include <cstdlib>
#include <iostream>

VideoBackend::VideoBackend() {
    gst_init(nullptr, nullptr);
    pipeline = nullptr;
}

VideoBackend::~VideoBackend() {
    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
    }
}

std::string VideoBackend::get_direct_url(const std::string &youtube_url) {
    std::string cmd = "yt-dlp -f best -g \"" + youtube_url + "\"";
    char buffer[1024];
    std::string result;
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    // Remove newline
    if (!result.empty()) {
        result.erase(result.find_last_not_of("\n\r") + 1);
    }
    return result;
}

void VideoBackend::play_video(const std::string &youtube_url) {
    std::string direct_url = get_direct_url(youtube_url);
    if (direct_url.empty()) {
        std::cerr << "Failed to get video URL!" << std::endl;
        return;
    }

    std::cout << "Playing: " << direct_url << std::endl;

    // Use fbdevsink for framebuffer output
    std::string pipeline_str =
        "playbin uri=\"" + direct_url + "\" video-sink=fbdevsink audio-sink=alsasink";

    GError *error = nullptr;
    pipeline = gst_parse_launch(pipeline_str.c_str(), &error);

    if (!pipeline) {
        std::cerr << "Failed to create pipeline: " << error->message << std::endl;
        g_error_free(error);
        return;
    }

    gst_element_set_state(pipeline, GST_STATE_PLAYING);
}
