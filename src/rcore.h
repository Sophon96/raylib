#pragma once

//----------------------------------------------------------------------------------
// Feature Test Macros required for this module
//----------------------------------------------------------------------------------
#if (defined(__linux__) || defined(PLATFORM_WEB) || defined(PLATFORM_WEB_RGFW)) && (_XOPEN_SOURCE < 500)
    #undef _XOPEN_SOURCE
    #define _XOPEN_SOURCE 500       // Required for: readlink if compiled with c99 without GNU extensions
#endif

#if (defined(__linux__) || defined(PLATFORM_WEB) || defined(PLATFORM_WEB_RGFW)) && (_POSIX_C_SOURCE < 199309L)
    #undef _POSIX_C_SOURCE
    #define _POSIX_C_SOURCE 199309L // Required for: CLOCK_MONOTONIC if compiled with c99 without GNU extensions
#endif

#include "raylib.h"                 // Declares module functions

#include "config.h"                 // Defines module configuration flags

#include <stdarg.h>                 // Required for: va_list, va_start(), va_end() [Used in TraceLog()]

#if defined(PLATFORM_MEMORY) || defined(PLATFORM_WEB)
    #define SW_GL_FRAMEBUFFER_COPY_BGRA false
#endif
#define RLGL_IMPLEMENTATION
#include "rlgl.h"                   // OpenGL abstraction layer to OpenGL 1.1, 3.3+ or ES2

#define RAYMATH_IMPLEMENTATION
#include "raymath.h"                // Vector2, Vector3, Quaternion and Matrix functionality

#if SUPPORT_GESTURES_SYSTEM
    #define RGESTURES_IMPLEMENTATION
    #include "rgestures.h"          // Gestures detection functionality
#endif

#if SUPPORT_CAMERA_SYSTEM
    #define RCAMERA_IMPLEMENTATION
    #include "rcamera.h"            // Camera system functionality
#endif

#if SUPPORT_COMPRESSION_API
    #define SINFL_IMPLEMENTATION
    #define SINFL_NO_SIMD
    #include "external/sinfl.h"     // Deflate (RFC 1951) decompressor

    #define SDEFL_IMPLEMENTATION
    #include "external/sdefl.h"     // Deflate (RFC 1951) compressor
#endif

#if SUPPORT_RPRAND_GENERATOR
    #define RPRAND_IMPLEMENTATION
    #include "external/rprand.h"
#endif

#if defined(__linux__) && !defined(_GNU_SOURCE)
    #define _GNU_SOURCE
#endif

// Platform specific defines to handle GetApplicationDirectory()
#if defined(_WIN32)
    #if !defined(MAX_PATH)
        #define MAX_PATH 260
    #endif

    struct HINSTANCE__;
    #if defined(__cplusplus)
    extern "C" {
    #endif
    __declspec(dllimport) unsigned long __stdcall GetModuleFileNameA(struct HINSTANCE__ *hModule, char *lpFilename, unsigned long nSize);
    __declspec(dllimport) unsigned long __stdcall GetModuleFileNameW(struct HINSTANCE__ *hModule, wchar_t *lpFilename, unsigned long nSize);
    __declspec(dllimport) int __stdcall WideCharToMultiByte(unsigned int cp, unsigned long flags, const wchar_t *widestr, int cchwide, char *str, int cbmb, const char *defchar, int *used_default);
    __declspec(dllimport) unsigned int __stdcall timeBeginPeriod(unsigned int uPeriod);
    __declspec(dllimport) unsigned int __stdcall timeEndPeriod(unsigned int uPeriod);
    #if defined(__cplusplus)
    }
    #endif
#elif defined(__linux__)
    #include <unistd.h>
#elif defined(__FreeBSD__)
    #include <sys/types.h>
    #include <sys/sysctl.h>
    #include <unistd.h>
#elif defined(__APPLE__)
    #include <sys/syslimits.h>
    #include <mach-o/dyld.h>
#endif

#define _CRT_INTERNAL_NONSTDC_NAMES  1
#include <sys/stat.h>               // Required for: stat(), S_ISREG [Used in GetFileModTime(), IsFilePath()]

#if !defined(S_ISREG) && defined(S_IFMT) && defined(S_IFREG)
    #define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#endif

#if defined(_WIN32) && (defined(_MSC_VER) || defined(__TINYC__))
    #define DIRENT_MALLOC RL_MALLOC
    #define DIRENT_FREE RL_FREE

    #include "external/dirent.h"    // Required for: DIR, opendir(), closedir() [Used in LoadDirectoryFiles()]
#else
    #include <dirent.h>             // Required for: DIR, opendir(), closedir() [Used in LoadDirectoryFiles()]
#endif

#if defined(_WIN32)
    #include <io.h>                 // Required for: _access() [Used in FileExists()]
    #include <direct.h>             // Required for: _getch(), _chdir(), _mkdir()
    #define GETCWD _getcwd          // NOTE: MSDN recommends not to use getcwd(), chdir()
    #define CHDIR _chdir
    #define MKDIR(dir) _mkdir(dir)
    #define ACCESS(fn) _access(fn, 0)
#else
    #include <unistd.h>             // Required for: getch(), chdir(), mkdir(), access()
    #define GETCWD getcwd
    #define CHDIR chdir
    #define MKDIR(dir) mkdir(dir, 0777)
    #define ACCESS(fn) access(fn, F_OK)
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
#ifndef MAX_TRACELOG_MSG_LENGTH
    #define MAX_TRACELOG_MSG_LENGTH      256        // Max length of one trace-log message
#endif

#ifndef MAX_FILEPATH_CAPACITY
    #define MAX_FILEPATH_CAPACITY       8192        // Maximum capacity for filepath
#endif
#ifndef MAX_FILEPATH_LENGTH
    #if defined(_WIN32)
        #define MAX_FILEPATH_LENGTH      256        // On Win32, MAX_PATH = 260 (limits.h) but Windows 10, Version 1607 enables long paths...
    #else
        #define MAX_FILEPATH_LENGTH     4096        // On Linux, PATH_MAX = 4096 by default (limits.h)
    #endif
#endif

#ifndef MAX_KEYBOARD_KEYS
    #define MAX_KEYBOARD_KEYS            512        // Maximum number of keyboard keys supported
#endif
#ifndef MAX_MOUSE_BUTTONS
    #define MAX_MOUSE_BUTTONS              8        // Maximum number of mouse buttons supported
#endif
#ifndef MAX_GAMEPADS
    #define MAX_GAMEPADS                   4        // Maximum number of gamepads supported
#endif
#ifndef MAX_GAMEPAD_NAME_LENGTH
    #define MAX_GAMEPAD_NAME_LENGTH      128        // Maximum number of characters in a gamepad name (byte size)
#endif
#ifndef MAX_GAMEPAD_AXES
    #define MAX_GAMEPAD_AXES               8        // Maximum number of axes supported (per gamepad)
#endif
#ifndef MAX_GAMEPAD_BUTTONS
    #define MAX_GAMEPAD_BUTTONS           32        // Maximum number of buttons supported (per gamepad)
#endif
#ifndef MAX_GAMEPAD_VIBRATION_TIME
    #define MAX_GAMEPAD_VIBRATION_TIME     2.0f     // Maximum vibration time in seconds
#endif
#ifndef MAX_TOUCH_POINTS
    #define MAX_TOUCH_POINTS               8        // Maximum number of touch points supported
#endif
#ifndef MAX_KEY_PRESSED_QUEUE
    #define MAX_KEY_PRESSED_QUEUE         16        // Maximum number of keys in the key input queue
#endif
#ifndef MAX_CHAR_PRESSED_QUEUE
    #define MAX_CHAR_PRESSED_QUEUE        16        // Maximum number of characters in the char input queue
#endif

#ifndef MAX_DECOMPRESSION_SIZE
    #define MAX_DECOMPRESSION_SIZE        64        // Maximum size allocated for decompression in MB
#endif

#ifndef MAX_AUTOMATION_EVENTS
    #define MAX_AUTOMATION_EVENTS      16384        // Maximum number of automation events to record
#endif

// File and directory scan filters
// NOTE: Used in ScanDirectoryFiles(), LoadDirectoryFilesEx() and GetDirectoryFileCountEx()
// WARNING: Custom file filters can be specified but following raylib IsFileExtension() convention: ".png;.wav;.glb"
#ifndef FILE_FILTER_TAG_ALL
    #define FILE_FILTER_TAG_ALL        "*.*"        // Filter to include all file types and directories on scan
#endif
#ifndef FILE_FILTER_TAG_FILE_ONLY
    #define FILE_FILTER_TAG_FILE_ONLY  "FILES*"     // Filter to include all file types on scan (no directories)
#endif
#ifndef FILE_FILTER_TAG_DIR_ONLY
    #define FILE_FILTER_TAG_DIR_ONLY   "DIRS*"      // Filter to include only directories on scan
#endif

// Flags bitwise operation macros
#define FLAG_SET(n, f) ((n) |= (f))
#define FLAG_CLEAR(n, f) ((n) &= ~(f))
#define FLAG_TOGGLE(n, f) ((n) ^= (f))
#define FLAG_IS_SET(n, f) (((n) & (f)) == (f))

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef struct { int x; int y; } Point;
typedef struct { unsigned int width; unsigned int height; } Size;

// Core global state context data
typedef struct CoreData {
    struct {
        const char *title;                  // Window text title const pointer
        unsigned int flags;                 // Configuration flags (bit based), keeps window state
        bool ready;                         // Check if window has been initialized successfully
        bool shouldClose;                   // Check if window set for closing
        bool resizedLastFrame;              // Check if window has been resized last frame
        bool eventWaiting;                  // Wait for events before ending frame
        bool usingFbo;                      // Using FBO (RenderTexture) for rendering instead of default framebuffer

        Size display;                       // Display width and height (monitor, device-screen, LCD, ...)
        Size screen;                        // Screen current width and height
        Point position;                     // Window current position
        Size previousScreen;                // Screen previous width and height (required on fullscreen/borderless-windowed toggle)
        Point previousPosition;             // Window previous position (required on fullscreen/borderless-windowed toggle)
        Size render;                        // Screen framebuffer width and height
        Point renderOffset;                 // Screen framebuffer render offset (Not required anymore?)
        Size currentFbo;                    // Current framebuffer render width and height (depends on active render texture)
        Size screenMin;                     // Screen minimum width and height (for resizable window)
        Size screenMax;                     // Screen maximum width and height (for resizable window)
        Matrix screenScale;                 // Matrix to scale screen (framebuffer rendering)

        char **dropFilepaths;               // Store dropped files paths pointers (provided by GLFW)
        unsigned int dropFileCount;         // Count dropped files strings

    } Window;
    struct {
        const char *basePath;               // Base path for data storage

    } Storage;
    struct {
        struct {
            int exitKey;                    // Default exit key
            char currentKeyState[MAX_KEYBOARD_KEYS]; // Registers current frame key state
            char previousKeyState[MAX_KEYBOARD_KEYS]; // Registers previous frame key state

            // NOTE: Since key press logic involves comparing previous vs current key state,
            // key repeats needs to be handled specially
            char keyRepeatInFrame[MAX_KEYBOARD_KEYS]; // Registers key repeats for current frame

            int keyPressedQueue[MAX_KEY_PRESSED_QUEUE]; // Input keys queue
            int keyPressedQueueCount;       // Input keys queue count

            int charPressedQueue[MAX_CHAR_PRESSED_QUEUE]; // Input characters queue (unicode)
            int charPressedQueueCount;      // Input characters queue count

        } Keyboard;
        struct {
            Vector2 offset;                 // Mouse offset
            Vector2 scale;                  // Mouse scaling
            Vector2 currentPosition;        // Mouse position on screen
            Vector2 previousPosition;       // Previous mouse position
            Vector2 lockedPosition;         // Mouse position when locked

            int cursor;                     // Tracks current mouse cursor
            bool cursorHidden;              // Track if cursor is hidden
            bool cursorLocked;              // Track if cursor is locked (disabled)
            bool cursorOnScreen;            // Tracks if cursor is inside client area

            char currentButtonState[MAX_MOUSE_BUTTONS]; // Registers current mouse button state
            char previousButtonState[MAX_MOUSE_BUTTONS]; // Registers previous mouse button state
            Vector2 currentWheelMove;       // Registers current mouse wheel variation
            Vector2 previousWheelMove;      // Registers previous mouse wheel variation

        } Mouse;
        struct {
            int pointCount;                                 // Number of touch points active
            int pointId[MAX_TOUCH_POINTS];                  // Point identifiers
            Vector2 position[MAX_TOUCH_POINTS];             // Touch position on screen
            Vector2 previousPosition[MAX_TOUCH_POINTS];     // Previous touch position on screen
            char currentTouchState[MAX_TOUCH_POINTS];       // Registers current touch state
            char previousTouchState[MAX_TOUCH_POINTS];      // Registers previous touch state

        } Touch;
        struct {
            int lastButtonPressed;          // Register last gamepad button pressed
            int axisCount[MAX_GAMEPADS];    // Register number of available gamepad axes
            bool ready[MAX_GAMEPADS];       // Flag to know if gamepad is ready
            char name[MAX_GAMEPADS][MAX_GAMEPAD_NAME_LENGTH];               // Gamepad name holder
            char currentButtonState[MAX_GAMEPADS][MAX_GAMEPAD_BUTTONS];     // Current gamepad buttons state
            char previousButtonState[MAX_GAMEPADS][MAX_GAMEPAD_BUTTONS];    // Previous gamepad buttons state
            float axisState[MAX_GAMEPADS][MAX_GAMEPAD_AXES];                // Gamepad axes state

        } Gamepad;
    } Input;
    struct {
        double current;                     // Current time measure (seconds)
        double previous;                    // Previous time measure (seconds)
        double update;                      // Time measure for frame update (seconds)
        double draw;                        // Time measure for frame draw (seconds)
        double frame;                       // Time measure for one frame (seconds)
        double target;                      // Desired time for one frame, if 0 not applied (seconds)
        unsigned long long base;            // Base time measure for hi-res timer (ticks or nanoseconds)
        unsigned int frameCounter;          // Frame counter (frames)

    } Time;
} CoreData;

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
RLAPI const char *raylib_version = RAYLIB_VERSION;  // raylib version exported symbol, required for some bindings

CoreData CORE = { 0 };                              // Global CORE state context

static int logTypeLevel = LOG_INFO;                 // Minimum log type level

static TraceLogCallback traceLog = NULL;            // TraceLog callback function pointer
static LoadFileDataCallback loadFileData = NULL;    // LoadFileData callback function pointer
static SaveFileDataCallback saveFileData = NULL;    // SaveFileText callback function pointer
static LoadFileTextCallback loadFileText = NULL;    // LoadFileText callback function pointer
static SaveFileTextCallback saveFileText = NULL;    // SaveFileText callback function pointer

#if SUPPORT_SCREEN_CAPTURE
static int screenshotCounter = 0;                   // Screenshots counter
#endif

#if SUPPORT_AUTOMATION_EVENTS
// Automation events type
typedef enum AutomationEventType {
    EVENT_NONE = 0,
    // Input events
    INPUT_KEY_UP,                   // param[0]: key
    INPUT_KEY_DOWN,                 // param[0]: key
    INPUT_KEY_PRESSED,              // param[0]: key
    INPUT_KEY_RELEASED,             // param[0]: key
    INPUT_MOUSE_BUTTON_UP,          // param[0]: button
    INPUT_MOUSE_BUTTON_DOWN,        // param[0]: button
    INPUT_MOUSE_POSITION,           // param[0]: x, param[1]: y
    INPUT_MOUSE_WHEEL_MOTION,       // param[0]: x delta, param[1]: y delta
    INPUT_GAMEPAD_CONNECT,          // param[0]: gamepad
    INPUT_GAMEPAD_DISCONNECT,       // param[0]: gamepad
    INPUT_GAMEPAD_BUTTON_UP,        // param[0]: button
    INPUT_GAMEPAD_BUTTON_DOWN,      // param[0]: button
    INPUT_GAMEPAD_AXIS_MOTION,      // param[0]: axis, param[1]: delta
    INPUT_TOUCH_UP,                 // param[0]: id
    INPUT_TOUCH_DOWN,               // param[0]: id
    INPUT_TOUCH_POSITION,           // param[0]: x, param[1]: y
    INPUT_GESTURE,                  // param[0]: gesture
    // Window events
    WINDOW_CLOSE,                   // no params
    WINDOW_MAXIMIZE,                // no params
    WINDOW_MINIMIZE,                // no params
    WINDOW_RESIZE,                  // param[0]: width, param[1]: height
    // Custom events
    ACTION_TAKE_SCREENSHOT,         // no params
    ACTION_SETTARGETFPS             // param[0]: fps
} AutomationEventType;

// Event type name strings, required for export
static const char *autoEventTypeName[] = {
    "EVENT_NONE",
    "INPUT_KEY_UP",
    "INPUT_KEY_DOWN",
    "INPUT_KEY_PRESSED",
    "INPUT_KEY_RELEASED",
    "INPUT_MOUSE_BUTTON_UP",
    "INPUT_MOUSE_BUTTON_DOWN",
    "INPUT_MOUSE_POSITION",
    "INPUT_MOUSE_WHEEL_MOTION",
    "INPUT_GAMEPAD_CONNECT",
    "INPUT_GAMEPAD_DISCONNECT",
    "INPUT_GAMEPAD_BUTTON_UP",
    "INPUT_GAMEPAD_BUTTON_DOWN",
    "INPUT_GAMEPAD_AXIS_MOTION",
    "INPUT_TOUCH_UP",
    "INPUT_TOUCH_DOWN",
    "INPUT_TOUCH_POSITION",
    "INPUT_GESTURE",
    "WINDOW_CLOSE",
    "WINDOW_MAXIMIZE",
    "WINDOW_MINIMIZE",
    "WINDOW_RESIZE",
    "ACTION_TAKE_SCREENSHOT",
    "ACTION_SETTARGETFPS"
};

static AutomationEventList *currentEventList = NULL;        // Current automation events list, set by user, keep internal pointer
static bool automationEventRecording = false;               // Recording automation events flag
//static short automationEventEnabled = 0b0000001111111111; // TODO: Automation events enabled for recording/playing
#endif
//-----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Module Functions Declaration
// NOTE: Those functions are common for all platforms!
//----------------------------------------------------------------------------------
#if SUPPORT_MODULE_RTEXT
extern void LoadFontDefault(void);      // [Module: text] Loads default font on InitWindow()
extern void UnloadFontDefault(void);    // [Module: text] Unloads default font from GPU memory
#endif

extern int InitPlatform(void);          // Initialize platform (graphics, inputs and more)
extern void ClosePlatform(void);        // Close platform

static void InitTimer(void);                                // Initialize timer, hi-resolution if available (required by InitPlatform())
static void SetupViewport(int width, int height);           // Set viewport for a provided width and height

static void ScanDirectoryFiles(const char *basePath, FilePathList *list, const char *filter, unsigned int expectedFileCount, bool scanSubdirs); // Scan all files and directories in a base path

#if SUPPORT_AUTOMATION_EVENTS
static void RecordAutomationEvent(void); // Record frame events (to internal events array)
#endif
