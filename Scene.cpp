#include "Scene.h"

#include "materials/Phong.h"
#include "materials/displacement.h"
#include "initializer/demloader.h"
#include "models/Plane.h"
#include "models/Terrain.h"

Scene::Scene()
{
    displacement = new Displacement();

    materials.append(displacement);

    Plane *plane = new Plane(displacement);
    models.push_back(plane);

    //Plane *plane = new Plane(map);
    //models.push_back(plane);
}
