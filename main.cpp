//
// Created by Camiel Verdult on 07/07/2022.
//

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#define USING_WIREPI 0

#if USING_WIREPI
    // Use wiringpi
    #include <wiringPi.h>
#else
    // Use bcm2835
    #include "bcm2835.h"
#endif

int setup_gpio(std::vector<int> gpio_pins) {
#if USING_WIREPI
    // Use wiringpi
    if (wiringPiSetup() == -1) {
        std::cout << "WiringPi setup failed!\n";
        return 1;
    }

    for (int i = 0; i < 7; i++) {
        pinMode(gpio_pins[i], OUTPUT);
    }
#else
    // Use bcm2835
    if (!bcm2835_init()) {
        printf("BCM2835 setup failed!\n");
        return 1;
    }

    // Set the pin to be an output
    for (int i = 0; i < 7; i++) {
        bcm2835_gpio_fsel(gpio_pins[i], BCM2835_GPIO_FSEL_OUTP);
    }
#endif

    return 0;
}

void set_gpio(unsigned int gpio, int value) {

#if USING_WIREPI
    digitalWrite(gpio, value);
#else
    bcm2835_gpio_write(gpio, value);
#endif
}

void write_number_to_segment(int number, std::vector<int> gpio_pins, std::vector<int> number_matrix) {

    if (number < 0 || number > 9) {
        std::cout << "Number passed to write to segment is too big!\n";
        return;
    }

    for (int i = 0; i < 7; i++) {
        // We invert the signal because it is a common anode display
        set_gpio(gpio_pins[i], !number_matrix[number * 7 + i]);
    }
}

int main(int argc, char* argv[]) {

#if USING_WIREPI
    std::vector<int> gpio_pins = {15, 16, 1, 4, 5, 6, 10};
#else
    std::vector<int> gpio_pins = {14, 15, 18, 23, 24, 25, 8};
#endif

    std::vector<int> number_matrix = {
        1, 1, 1, 1, 1, 1, 0,
        0, 1, 1, 0, 0, 0, 0,
        1, 1, 0, 1, 1, 0, 1,
        1, 1, 1, 1, 0, 0, 1,
        0, 1, 1, 0, 0, 1, 1,
        1, 0, 1, 1, 0, 1, 1,
        1, 0, 1, 1, 1, 1, 1,
        1, 1, 1, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 0, 1, 1
    };

    setup_gpio(gpio_pins);

    while (true) {
        for (int i = 0; i < 10; i++) {
            std::cout << "Writing " << i << " to seven segment.\n";
            write_number_to_segment(i, gpio_pins, number_matrix);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        std::cout << "\n\n\n";
    }

    return EXIT_SUCCESS;
}