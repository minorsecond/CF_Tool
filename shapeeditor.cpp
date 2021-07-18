#include "shapeeditor.h"

OGRLayer* ShapeEditor::shapefile_reader(const std::string path) {
    /*
     * Open a shapefile dbf
     * @param path: Path to shapefile dbf
     * @return layer: A layer object
     */

    //OGRRegisterAll();
    GDALDataset *poDataset;
    GDALAllRegister();
    poDataset = (GDALDataset *) GDALOpen(path.c_str(), GA_ReadOnly);
    OGRLayer *layer {poDataset->GetLayer()};
    //OGRDataSource *poDS {OGRSFDriverRegistrar::Open(path.c_str(), FALSE)};
    //OGRLayer *layer {poDS->GetLayer(0)};
    return layer;
}
