#ifndef SHAPEEDITOR_H
#define SHAPEEDITOR_H

#include <gdal_priv.h>
#include <ogrsf_frmts.h>

class ShapeEditor
{
public:
    GDALDriverH gdaldriver {GDALGetDriverByName("ESRI Shapefile")};
    static OGRLayer* shapefile_reader(const std::string path);
    static OGRLayer* create_demand_point_fields(OGRLayer *dp_layer);
    static void process_demand_points(const std::string name_to_change, OGRLayer *in_layer);
    static void process_access_points(OGRLayer *in_layer);
    static void process_poles(OGRLayer *in_layer);
    static void process_aerial_connections(OGRLayer *in_layer);
    static void process_fdt_boundaries(OGRLayer *in_layer);
    static void reproject(OGRLayer *in_layer, int utm_zone);
    static int find_field_index (const std::string field_name, OGRLayer *in_layer);
    static std::string uppercase_string(std::string input_string);
    ShapeEditor();
};

#endif // SHAPEEDITOR_H
