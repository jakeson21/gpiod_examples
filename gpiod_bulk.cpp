#include <gpiod.h>
#include <error.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Based off of: https://lloydrochester.com/post/hardware/libgpiod-input-rpi/
// sudo dtoverlay /home/fuguru/git/pcf8574/pcf8574.dtbo
// gpiod_4lines gpiochip3 0 1 2 3

int main(int argc, char **argv)
{
    if (argc<6)
    {
        printf("Usage: gpiod_4lines <gpiochipx> <in1_pin> <in2_pin> <in3_pin> <out4_pin>");
        exit(1);
    }
    
    unsigned int offsets[4] = {0, 1, 2, 3};
    int values[4] = {0, 0, 0, 0};
    int err;
    struct gpiod_chip *chip;
    struct gpiod_line_request_config config;
    struct gpiod_line_bulk lines;

    const char *chipname = argv[1];
    chip = gpiod_chip_open("/dev/gpiochip3");
    if(!chip)
    {
        perror("gpiod_chip_open");
        goto cleanup;
    }

    // use pins as outputs
    values[2] = 1;

    err = gpiod_chip_get_lines(chip, offsets, 4, &lines);
    if(err)
    {
        perror("gpiod_chip_get_lines");
        goto cleanup;
    }

    memset(&config, 0, sizeof(config));
    config.consumer = "input example";
    config.request_type = GPIOD_LINE_REQUEST_DIRECTION_OUTPUT;
    config.flags = 0;

    // err = gpiod_line_request_bulk(&lines, &config, values);
    // if(err)
    // {
    //     perror("gpiod_line_request_bulk");
    //     goto cleanup;
    // }

    err = gpiod_line_request_bulk_output(&lines, config.consumer, values);
    if(err)
    {
        perror("gpiod_line_request_bulk");
        goto cleanup;
    }

    err = gpiod_line_set_value_bulk(&lines, values);
    if(err)
    {
        perror("gpiod_line_set_value_bulk");
        goto cleanup;
    }

    usleep(2000000);

    cleanup:
    gpiod_line_release_bulk(&lines);
    gpiod_chip_close(chip);

    return EXIT_SUCCESS;
}