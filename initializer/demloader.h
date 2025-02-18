#ifndef DEMLOADER_H
#define DEMLOADER_H

#include <QString>
#include <QVector>
#include <QVector2D>
#include <QVector3D>

#include <gdal_priv.h>

#include "materials/displacement.h"

class DemLoader {
public:
    DemLoader(const QString& fileName);
    ~DemLoader();

    Displacement* getDisplacement();

private:
    QString fileName;
    GDALDatasetUniquePtr poDataset;


};

#endif // DEMLOADER_H
