#include "demloader.h"

#include <QDebug>

#include <gdal_priv.h>
#include <qdir.h>

DemLoader::DemLoader(const QString& fileName){
    this->fileName = fileName;
}

DemLoader::~DemLoader(){
    // TODO
}


Displacement *DemLoader::getDisplacement(){
    GDALAllRegister(); // TODO move to main

    const GDALAccess eAccess = GA_ReadOnly;
    poDataset = GDALDatasetUniquePtr(GDALDataset::FromHandle(GDALOpen(fileName.toLocal8Bit().data(), eAccess)));

    if (!poDataset){
        qDebug() << "Loading failed!";
    }

    /**
     * Get Dataset Information
     */
    double        adfGeoTransform[6];
    qDebug() <<  "Driver: " <<
        poDataset->GetDriver()->GetDescription() <<
        poDataset->GetDriver()->GetMetadataItem( GDAL_DMD_LONGNAME ) ;
    qDebug() <<   "Size is " <<
        poDataset->GetRasterXSize() << poDataset->GetRasterYSize() <<
        poDataset->GetRasterCount() ;
    if( poDataset->GetProjectionRef()  != NULL )
        qDebug() <<   "Projection is " << poDataset->GetProjectionRef() ;
    if( poDataset->GetGeoTransform( adfGeoTransform ) == CE_None )
    {
        qDebug() <<   "Origin = (" << adfGeoTransform[0]<< "," << adfGeoTransform[3] << ")";
        qDebug() <<   "Pixel Size = (" << adfGeoTransform[1] << "," <<  adfGeoTransform[5] << ")";
    }

    /**
     * Fetch raster band
     */
    GDALRasterBand  *poBand;
    int             nBlockXSize, nBlockYSize;
    int             bGotMin, bGotMax;
    double          adfMinMax[2];
    poBand = poDataset->GetRasterBand( 1 );
    poBand->GetBlockSize( &nBlockXSize, &nBlockYSize );
    qDebug() <<  "Block=" << nBlockXSize << 'x' << nBlockYSize <<
        " Type=" << GDALGetDataTypeName(poBand->GetRasterDataType()) <<
        ", ColorInterp=" <<            GDALGetColorInterpretationName(
            poBand->GetColorInterpretation());

    adfMinMax[0] = poBand->GetMinimum( &bGotMin );
    adfMinMax[1] = poBand->GetMaximum( &bGotMax );
    if( ! (bGotMin && bGotMax) )
        GDALComputeRasterMinMax((GDALRasterBandH)poBand, TRUE, adfMinMax);
    qDebug() << "Min=" << adfMinMax[0] << ", Max=" << adfMinMax[1];
    if( poBand->GetOverviewCount() > 0 )
        qDebug() << "Band has" << poBand->GetOverviewCount()<< " overviews.";
    if( poBand->GetColorTable() != NULL )
        qDebug() << "Band has a color table with " <<poBand->GetColorTable()->GetColorEntryCount()<< " entries.";


    /**
     * Fetch raster band
     *
     * */

    float *pafScanline;
    int   nXSize = poBand->GetXSize();
    pafScanline = (float *) CPLMalloc(sizeof(float)*nXSize);
    /*CPLErr readErr = poBand->RasterIO( GF_Read, 0, 0, nXSize, 1,
                     pafScanline, nXSize, 1, GDT_Float32,
                     0, 0 );
    */
    QVector<float> data;


    // TODO REMOVE
    int count0 = 0;

    for(int x = 0; x < nBlockXSize; x++ ){
        CPLErr readErr = poBand->RasterIO( GF_Read, 0, x, nXSize, 1,
                                          pafScanline, nXSize, 1, GDT_Float32,
                                          0, 0 );
        if(readErr){
            qDebug() << "Read error!" << readErr;
        }

        // TODO change, replace with memcpy if too slow
        for(int y = 0; y < nBlockYSize; y++){
            data.push_back(pafScanline[y]);

            if (pafScanline[y] == 0)
                count0++;
        }
    }

    qDebug() << "Num zero:" << count0 << "total:" << nBlockXSize * nBlockYSize << data.size();


    qDebug() << std::max_element(data.cbegin(), data.cend())[0];

    Displacement *disp = new Displacement();
    disp->setData(data, nBlockXSize, nBlockYSize);

    return disp;
}
