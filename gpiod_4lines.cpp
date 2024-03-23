#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

// sudo dtoverlay /home/fuguru/git/pcf8574/pcf8574.dtbo
// gpiod_4lines gpiochip3 0 1 2 3

int main(int argc, char **argv)
{
    if (argc<6)
    {
        printf("Usage: gpiod_4lines <gpiochipx> <in1_pin> <in2_pin> <in3_pin> <out4_pin>");
        exit(1);
    }

    const char *chipname = argv[1];
    const int pin1 = atoi(argv[2]);
    const int pin2 = atoi(argv[3]);
    const int pin3 = atoi(argv[4]);
    const int pin4 = atoi(argv[5]);

    struct gpiod_chip *chip;
    struct gpiod_line *lineRed;    // Red LED
    struct gpiod_line *lineGreen;  // Green LED
    struct gpiod_line *lineYellow; // Yellow LED
    struct gpiod_line *lineButton; // Pushbutton
    int i, val;

    // Open GPIO chip
    chip = gpiod_chip_open_by_name(chipname);

    // Open GPIO lines
    lineRed = gpiod_chip_get_line(chip, pin1);
    lineGreen = gpiod_chip_get_line(chip, pin2);
    lineYellow = gpiod_chip_get_line(chip, pin3);
    lineButton = gpiod_chip_get_line(chip, pin4);

    // Open LED lines for output
    gpiod_line_request_output(lineRed, "in1_pin", 0);
    gpiod_line_request_output(lineGreen, "in2_pin", 0);
    gpiod_line_request_output(lineYellow, "in3_pin", 0);

    // Open switch line for input
    gpiod_line_request_input(lineButton, "out4_pin");

    // Blink LEDs in a binary pattern
    i = 0;
    while (true) {
        gpiod_line_set_value(lineRed, (i & 1) != 0);
        gpiod_line_set_value(lineGreen, (i & 2) != 0);
        gpiod_line_set_value(lineYellow, (i & 4) != 0);

        // Read button status and exit if pressed
        val = gpiod_line_get_value(lineButton);
        if (val == 0) {
        break;
        }

        usleep(100000);
        i++;
    }

    // Release lines and chip
    gpiod_line_release(lineRed);
    gpiod_line_release(lineGreen);
    gpiod_line_release(lineYellow);
    gpiod_line_release(lineButton);
    gpiod_chip_close(chip);
    return 0;
}
