#include <windows.h>

int APIENTRY WinMain(HINSTANCE instance,
                     [[maybe_unused]] HINSTANCE prev_instance,
                     [[maybe_unused]] PSTR cmd_line,
                     int show_cmd) {
    return MessageBox(nullptr, TEXT(MAGE_VERSION), TEXT("Mage3D"), 0);
}
