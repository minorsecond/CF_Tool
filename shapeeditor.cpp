#include "shapeeditor.h"

ShapeEditor::ShapeEditor()
{

}

OGRDataSource* ShapeEditor::shapefile_reader(const std::string path) {
    /*
     * Open a shapefile
     * @param path: Path to shapefile
     */

    OGRRegisterAll();
    OGRDataSource *poDS {OGRSFDriverRegistrar::Open(path.c_str(), FALSE)};
    return poDS;
}
