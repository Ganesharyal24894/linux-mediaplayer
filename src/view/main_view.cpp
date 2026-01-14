#include "main_view.hpp"
#include "../controller/video_controller.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <cstring>
#include <iostream>

static VideoController controller;
static int fb_fd = -1;
static char *fb_mem = nullptr;
static struct fb_var_screeninfo vinfo;
static struct fb_fix_screeninfo finfo;
static lv_color_t *draw_buf;

static void fb_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
    int32_t w = lv_area_get_width(area);
    int32_t h = lv_area_get_height(area);

    for (int32_t y = 0; y < h; y++) {
        int32_t fb_y = area->y1 + y;
        if (fb_y >= 0 && fb_y < (int32_t)vinfo.yres) {
            int32_t fb_offset = (fb_y * finfo.line_length) + (area->x1 * (vinfo.bits_per_pixel / 8));
            memcpy(fb_mem + fb_offset, px_map + (y * w * (vinfo.bits_per_pixel / 8)),
                   w * (vinfo.bits_per_pixel / 8));
        }
    }

    lv_display_flush_ready(disp);
}

static bool init_framebuffer() {
    fb_fd = open("/dev/fb0", O_RDWR);
    if (fb_fd < 0) {
        std::cerr << "Failed to open framebuffer device" << std::endl;
        return false;
    }

    if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        std::cerr << "Failed to get framebuffer info" << std::endl;
        close(fb_fd);
        return false;
    }

    if (ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo) < 0) {
        std::cerr << "Failed to get fixed framebuffer info" << std::endl;
        close(fb_fd);
        return false;
    }

    size_t fb_size = finfo.line_length * vinfo.yres;
    fb_mem = (char *)mmap(nullptr, fb_size, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
    if (fb_mem == MAP_FAILED) {
        std::cerr << "Failed to mmap framebuffer" << std::endl;
        close(fb_fd);
        return false;
    }

    std::cout << "Framebuffer: " << vinfo.xres << "x" << vinfo.yres
              << " @ " << vinfo.bits_per_pixel << "bpp" << std::endl;

    return true;
}

void create_ui() {
    lv_init();

    if (!init_framebuffer()) {
        std::cerr << "Framebuffer init failed, UI will not display" << std::endl;
        return;
    }

    // Create LVGL display
    lv_display_t *disp = lv_display_create(vinfo.xres, vinfo.yres);

    // Allocate draw buffer
    size_t buf_size = vinfo.xres * vinfo.yres * sizeof(lv_color_t);
    draw_buf = (lv_color_t *)malloc(buf_size);
    lv_display_set_buffers(disp, draw_buf, nullptr, buf_size, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(disp, fb_flush);

    lv_obj_t *scr = lv_screen_active();

    lv_obj_t *btn = lv_button_create(scr);
    lv_obj_set_size(btn, 200, 80);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, "Play Video");
    lv_obj_center(label);

    lv_obj_add_event_cb(btn, [](lv_event_t *e) {
        controller.play("https://www.youtube.com/watch?v=dQw4w9WgXcQ");
    }, LV_EVENT_CLICKED, nullptr);
}
