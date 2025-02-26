
#ifndef SETTINGS_H
#define SETTINGS_H

enum class Method {
    Flat, Waterlining, Stippling, ContourHatching
};

struct Settings {
    bool wireFrameMode = false;

    // General
    double vertical_scale = 1.0;
    Method method = Method::Flat;

    int map_width = 0;
    int map_height = 0;

    // Waterlining
    double s = 30;
    double e = 0.75;
    double t = 0.01;
    double h = 0.0;

    // Stippling


    // Contour hatching
    int c_mult1 = 50;
    int c_mult2 = 50;
    float c_exponent = 5;
    float c_density = 0.5;
};

#endif //SETTINGS_H
