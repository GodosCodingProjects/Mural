/*
* Project: WallPaperAnimator
* File: App.cpp
* 
* Created: 2022/08/18
* Version: 2022/08/24
* 
* Contributiors:
*   Godo
*/

#include <windows.h>
#include <filesystem>
#include <fstream>
#include <tlhelp32.h>

#include <wininet.h> // Required before importing <shlobj_core.h> for IActiveDesktop Interface to be defined
#include <shlobj_core.h>

#define PROC_NAME "WallPaperAnimator.exe"
#define PROC_NAME_W L"WallPaperAnimator.exe"
#define CONFIG_FILE "\\config.txt"
#define FRAMES_FOLDER "\\frames"
#define MAX_LINE_LENGTH 255

#define WP_CENTER "center"
#define WP_TILE "tile"
#define WP_STRETCH "stretch"
#define WP_FIT "fit"
#define WP_FILL "fill"
#define WP_SPAN "span"

#define MIN_DELAY 16
#define ONE_SEC 1000
#define ITERATION_TIME 5000

#define HEX 16
#define DEC 10

enum Arguments {
    EXE,
    PATH,
    N_ARGS
};

enum Config {
    BG_COLOR,
    STYLE,
    FPS,
    N_CONFIG
};

/*
* Resources consulted:
* 
* Find the process by name:
* https://stackoverflow.com/questions/865152/how-can-i-get-a-process-handle-by-its-name-in-c
* 
* Kill a process:
* https://stackoverflow.com/questions/1916574/how-to-effectively-kill-a-process-in-c-win32
* 
*/
void killOtherInstances() {
    // Find all instances of the current program that are running
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            // Terminate all other instances of the program
            if (_wcsicmp(entry.szExeFile, PROC_NAME_W) == 0 && entry.th32ProcessID != GetCurrentProcessId())
            {
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
                TerminateProcess(hProcess, 1);
                CloseHandle(hProcess);
            }
        }
    }

    CloseHandle(snapshot);
}

/*
* Resources consulted:
* 
* http://eddiejackson.net/wp/?p=21882
*/
void setBackgroundColor(const char* hexColor_str) {
    int aElements[1] = { COLOR_BACKGROUND };
    DWORD aNewColors[1] = { std::stoi(hexColor_str, nullptr, HEX) };
    SetSysColors(1, aElements, aNewColors);
}

/*
* Resources consulted:
* 
* Hearing about IActiveDesktop:
* https://stackoverflow.com/questions/69294365/how-does-one-tell-windows-10-to-tile-center-or-stretch-desktop-wallpaper-using
*
* Use the IActiveDesktop:
* https://cpp.hotexamples.com/examples/-/IActiveDesktop/ApplyChanges/cpp-iactivedesktop-applychanges-method-examples.html
* 
* Doc:
* https://docs.microsoft.com/en-us/windows/win32/api/shlobj_core/nn-shlobj_core-iactivedesktop
*/
void setWallPaperStyle(const char* style_str) {
    CoInitialize(NULL);

    IActiveDesktop* pDesk;
    if (S_OK != CoCreateInstance(CLSID_ActiveDesktop, NULL, CLSCTX_INPROC_SERVER, IID_IActiveDesktop, (void**)&pDesk))
    {
        pDesk->Release();

        CoFreeUnusedLibraries();
        CoUninitialize();

        return;
    }
    WALLPAPEROPT wallpaperopt;
    wallpaperopt.dwSize = sizeof(WALLPAPEROPT);

    if (strcmp(style_str, WP_CENTER) == 0) {
        wallpaperopt.dwStyle = WPSTYLE_CENTER;
    }
    else if (strcmp(style_str, WP_TILE) == 0) {
        wallpaperopt.dwStyle = WPSTYLE_TILE;
    }
    else if (strcmp(style_str, WP_STRETCH) == 0) {
        wallpaperopt.dwStyle = WPSTYLE_STRETCH;
    }
    else if (strcmp(style_str, WP_FILL) == 0) {
        wallpaperopt.dwStyle = WPSTYLE_CROPTOFIT;
    }
    else if (strcmp(style_str, WP_SPAN) == 0) {
        wallpaperopt.dwStyle = WPSTYLE_SPAN;
    }
    else {
        wallpaperopt.dwStyle = WPSTYLE_KEEPASPECT;
    }

    pDesk->SetWallpaperOptions(&wallpaperopt, 0);
    pDesk->ApplyChanges(AD_APPLY_ALL);
    pDesk->Release();

    CoFreeUnusedLibraries();
    CoUninitialize();
}

/*
* Resources consulted:
*
* Find all files in a directory:
* https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
* 
* Set the WallPaper image:
* https://stackoverflow.com/questions/3331682/change-wallpaper-programmatically-using-c-and-windows-api
*/
void animateWallPaper(char* folderPath, char* fps_str) {
    int delay = ONE_SEC / std::stoi(fps_str, nullptr, DEC);
    delay = delay < MIN_DELAY ? MIN_DELAY : delay;

    size_t nImages = 0;
    auto paths = std::vector<LPWSTR>();

    // Loop through images' file paths a first time and construct an array
    {
        char* configPath = new char[MAX_LINE_LENGTH];
        strcpy_s(configPath, MAX_LINE_LENGTH, folderPath);
        strcat_s(configPath, MAX_LINE_LENGTH, FRAMES_FOLDER);

        for (const auto& entry : std::filesystem::directory_iterator(configPath)) {
            LPWSTR pathP = new wchar_t[MAX_LINE_LENGTH];
            wcscpy_s(pathP, MAX_LINE_LENGTH, const_cast<LPWSTR>(entry.path().c_str()));

            paths.push_back(pathP);
            ++nImages;
        }
    }

    // Loop through the images and set them as the WallPaper
    for (size_t i = 0; /* INFINITE LOOP */ ; i = (i >= nImages - 1) ? 0 : i + 1) {
        SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, paths[i], SPIF_UPDATEINIFILE);
        Sleep(delay);
    }
}

/*
* Arguments:
* 1- The path of the folder to target. That folder must contain the file config.txt and the frames folder.
* 
* Resources consulted:
* 
* No console:
* https://stackoverflow.com/questions/9618815/i-dont-want-console-to-appear-when-i-run-c-program
*/
int main(int argc, char** argv) {
    // Kill the already existing process of this program (if it does indeed exist)
    killOtherInstances();

    // Open the config file and read the arguments
    char* args[N_CONFIG];

    {
        char* configPath = new char[MAX_LINE_LENGTH];
        strcpy_s(configPath, MAX_LINE_LENGTH, argv[PATH]);
        strcat_s(configPath, MAX_LINE_LENGTH, CONFIG_FILE);
        std::ifstream in(configPath, std::ios_base::in);

        if (in.fail()) {
            return -1;
        }

        for (size_t i = 0; i < N_CONFIG; ++i) {
            args[i] = new char[MAX_LINE_LENGTH];
            in.getline(args[i], MAX_LINE_LENGTH, '\n');
        }

        in.close();
    }

    // Change WallPaper Background color
    setBackgroundColor(args[BG_COLOR]);

    // Change WallPaper style
    setWallPaperStyle(args[STYLE]);

    // Gif loop
    animateWallPaper(argv[PATH], args[FPS]);
}