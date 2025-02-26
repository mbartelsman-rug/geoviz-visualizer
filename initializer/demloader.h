#ifndef DEMLOADER_H
#define DEMLOADER_H

#include <QVector>

#include <gdal_priv.h>

class DemLoader {
public:
    DemLoader(const QString& fileName);
    ~DemLoader();

    inline QVector<float> &getData() { return data;};
    inline int &getNBlockXSize() {return nBlockXSize; };
    inline int &getNBlockYSize() {return nBlockYSize; };

private:
    QString fileName;
    GDALDatasetUniquePtr poDataset;

    QVector<float> data;
    int nBlockXSize, nBlockYSize;

};

#endif // DEMLOADER_H
