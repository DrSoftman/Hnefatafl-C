#ifndef WINDOW_H
#define WINDOW_H

#include <roofnut.h>
#include <stdio.h>

// Function to create and show a window using RoofNut
int createWindow() {
    if (roofnut_init() != ROOFNUT_SUCCESS) {
        printf("RoofNut Initialization Error\n");
        return 1;
    }

    Roofnut_Window *window = roofnut_window_create("Hello World!", 100, 100, 640, 480);
    if (window == NULL) {
        printf("RoofNut Window Creation Error\n");
        roofnut_shutdown();
        return 1;
    }

    roofnut_window_show(window);
    roofnut_delay(3000); // Display the window for 3 seconds
    roofnut_window_destroy(window);
    roofnut_shutdown();

    return 0;
}

#endif // WINDOW_H
