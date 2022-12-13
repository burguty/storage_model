#include "MainWindow.h"
#include "OpenWindow.h"

int main() {
    OpenWindow open_window;
    MainWindow main_window;
    while (true) {
        if (open_window.MainLoop() == nullptr) {
            return 0;
        }
        if (main_window.MainLoop(nullptr)) {
            return 0;
        }
    }
}
