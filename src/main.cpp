#include <vector>
#include <iostream>
#include <windows.h>


#include <mage/platform/com_ptr.hpp>
#include <mage/platform/win32_window.hpp>

#include <dxgi.h>

int APIENTRY WinMain([[maybe_unused]] HINSTANCE hinstance,
                     [[maybe_unused]] HINSTANCE prev_instance,
                     [[maybe_unused]] PSTR cmd_line,
                     [[maybe_unused]] int show_cmd) {
    mage::win32_window window(hinstance, show_cmd);

    mage::com_ptr<IDXGIFactory1> factory;
    CreateDXGIFactory1(__uuidof(IDXGIFactory1), factory.get_address_of());

    std::vector<mage::com_ptr<IDXGIAdapter>> devices;

    IDXGIAdapter1* adapter = nullptr;
    UINT adapter_index = 0;
    while (factory->EnumAdapters1(adapter_index++, &adapter) != DXGI_ERROR_NOT_FOUND) {
        devices.emplace_back(adapter);
    }

    MSG msg;
    BOOL ret;
    while ((ret = GetMessage(&msg, nullptr, 0, 0)) != 0) {
        if (ret == -1) {
            return ret;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return ret;
}
