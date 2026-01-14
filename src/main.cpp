#include "view/main_view.hpp"
#include <unistd.h>

int main() {
    create_ui();

    while (true) {
        lv_timer_handler();
        usleep(5000);
    }

    return 0;
}
