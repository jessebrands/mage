#include <vector>
#include <iostream>
#include <windows.h>
#include <memory>

#include <mage/platform/com_ptr.hpp>
#include <mage/platform/win32_window.hpp>

#include <dxgi.h>
#include <d3d11.h>

int APIENTRY WinMain([[maybe_unused]] HINSTANCE hinstance,
                     [[maybe_unused]] HINSTANCE prev_instance,
                     [[maybe_unused]] PSTR cmd_line,
                     [[maybe_unused]] int show_cmd) {
    mage::win32_window window(hinstance, show_cmd);

    mage::com_ptr<ID3D11Device> device;
    mage::com_ptr<ID3D11DeviceContext> device_context;

    HRESULT result = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        device.get_address_of(),
        nullptr,
        device_context.get_address_of()
    );

    if (FAILED(result)) {
        std::cerr << "could not create D3D11 device, sorry..." << std::endl;
        return 1;
    }

    mage::com_ptr<ID3D11Debug> debugger;
    if (FAILED(device.query_interface(debugger))) {
        std::cerr << "Could not retrieve debugger from D3D11 device." << std::endl;
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
