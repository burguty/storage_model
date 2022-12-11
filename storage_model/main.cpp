#include "MainWindow.h"

int main() {
    MainWindow main_window;
    while (true) {
        if (main_window.MainLoop(nullptr))
            return 0;
    }
}
