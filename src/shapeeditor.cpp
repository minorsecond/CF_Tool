#include "shapeeditor.h"
#include "ui/errorwindow.h"
#include "ui/overrideponhomes.h"

#include <iostream>
#include <filesystem>
#include <ogr_feature.h>
#include <gdal.h>
#include <memory>

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
    poDataset = (GDALDataset *) GDALOpenEx(path.c_str(), GDAL_OF_ALL | GDAL_OF_UPDATE, NULL, NULL, NULL);
    OGRLayer *layer {poDataset->GetLayer(0)};
    //TODO: Figure out why poDataset can't be deleted

    return layer;
}

OGRLayer* ShapeEditor::create_demand_point_fields(OGRLayer *dp_layer) {
    /*
     * Add PON_HOMES and STREETNAME fields to demand points layer
     * @param dp_layer: The demand points layer to add fields to
     */
    //OGRFieldDefn pon_homes_field("PON_HOMES", OFTInteger);
    OGRFieldDefn streetname_field("STREETNAME", OFTString);
    //dp_layer->CreateField(&pon_homes_field);
    dp_layer->CreateField(&streetname_field);

    return dp_layer;
}

void ShapeEditor::process_demand_points(OGRLayer *in_layer) {
    /*
     * Process demand points
     * @param name_to_change: The attribute name to change to all caps
     * @param in_layer: An OGRLayer that contains the attribute name to change
     */

    /*
     * All field searches assume that CGIS provides attribute names in lowercase, which has
     * been the case so far. If they begin also providing them in uppercase, we'll need to
     * add nested if/else statements to try to determine if the field exists in whichever
     * case.
     */

    ErrorWindow er;
    const std::string input_streetname_field_name {"street_nam"};
    const int include_field_idx {find_field_index("include", in_layer)};  // Find include field. Returns -1 if "include" isn't present
    int pon_homes_field_idx {-1};
    bool all_pon_homes_zero {true};
    bool override_pon_homes {false};

    if (include_field_idx == -1) {
        er.set_error_message("Error: could not find include field in demand points layer. Defaulting to True.");
        er.exec();
    }

    // Create tmp field to store original include field and populate it with original include values
    // This is done to switch from the lowercase "include" field to uppercase "INCLUDE"
    create_tmp_include_field(include_field_idx, in_layer);

    // Create tmp field to store original pon_homes values, just as we did for the inlude field
    create_tmp_pon_homes_field(in_layer, pon_homes_field_idx, all_pon_homes_zero);

    // Prompt user if they want to set PON_HOMES to 1 if CGIS has set them all to 0.
    if (all_pon_homes_zero && pon_homes_field_idx != -1) {
        OverridePonHomes override = OverridePonHomes();
        override.setModal(true);
        if (override.exec() == QDialog::Accepted) {
            override_pon_homes = true;
        }
    }

    int streetname_idx {find_street_name_field(in_layer)};
    add_dp_fields(in_layer, include_field_idx, override_pon_homes, pon_homes_field_idx, streetname_idx);

    delete_tmp_dp_fields(in_layer, include_field_idx, pon_homes_field_idx);
}

void ShapeEditor::create_tmp_include_field(const int include_field_idx, OGRLayer *in_layer) {
    /*
     * Create tmp field to store original include field and populate it with original include values
     * This is done to switch from the lowercase "include" field to uppercase "INCLUDE"
     */

    if (include_field_idx != -1) {
        OGRFieldDefn tmp_include_field_def("tmp_inc", OFTString);
        in_layer->CreateField(&tmp_include_field_def);

        for (OGRFeatureUniquePtr &feature : in_layer) {
            const std::string include_attr {feature->GetFieldAsString(include_field_idx)};
            std::cout << "Include attr: " << include_attr << std::endl;
            feature->SetField("tmp_inc", include_attr.c_str());
            in_layer->SetFeature(feature.release());
        }
        std::cout << "Deleting include field at index " << include_field_idx << std::endl;
        in_layer->DeleteField(include_field_idx);  // Delete original include field
    }
}

void ShapeEditor::create_tmp_pon_homes_field(OGRLayer *in_layer, int &pon_homes_idx, bool &all_zero) {
    /* Create tmp pon_homes field and set pon_homes idx and all_zero
     * @param in_layer: The layer to modify (create tmp pon homes field in)
     * @param pon_homes_idx: The column number of the pon_homes attribute
     * @param all_zero: a bool denoting whether all pon_home values are 0
     */

    pon_homes_idx = find_field_index("pon_homes", in_layer);  // Find pon_homes field

    if (pon_homes_idx != -1) {
        OGRFieldDefn tmp_pon_homes_field_defn("tmp_ph", OFTInteger);
        in_layer->CreateField(&tmp_pon_homes_field_defn);

        for (OGRFeatureUniquePtr &feature : in_layer) {
            const int ph_attr {feature->GetFieldAsInteger(pon_homes_idx)};
            if (ph_attr == 1) {
                all_zero = false;
            }

            feature->SetField("tmp_ph", ph_attr);
            in_layer->SetFeature(feature.release());
        }
        std::cout << "Deleting pon_homes field at index " << pon_homes_idx << std::endl;
        in_layer->DeleteField(pon_homes_idx);
    }

}

int ShapeEditor::find_street_name_field(OGRLayer *in_layer) {
    /*
     * Find the streetname field
     * @param in_layer: The layer to search
     */

    std::array<std::string, 2> streetname_attr_names {"street_nam", "street"};
    int streetname_idx {-1};
    for (std::string attr_name : streetname_attr_names) {
        if (streetname_idx == -1) {
            streetname_idx = find_field_index(attr_name, in_layer);
        }
    }

    return streetname_idx;
}

void ShapeEditor::add_dp_fields(OGRLayer *in_layer, const int include_field_idx, const bool override_pon_homes, const int pon_homes_field_idx, const int streetname_idx) {
    /*
     * Add new fields to demand points layer
     * @param in_layer: The layer to add fields to
     */

    OGRFieldDefn new_include_Field("INCLUDE", OFTString);
    OGRFieldDefn new_pon_homes_field("PON_HOMES", OFTString);
    OGRFieldDefn streetname_field("STREETNAME", OFTString);
    in_layer->CreateField(&new_include_Field);
    in_layer->CreateField(&new_pon_homes_field);
    in_layer->CreateField(&streetname_field);


    in_layer->ResetReading();  // Restart reading layer at beginning

    // Populate new field
    for (OGRFeatureUniquePtr &feature : in_layer) {
        std::string tmp_include_value {};
        int tmp_ph_value {};

        // Populate new INCLUDE field with values in tmp_include field
        if (include_field_idx != -1) {
            tmp_include_value = feature->GetFieldAsString("tmp_inc");
            std::cout << "TMP INCLUDE VAL: " << tmp_include_value << std::endl;
        } else {
            tmp_include_value = "T";
        }
        feature->SetField("INCLUDE", tmp_include_value.c_str());

        // Populate new PON_HOMES field with values in tmp_pon_homes field
        if (override_pon_homes) {
            tmp_ph_value = 1;
        } else if (pon_homes_field_idx != -1) {
            tmp_ph_value = feature->GetFieldAsInteger("tmp_ph");
        } else {  // If we can't find a pon_homes field from CGIS
            tmp_ph_value = 1;
        }
        feature->SetField("PON_HOMES", tmp_ph_value);

        // Here, we convert streetnames to uppercase, or set the field to UNKNOWN if
        // the CGIS streetname attribute doesn't exist.
        std::string streetname {};
        if (streetname_idx != -1) {
            streetname = feature->GetFieldAsString(streetname_idx);
            std::transform(streetname.begin(), streetname.end(), streetname.begin(), [] (unsigned char c) {return std::toupper(c);});
        } else {
            streetname = "UNKNOWN";
        }

        feature->SetField("STREETNAME", streetname.c_str());
        in_layer->SetFeature(feature.release());
    }
}

void ShapeEditor::delete_tmp_dp_fields(OGRLayer *in_layer, int include_field_idx, int pon_homes_field_idx) {
    /*
     * Delete temporary demand point fields.
     * @param in_layer: The layer to delete from
     */

    // Find & delete tmp include field.
    if (include_field_idx != -1) {
        const int tmp_index {find_field_index("tmp_inc", in_layer)};
        if (tmp_index != -1) {  // Just in case we can't find the tmp_include attribute
            in_layer->DeleteField(find_field_index("tmp_inc", in_layer));
        }
    }

    // Find & delete tmp pon_homes field.
    if (pon_homes_field_idx != -1) {
        const int tmp_index {find_field_index("tmp_ph", in_layer)};
        if (tmp_index != -1) {
            in_layer->DeleteField(tmp_index);
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
    const int structur2_idx {find_field_index("structur_2", in_layer)};

    if (find_field_index("structur_1", in_layer) == -1) {
        er.set_error_message("Error: Could not find structur_1 attribute in access_points layer.");
        er.exec();
        return;
    }

    // Loop through AP features and add the TYPE attribute.
    for (OGRFeatureUniquePtr &feature : in_layer) {
        std::string size {};
        std::string new_type {};
        const std::string type {feature->GetFieldAsString("structur_1")};  // TODO: Handle different attribute names
        if (structur2_idx != -1) {
            size = feature->GetFieldAsString(structur2_idx);  // Get size value from structur2 field.
        }

        if (!size.empty()) {
            new_type = "HANDHOLE{" + type + '-' + size + '}';
        } else {
            new_type = "HANDHOLE{" + type + '}';
        }
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

    /*
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
    */

    for (OGRFeatureUniquePtr &feature : in_layer) {
        /*
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
        */

        feature->SetField("EXISTING", "T");
        in_layer->SetFeature(feature.release());
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
     * Find the index of field. Returns -1 if field could not be found.
     * @param in_layer: The layer to search
     * @param field_name: Field to search for
     */
    int field_idx {NULL};
    bool field_found {false};
    OGRFeatureDefn *lyr_def {in_layer->GetLayerDefn()};
    const int field_count {lyr_def->GetFieldCount()};
    std::cout << "Field count: " << field_count << std::endl;
    for (int idx {0}; idx < field_count; idx ++) {
        OGRFieldDefn field_def {lyr_def->GetFieldDefn(idx)};
        const std::string shp_name {field_def.GetNameRef()};
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

void ShapeEditor::reproject(OGRLayer *in_layer, const int utm_zone, const std::string &path) {
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
    // as new areas become active. the N specifier, e.g. 10N, is assumed.

    // Proj library path
    const std::string ppath {std::filesystem::current_path().string()};
    const char * proj_path[] {ppath.c_str(), nullptr};
    std::cout << "Searching for proj.db in " << ppath << std::endl;
    OSRSetPROJSearchPaths(proj_path);

    // Convert UTM zone string into CRS integer for GDAL
    int crs {};
    for (auto it {utm_zones.begin()}; it != utm_zones.end(); it++) {
        if (it->first == utm_zone) {
            crs = it->second;
        }
    }

    // Get projection data
    OGRSpatialReference *srFrom {in_layer->GetSpatialRef()};
    auto srTo {std::make_unique<OGRSpatialReference>()};

    std::cout << "Converting to EPSG: " << crs << std::endl;
    srTo->importFromEPSG(crs);
    OGRCoordinateTransformation *coordTrans {OGRCreateCoordinateTransformation(srFrom, srTo.get())};

    // Create new layer
    GDALDataset *poDS {nullptr};
    poDS = poDriver->Create(path.c_str(), 0, 0, 0, GDT_Unknown, NULL);
    OGRLayer *poLayer {nullptr};
    poLayer = poDS->CreateLayer("Reprojected", srTo.get(), wkbUnknown, NULL);
    auto lyr_def {in_layer->GetLayerDefn()};

    // Add attribute table
    for (int i {0}; i < lyr_def->GetFieldCount(); i++) {
        poLayer->CreateField(lyr_def->GetFieldDefn(i));
    }

    // Add reprojected geometry
    for (OGRFeatureUniquePtr &feature : in_layer) {
        OGRGeometry *transformed {feature->GetGeometryRef()};
        transformed->transform(coordTrans);
        feature->SetGeometry(transformed);
        poLayer->CreateFeature(feature.get()->Clone());
        poLayer->SetFeature(feature.release());
    }
    // Cleanup
    delete coordTrans;
    poLayer->SyncToDisk();
    GDALClose(poDS);
}
