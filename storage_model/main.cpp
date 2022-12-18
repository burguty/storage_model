#include "MainWindow.h"
#include "OpenWindow.h"

int main() {
    OpenWindow open_window;
    MainWindow main_window;
    while (true) {
        ModelData* data = open_window.MainLoop();
        if (data == nullptr) {
            return 0;
        }
        if (main_window.MainLoop(data)) {
            delete data;
            return 0;
        }
        delete data;
    }
}
