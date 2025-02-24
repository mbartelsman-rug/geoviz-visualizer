
#ifndef SETTINGS_H
#define SETTINGS_H

enum class Method {
    Waterlining, Stippling
};

struct Settings {
    bool wireFrameMode = false;

    // General
    double vertical_scale = 1.0;
    Method method = Method::Waterlining;

    // Waterlining
    double s = 30;
    double e = 0.75;
    double t = 0.01;
    double h = 0.0;

    // Stippling
};

#endif //SETTINGS_H
