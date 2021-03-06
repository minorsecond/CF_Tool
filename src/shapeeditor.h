#ifndef SHAPEEDITOR_H
#define SHAPEEDITOR_H

#include <gdal_priv.h>
#include <ogrsf_frmts.h>

const std::map<int, int> utm_zones {
    {10, 26910},
    {11, 26911},
    {12, 26912},
    {13, 26913},
    {14, 26914},
    {15, 26915},
    {16, 26916},
    {17, 26917},
    {18, 26918},
    {19, 26919}
};

class ShapeEditor
{
public:
    GDALDriverH gdaldriver {GDALGetDriverByName("ESRI Shapefile")};
    static OGRLayer* shapefile_reader(const std::string path);
    static void process_demand_points(OGRLayer *in_layer);
    static void process_access_points(OGRLayer *in_layer);
    static void process_poles(OGRLayer *in_layer);
    static void process_aerial_connections(OGRLayer *in_layer);
    static void process_fdt_boundaries(OGRLayer *in_layer);
    static void reproject(OGRLayer *in_layer, const int utm_zone, const std::string &path);
    ShapeEditor();

private:
    static OGRLayer* create_demand_point_fields(OGRLayer *dp_layer);
    static void create_tmp_include_field(const int include_field_idx, OGRLayer *in_layer);
    static void create_tmp_pon_homes_field(OGRLayer *in_layer, int &pon_homes_idx, bool &all_zero);
    static int find_street_name_field(OGRLayer *in_layer);
    static void add_dp_fields(OGRLayer *in_layer, const int include_field_idx, const bool override_pon_homes, const int pon_homes_field_idx, const int streetname_idx);
    static void delete_tmp_dp_fields(OGRLayer *in_layer, int include_field_idx, int pon_homes_field_idx);
    static int find_field_index (const std::string field_name, OGRLayer *in_layer);
};

#endif // SHAPEEDITOR_H
