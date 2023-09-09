#include "window.h"

#include <algorithm>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_syswm.h>
#include <SDL_ttf.h>

#ifdef _WIN32
#include <Windows.h>
#endif//_WIN32

Window window;
bool running;


namespace {


void auto_window_size()
{
    SDL_DisplayMode display_mode;
    if (SDL_GetDesktopDisplayMode(0, &display_mode) != 0)
    {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        window.width = display_mode.w = WIDTH;
        window.height = display_mode.h = HEIGHT;
    }
    else {
        window.height = std::max(
            std::min(display_mode.h, 180),
            std::min(display_mode.h - 61, display_mode.h * 4 / 5)
        );
        window.width = window.height * 16 / 9;
    }
}

void set_borderless_fullscreen(bool f)
{
    if (f) {
        if (!window.borderless_fullscreen) {
            window.old_width = window.width;
            window.old_height = window.height;            
            SDL_GetWindowPosition(window.ptr2, &window.old_pos_x, &window.old_pos_y);
            window.borderless_fullscreen = true;
        }

        SDL_Rect bounds;
        SDL_GetDisplayBounds(window.display, &bounds);
        SDL_SetWindowBordered(window.ptr2, SDL_FALSE);
        SDL_SetWindowResizable(window.ptr2, SDL_FALSE);
        SDL_SetWindowSize(window.ptr2, bounds.w, bounds.h);
        SDL_SetWindowPosition(window.ptr, bounds.x, bounds.y);
        SDL_SetWindowPosition(window.ptr2, bounds.x, bounds.y);
    }
    else {
        window.borderless_fullscreen = false;
        SDL_SetWindowBordered(window.ptr2, SDL_TRUE);
        SDL_SetWindowResizable(window.ptr2, SDL_TRUE);
        SDL_SetWindowSize(window.ptr2, window.old_width, window.old_height);
        SDL_SetWindowPosition(window.ptr, window.old_pos_x, window.old_pos_y);
        SDL_SetWindowPosition(window.ptr2, window.old_pos_x, window.old_pos_y);

    }
}

void toggle_borderless_fullscreen()
{
    if (!window.borderless_fullscreen) {
        set_borderless_fullscreen(true);
    }
    else {
        set_borderless_fullscreen(false);
    }
}

void center_screen()
{
    SDL_SetWindowPosition(window.ptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void set_min_max_window_size(SDL_Window* ptr_window)
{
    SDL_DisplayMode display_mode;
    if (SDL_GetDesktopDisplayMode(window.display, &display_mode) == 0) {
        SDL_SetWindowMaximumSize(ptr_window, display_mode.w, display_mode.h);
        SDL_SetWindowMinimumSize(
            ptr_window, 
            std::min(display_mode.w, 320),
            std::min(display_mode.h, 180)
        );
    }
    else {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        SDL_SetWindowMinimumSize(ptr_window, 320, 180);
    }
}

// Makes a window transparent by setting a transparency color.
bool MakeWindowTransparent(SDL_Window* window, int alpha) {
#ifdef _WIN32
    COLORREF colorKey = RGB(192, 192, 192);
    // Get window handle (https://stackoverflow.com/a/24118145/3357935)
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);  // Initialize wmInfo
    SDL_GetWindowWMInfo(window, &wmInfo);
    HWND hWnd = wmInfo.info.win.window;


    static bool layered = false;
    // Change window type to layered (https://stackoverflow.com/a/3970218/3357935)
    if (!layered) {
        SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT);
        layered = true;
    }

    // Set transparency color
    return SetLayeredWindowAttributes(hWnd, colorKey, alpha, LWA_COLORKEY);
#else
    return true;
#endif
}

} // namespace

bool window_init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL_Init(SDL_INIT_VIDEO) failed: %s\n", SDL_GetError());
        return false;
    }

    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"))
    {
        SDL_Log("Warning: Linear texture scaling not enabled!\n");
    }

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    auto_window_size();
    window.ptr = SDL_CreateWindow(
        WINDOW_NAME,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window.width,
        window.height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
    );
    if (!window.ptr) {
        SDL_Log("SDL_CreateWindow failed: %s\n", SDL_GetError());
        return false;
    }

    int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if(!(IMG_Init(img_flags) & img_flags))
    {
        SDL_Log("IMG_Init failed:: %s\n", IMG_GetError());
        return false;
    }

    if(TTF_Init() < 0)
    {
        SDL_Log("TTF_Init() failed: %s\n", TTF_GetError());
        return false;
    }


    if (window.transparent) {
        MakeWindowTransparent(window.ptr, 0);
        window.ptr2 = SDL_CreateWindow(
            WINDOW_NAME,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            window.width,
            window.height,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SKIP_TASKBAR
        );
        SDL_SetWindowOpacity(window.ptr2, 0.01f);
        SDL_RaiseWindow(window.ptr2);
    }
    else {
        window.ptr2 = window.ptr;
        SDL_SetWindowResizable(window.ptr, SDL_TRUE);
    }
    set_min_max_window_size(window.ptr);
    set_min_max_window_size(window.ptr2);

    SDL_Log("window_init success\n");

    running = true;

    return true;
}

void window_close()
{
    running = false;

    SDL_DestroyWindow(window.ptr);
    SDL_DestroyWindow(window.ptr2);
    window.ptr = nullptr;

    TTF_Quit();
	IMG_Quit();
    SDL_Quit();

    SDL_Log("SDL shutdown\n");
}

void handle_window_events(SDL_Event& e)
{
    if (e.type == SDL_WINDOWEVENT) {
        switch(e.window.event) {
            case SDL_WINDOWEVENT_SIZE_CHANGED: {
                window.width = e.window.data1;
                window.height = e.window.data2;
                SDL_SetWindowSize(window.ptr, window.width, window.height);
                break;
            }
            case SDL_WINDOWEVENT_MINIMIZED: {
                window.minimized = true;
                SDL_MinimizeWindow(window.ptr);
                SDL_MinimizeWindow(window.ptr2);
                break;
            }
            case SDL_WINDOWEVENT_RESTORED: {
                window.minimized = false;
                if (window.borderless_fullscreen) {
                    set_borderless_fullscreen(true);
                }
                SDL_RestoreWindow(window.ptr);
                SDL_RestoreWindow(window.ptr2);
                break;
            }
            case SDL_WINDOWEVENT_MOVED: {
                int x, y;
                SDL_GetWindowPosition(window.ptr2, &x, &y);
                SDL_SetWindowPosition(window.ptr, x, y);
                break;
            }
            case SDL_WINDOWEVENT_DISPLAY_CHANGED: {
                window.display = e.display.data1;
                set_min_max_window_size(window.ptr);
                set_min_max_window_size(window.ptr2);
                break;
            }
            case SDL_WINDOWEVENT_CLOSE: {
                running = false;
                SDL_Log("Quit\n");
                break;
            }
            case SDL_WINDOWEVENT_FOCUS_GAINED: {
                auto ptr_window = SDL_GetWindowFromID(e.window.windowID);
                if (window.transparent) {
                    if (ptr_window == window.ptr) {
                        SDL_RaiseWindow(window.ptr2);
                    }
                }
                break;
            }
        }
    }
    if (e.type == SDL_QUIT) {
        running = false;
        SDL_Log("Quit\n");
    }
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_F11:
            case SDLK_f: {
                toggle_borderless_fullscreen();
                break;
            }
            case SDLK_c: {
                center_screen();
                break;
            }
            case SDLK_t: {
                SDL_SetWindowOpacity(window.ptr, 0.5f);
                break;
            }
            case SDLK_o: {
                SDL_SetWindowOpacity(window.ptr, 1.0f);
                break;
            }
        }
    }
}
