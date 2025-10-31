// Editor3D.cpp : Define el punto de entrada de la aplicación.
//

#include "framework.h"
#include "Editor3D.h"


LRESULT CALLBACK  WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    switch (message) {

    case IDM_EXIT:
        DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    };

    return 0;
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,_In_ int nCmdShow){

    const wchar_t* szWindowClass = L"Wnd3D";

    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EDITOR3D));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_EDITOR3D);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
   
    if (!RegisterClassExW(&wcex)) {

        MessageBoxW(
           
            0,
            L"RegisterClassExW Error",
            L"Error",
            MB_ICONERROR);

        return 0;
    }

    HWND hwnd = CreateWindowExW(

        WS_EX_APPWINDOW,
        szWindowClass,
        L"Editor3D",
        WS_OVERLAPPEDWINDOW,
        0, 
        0, 
        1200, 
        720, 
        nullptr, 
        nullptr, 
        hInstance, 
        nullptr);

    if (!hwnd) {

        MessageBoxW(

            0,
            L"Create HWND Error",
            L"Error",
            MB_ICONERROR);

        return 0;
    }


    // Pointers D3D11 and DXGI
    
    const D3D_FEATURE_LEVEL feature_Levels[] = {
        
        D3D_FEATURE_LEVEL_11_1, 
        D3D_FEATURE_LEVEL_11_0, 
        D3D_FEATURE_LEVEL_10_1, 
        D3D_FEATURE_LEVEL_10_0 };

    D3D_FEATURE_LEVEL feature_Level = D3D_FEATURE_LEVEL_11_0;
   
    IDXGIAdapter* pAdapterMain = nullptr;
    IDXGISwapChain* pSwapChain = nullptr;
    ID3D11Device* pDevice = nullptr;
    ID3D11DeviceContext* pDeviceContext = nullptr;


    // Swap chain description
    DXGI_SWAP_CHAIN_DESC chain_desc = {};

    chain_desc.BufferCount = 1;
    chain_desc.BufferDesc.Height = 720;
    chain_desc.BufferDesc.Width = 1200;
    chain_desc.BufferDesc.RefreshRate.Denominator = 1;
    chain_desc.BufferDesc.RefreshRate.Numerator = 60;
    chain_desc.SampleDesc.Count = 1;
    chain_desc.SampleDesc.Quality = 0;
    chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    chain_desc.Windowed = TRUE;
    chain_desc.OutputWindow = hwnd;


    // Create device and swap chain
    HRESULT h = D3D11CreateDeviceAndSwapChain(

        pAdapterMain,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        feature_Levels,
        ARRAYSIZE(feature_Levels),
        D3D11_SDK_VERSION,
        &chain_desc,
        &pSwapChain,
        &pDevice,
        &feature_Level,
        &pDeviceContext);


    D3D11_VIEWPORT viewPort = {};

    viewPort.Width = 1600.0f;
    viewPort.Height = 900.0f;
    viewPort.MaxDepth = 1.0f;
    viewPort.MinDepth = 0.0f;
    viewPort.TopLeftX = 0.0f;
    viewPort.TopLeftY = 0.0f;

    pDeviceContext->RSSetViewports(1, &viewPort);

    MSG msg;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Bucle principal de mensajes:
    while (GetMessage(&msg, nullptr, 0, 0)){
       
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        
    }


    // Free Memory
    if (pAdapterMain) {

        pAdapterMain->Release();
        pAdapterMain = nullptr;

    }

    if (pSwapChain) {

        pSwapChain->Release();
        pSwapChain = nullptr;

    }

    if (pDevice) {

        pDevice->Release();
        pDevice = nullptr;

    }

    if (pDeviceContext) {

        pDeviceContext->Release();
        pDeviceContext = nullptr;

    }


    return (int) msg.wParam;

}
