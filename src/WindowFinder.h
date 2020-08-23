//
// Created by cook on 8/21/20.
//

#ifndef TIGERCAPTURE_WINDOWFINDER_H
#define TIGERCAPTURE_WINDOWFINDER_H

#include <vector>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <cstdio>
#include <X11/Xutil.h>

class WindowFinder {
public:
    static Window* allWindows(Display* display, ulong* numWindows) {
        Atom property = XInternAtom(display, "_NET_CLIENT_LIST", False), type;
        int form;
        ulong bytesRemaining;
        u_char* list;

        if (
            XGetWindowProperty(
                display, XDefaultRootWindow(display), property,
                0L, (~0L),
                False, XA_WINDOW, &type, &form, numWindows, &bytesRemaining, &list
            ) == Success
            ) {
            return (Window*) list;
        }
        return nullptr;
    };
    static Window* allWindowschild(Display* display, ulong* numWindows, Window root) {
        Atom property = XInternAtom(display, "_NET_CLIENT_LIST", False), type;
        int form;
        ulong bytesRemaining;
        u_char* list;

        if (
            XGetWindowProperty(
                display, root, property,
                0L, (~0L),
                False, XA_WINDOW, &type, &form, numWindows, &bytesRemaining, &list
            ) == Success
            ) {
            return (Window*) list;
        }
        return nullptr;
    };

    static Window* getChildWindows(Display* display, Window window, uint* numChildren) {
        Window rootWindow;
        Window parentWindow;
        Window* list;
        if (
            XQueryTree(display, window, &rootWindow, &parentWindow, &list, numChildren) == Success
        ) {
            return list;
        }
        return nullptr;
    };

    static void addWindowsAndChildren(Display* display, std::vector<Window>* vec, Window* windows, ulong numWindows) {
        for (int i = 0; i < numWindows; i++) {
            auto window = windows[i];
            vec->push_back(window);

            uint numChildren;
            Window* children = getChildWindows(display, window, &numChildren);
            if (children != nullptr) {
                addWindowsAndChildren(display, vec, children, numChildren);
            }
        }
    };

    /// Does not contain root window
    static std::vector<Window>* getAllWindowsRecursively(Display* display) {
        auto windows = new std::vector<Window>();

        // Top level windows
        {
            ulong numTopWindows;
            auto topWindows = WindowFinder::allWindows(display, &numTopWindows);

            if (topWindows == nullptr) return windows;

            addWindowsAndChildren(display, windows, topWindows, numTopWindows);
        }

        return windows;
    }

    static XWindowAttributes* getWindowAttributes(Display* display, Window window) {
        auto attributes = new XWindowAttributes();
        auto result = XGetWindowAttributes(display, window, attributes);
        if (result == Success) {
            return attributes;
        }
        printf("Result == %d\n", result);
        delete attributes;
        return nullptr;
    };

    static char* getWindowName(Display* display, Window window) {
        Atom property = XInternAtom(display, "WM_NAME", False), type;
        int form;
        ulong remaining, length;
        u_char* list;

        if (
            XGetWindowProperty(
                display, window, property,
                0, 1024,
                False, XA_STRING, &type, &form, &length, &remaining, &list
            ) == Success
        ) {
            return (char*) list;
        }
        return nullptr;
    };
};

#endif //TIGERCAPTURE_WINDOWFINDER_H
