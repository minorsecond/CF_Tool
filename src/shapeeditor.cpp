#include "shapeeditor.h"
#include "ui/errorwindow.h"
#include <iostream>
#include <filesystem>
#include <ogr_feature.h>
#include <gdal.h>

ShapeEditor::ShapeEditor() {};

OGRLayer* ShapeEditor::shapefile_reader(const std::string path) {
    /*
     * Open a shapefile dbf
     * @param path: Path to shapefile dbf
     * @return layer: A layer object
     */

    GDALDataset *poDataset;
    std::cout << "Reading layer from shapefile " + path << std::endl;
    GDALAllRegister();
   // poDataset = (GDALDataset *) GDALOpen(path.c_str(), GA_ReadOnly);
    poDataset = (GDALDataset *) GDALOpenEx(path.c_str(), GDAL_OF_ALL | GDAL_OF_UPDATE, NULL, NULL, NULL);
    OGRLayer *layer {poDataset->GetLayer(0)};
    //OGRLayer *layer {nullptr};
    return layer;
}

OGRLayer* ShapeEditor::create_demand_point_fields(OGRLayer *dp_layer) {
    /*
     * Add PON_HOMES and STREETNAME fields to demand points layer
     * @param dp_layer: The demand points layer to add fields to
     */
    OGRFieldDefn pon_homes_field("PON_HOMES", OFTInteger);
    OGRFieldDefn streetname_field("STREETNAME", OFTString);
    dp_layer->CreateField(&pon_homes_field);
    dp_layer->CreateField(&streetname_field);

    return dp_layer;
}

void ShapeEditor::process_demand_points(const std::string name_to_change, OGRLayer *in_layer) {
    /*
     * Process demand points
     * @param name_to_change: The attribute name to change to all caps
     * @param in_layer: An OGRLayer that contains the attribute name to change
     */
    ErrorWindow er;
    const std::string input_streetname_field_name {"street_nam"};
    int field_idx {find_field_index(name_to_change, in_layer)};
    std::cout << "Index: " << field_idx << std::endl;

    if (field_idx == -1) {
        er.set_error_message("Error: could not find " + name_to_change + " in demand points layer. Defaulting to True.");
        er.exec();
    }

    if (field_idx != -1) {
        OGRFieldDefn tmp_field_def("tmp", OFTString);
        in_layer->CreateField(&tmp_field_def);

        for (OGRFeatureUniquePtr &feature : in_layer) {
            std::string include_attr {feature->GetFieldAsString(field_idx)};
            std::cout << include_attr << std::endl;
            feature->SetField("tmp", feature->GetFieldAsString(field_idx));
            in_layer->SetFeature(feature.release());
        }
        std::cout << "Deleting include field at index " << field_idx << std::endl;
        in_layer->DeleteField(field_idx);
    }

    // Find street name field
    int streetname_idx {find_field_index("street_nam", in_layer)};
    if (streetname_idx == -1) {
        streetname_idx = find_field_index("street", in_layer);
        if (streetname_idx == -1) {
            er.set_error_message("Warning: could not find streetname field");
            er.exec();
        }
    }

    // Add the uppercase field
    std::string upper_name {uppercase_string(name_to_change)};
    OGRFieldDefn upper_name_field(upper_name.c_str(), OFTString);
    in_layer->CreateField(&upper_name_field);

    in_layer->ResetReading();  // Restart reading layer at beginning
    std::string tmp_value {};

    // Populate new field
    for (OGRFeatureUniquePtr &feature : in_layer) {
        if (field_idx != -1) {
            tmp_value = feature->GetFieldAsString("tmp");
        } else {
            tmp_value = "T";
        }
        feature->SetField(upper_name.c_str(), tmp_value.c_str());

        std::string streetname {};
        if (streetname_idx != -1) {
            streetname = uppercase_string(feature->GetFieldAsString(streetname_idx));
        } else {
            streetname = "UNKNOWN";
        }

        feature->SetField("STREETNAME", streetname.c_str());
        feature->SetField("PON_HOMES", 1);
        in_layer->SetFeature(feature.release());
    }

    if (field_idx != -1) {
        int tmp_index {find_field_index("tmp", in_layer)};
        if (tmp_index != -1) {  // Just in case we can't find the tmp attribute
            in_layer->DeleteField(find_field_index("tmp", in_layer));
        }
    }
}

void ShapeEditor::process_access_points(OGRLayer *in_layer) {
    /*
     * Process access points
     * @param in_layer: The layer to process
     */
    ErrorWindow er;
    OGRFieldDefn type_defn("TYPE", OFTString);
    type_defn.SetWidth(254);
    in_layer->CreateField(&type_defn);

    if (find_field_index("structur_1", in_layer) == -1) {
        er.set_error_message("Error: Could not find structur_1 attribute in access_points layer.");
        er.exec();
        return;
    }

    for (OGRFeatureUniquePtr &feature : in_layer) {
        const std::string type {feature->GetFieldAsString("structur_1")};  // TODO: Handle different attribute names
        const std::string new_type {"HANDHOLE{" + type + "}"};
        feature->SetField("TYPE", new_type.c_str());
        in_layer->SetFeature(feature.release());
    }
}

void ShapeEditor::process_poles(OGRLayer *in_layer) {
    /*
     * Process poles
     * @param in_layer: The layer to process
     */

    OGRFieldDefn type_defn("EXISTING", OFTString);
    type_defn.SetWidth(254);
    in_layer->CreateField(&type_defn);
    bool include_field_exists {false};
    int include_field_id {-1};

    // Try to find field ID of index attribute, if it exists
    if (find_field_index("include", in_layer) != -1) {
            include_field_exists = true;
            include_field_id = find_field_index("index", in_layer);
            std::cout << "Found Poles include field" << std::endl;
    } else if (find_field_index("INCLUDE", in_layer) != -1) {
            include_field_exists = true;
            include_field_id = find_field_index("INCLUDE", in_layer);
            std::cout << "Found poles INCLUDE field" << std::endl;
    } else if (find_field_index("Include", in_layer) != -1) {
            include_field_exists = true;
            include_field_id = find_field_index("Include", in_layer);
            std::cout << "Found poles Include field" << std::endl;
    }

    for (OGRFeatureUniquePtr &feature : in_layer) {
        bool deleted_feature {false};
        if (include_field_exists == true) {
            std::string attr_value {feature->GetFieldAsString(include_field_id)};
            std::transform(attr_value.begin(), attr_value.end(), attr_value.begin(),
                                                  [](unsigned char c){return std::tolower(c); });
            if (attr_value == "false") {
                std::cout << "Deleting feature" << std::endl;
                auto delete_result {in_layer->DeleteFeature(feature->GetFID())};
                delete_result = true;
            }
        }

        if (!deleted_feature) {
            feature->SetField("EXISTING", "T");
            in_layer->SetFeature(feature.release());
        }
    }
}

void ShapeEditor::process_aerial_connections(OGRLayer *in_layer) {
    /*
     * Process aerial connections
     * @param in_layer: The layer to process
     */

    OGRFieldDefn type_defn("LOCKED", OFTString);
    type_defn.SetWidth(254);
    in_layer->CreateField(&type_defn);

    for (OGRFeatureUniquePtr &feature : in_layer) {
        feature->SetField("LOCKED", "T");
        in_layer->SetFeature(feature.release());
    }
}

void ShapeEditor::process_fdt_boundaries(OGRLayer *in_layer) {
    /*
     * Process FDT Boundaries
     * @param in_layer: The layer to process
     */

    OGRFieldDefn type_defn("LOCKED", OFTString);
    type_defn.SetWidth(254);
    in_layer->CreateField(&type_defn);

    for (OGRFeatureUniquePtr &feature : in_layer) {
        feature->SetField("LOCKED", "T");
        in_layer->SetFeature(feature.release());
    }
}

int ShapeEditor::find_field_index(const std::string field_name, OGRLayer *in_layer) {
    /*
     * Find the index of field
     * @param in_layer: The layer to search
     * @param field_name: Field to search for
     */
    int field_idx {NULL};
    bool field_found {false};
    OGRFeatureDefn *lyr_def {in_layer->GetLayerDefn()};
    int field_count {lyr_def->GetFieldCount()};
    std::cout << "Field count: " << field_count << std::endl;
    for (int idx {0}; idx < field_count; idx ++) {
        OGRFieldDefn field_def {lyr_def->GetFieldDefn(idx)};
        std::string shp_name {field_def.GetNameRef()};
        if (shp_name == field_name) {
            field_idx = idx;  // This is the field index that will be changed
            field_found = true;
        }
    }

    if (field_found == false) {
        return -1;
    }
    return field_idx;
}

std::string ShapeEditor::uppercase_string(std::string input_string) {
    /*
     * Convert string to an uppercase string
     * @param input_string: Input string
     * @return string: Input string as all uppercase
     */

    std::string uppercase_string {input_string};
    for (int i {0}; i < uppercase_string.size(); i++) {
        uppercase_string[i] = std::toupper(uppercase_string[i]);
    }
    return uppercase_string;
}

void ShapeEditor::reproject(OGRLayer *in_layer, int utm_zone, std::string path) {
    /*
     * Reproject the layer.
     * @param in_layer: The layer to reproject
     * @param utm_zone: The UTM zone to reproject to
     */

    const char *pszDriverName {"ESRI Shapefile"};
    GDALAllRegister();
    GDALDriver *poDriver {nullptr};
    poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName);

    // Convert UTM zone integer into the EPSG code. This map will be updated
    // as new areas become active. the N specifier, e.g. 10N, is taken for
    // granted.
    std::map<int, int> utm_zones {
        {10, 26910},
        {11, 32611},
        {16, 32616}
    };

    // Proj library path
    const std::string ppath {std::filesystem::current_path().string()};
    const char * proj_path[] {ppath.c_str(), nullptr};
    std::cout << "Searching for proj.db in " << ppath << std::endl;
    OSRSetPROJSearchPaths(proj_path);

    int crs {};
    for (auto it {utm_zones.begin()}; it != utm_zones.end(); it++) {
        if (it->first == utm_zone) {
            crs = it->second;
        }
    }

    OGRSpatialReference *srFrom {in_layer->GetSpatialRef()};
    OGRSpatialReference *srTo = new OGRSpatialReference;

    std::cout << "Converting to EPSG: " << crs << std::endl;
    srTo->importFromEPSG(crs);
    OGRCoordinateTransformation *coordTrans {OGRCreateCoordinateTransformation(srFrom, srTo)};

    // Create new layer
    GDALDataset *poDS {nullptr};
    poDS = poDriver->Create(path.c_str(), 0, 0, 0, GDT_Unknown, NULL);
    OGRLayer *poLayer {nullptr};
    poLayer = poDS->CreateLayer("Reprojected", srTo, wkbUnknown, NULL);

    for (OGRFeatureUniquePtr &feature : in_layer) {
        OGRGeometry *transformed {feature->GetGeometryRef()};
              transformed->transform(coordTrans);
              feature->SetGeometry(transformed);
              poLayer->CreateFeature(feature.get()->Clone());
              poLayer->SetFeature(feature.release());
    }

    // Cleanup
    poLayer->SyncToDisk();
    delete poLayer;
    delete poDS;
    delete coordTrans;
    delete srTo;
}
