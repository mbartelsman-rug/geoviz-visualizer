#include "Scene.h"

#include "materials/Phong.h"
#include "models/Plane.h"

Scene::Scene() {
    Model * plane = new Plane(materials.first());
    models.push_back(plane);
}
