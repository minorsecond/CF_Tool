#ifndef SHAPEEDITOR_H
#define SHAPEEDITOR_H

#include <gdal_priv.h>
#include <ogrsf_frmts.h>

class ShapeEditor
{
public:
    GDALDriverH gdaldriver {GDALGetDriverByName("ESRI Shapefile")};
    OGRDataSource* shapefile_reader(const std::string path);
    ShapeEditor();
};

#endif // SHAPEEDITOR_H
