
#ifndef SCENE_H
#define SCENE_H

#include <qlist.h>

#include "Settings.h"
#include "cameras/PerspectiveCamera.h"
#include "lights/Light.h"
#include "materials/Phong.h"
#include "materials/displacement.h"
#include "models/Model.h"

class Mesh;

struct Scene {
    Scene(Settings *settings);

    Settings *settings;

    Camera * camera { new PerspectiveCamera() };
    Light * light { new Light() };
    QList<Material *> materials {/* new Phong*/ };
    QList<Model *> models { };
    Displacement *displacement;
};



#endif //SCENE_H
