# Geo visualization waterlining

## Installation
1. Install Qt, instructions [here](https://www.qt.io/download-qt-installer-oss)
2. Install GDAL:
     On ubuntu using this command: `sudo aptitude install gdal-bin libgdal-dev`
     On windows by installing OSGeo4W -> advanced install, add GDAL and GDAL-devel packages
     

## Data conversion
`cd testing`
`gdaldem color-relief  ../data/6602_1_10m_z32.dem ./color.txt output2.png`