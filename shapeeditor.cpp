#include "shapeeditor.h"
#include <iostream>

OGRLayer* ShapeEditor::shapefile_reader(const std::string path) {
    /*
     * Open a shapefile dbf
     * @param path: Path to shapefile dbf
     * @return layer: A layer object
     */

    //OGRRegisterAll();
    GDALDataset *poDataset;
    //GDALAllRegister();
    poDataset = (GDALDataset *) GDALOpen(path.c_str(), GA_ReadOnly);
    OGRLayer *layer {poDataset->GetLayer(0)};
    //OGRDataSource *poDS {OGRSFDriverRegistrar::Open(path.c_str(), FALSE)};
    //OGRLayer *layer {poDS->GetLayer(0)};
    return layer;
}

OGRLayer* ShapeEditor::create_demand_point_fields(OGRLayer *dp_layer) {
    /*
     * Add PON_HOMES and STREETNAME fields to demand points layer
     * @param dp_layer: The demand points layer to add fields to
     */
    OGRFieldDefn *pon_homes_field {nullptr};
    OGRFieldDefn *streetname_field {nullptr};
    pon_homes_field->SetName("PON_HOMES");
    pon_homes_field->SetType(OFTInteger);
    streetname_field->SetName("STREETNAME");
    streetname_field->SetType(OFTString);
    dp_layer->CreateField(pon_homes_field);
    dp_layer->CreateField(streetname_field);

    return dp_layer;
}

void ShapeEditor::process_demand_points(const std::string name_to_change, OGRLayer *in_layer) {
    /*
     * Process demand points
     * @param name_to_change: The attribute name to change to all caps
     * @param in_layer: An OGRLayer that contains the attribute name to change
     */
    unsigned field_idx {find_field_index(name_to_change, in_layer)};

    if (field_idx != NULL) {
        OGRFieldDefn *tmp_field_def {nullptr};
        tmp_field_def->SetName("tmp");
        tmp_field_def->SetType(OFTString);  // Make this dynamic to add ability to rename multiple fields
        in_layer->CreateField(tmp_field_def);

        for (OGRFeatureUniquePtr &feature : in_layer) {
            feature->SetField("tmp", feature->GetFieldAsString(field_idx));
            in_layer->SetFeature(feature.release());
        }
        in_layer->DeleteField(field_idx);

        // Add the uppercase field
        std::string upper_name {uppercase_string(name_to_change)};
        OGRFieldDefn *upper_name_field {nullptr};
        upper_name_field->SetName(upper_name.c_str());
        upper_name_field->SetType(OFTString);
        in_layer->CreateField(upper_name_field);

        in_layer->ResetReading();  // Restart reading layer at beginning

        // Populate new field
        for (OGRFeatureUniquePtr &feature : in_layer) {
            feature->SetField(upper_name.c_str(), "tmp");
            std::string streetname {};
            try {
                streetname =uppercase_string(feature->GetFieldAsString("streetname"));
            }  catch (...) {
                std::cout << "Error parsing streetname" << std::endl;
            }
            feature->SetField("STREETNAME", "streetname");
            feature->SetField("PON_HOMES", 1);
            in_layer->SetFeature(feature.release());
        }
    }
}

void ShapeEditor::process_access_points(OGRLayer *in_layer) {
    /*
     * Process access points
     * @param in_layer: The layer to process
     */

    OGRFieldDefn *type_defn {nullptr};
    type_defn->SetName("TYPE");
    type_defn->SetType(OFTString);
    type_defn->SetWidth(254);
    in_layer->CreateField(type_defn);

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

    OGRFieldDefn *type_defn {nullptr};
    type_defn->SetName("EXISTING");
    type_defn->SetType(OFTString);
    type_defn->SetWidth(254);
    in_layer->CreateField(type_defn);

    for (OGRFeatureUniquePtr &feature : in_layer) {
        feature->SetField("EXISTING", "T");
        in_layer->SetFeature(feature.release());
    }
}

void ShapeEditor::process_aerial_connections(OGRLayer *in_layer) {
    /*
     * Process aerial connections
     * @param in_layer: The layer to process
     */

    OGRFieldDefn *type_defn {nullptr};
    type_defn->SetName("LOCKED");
    type_defn->SetType(OFTString);
    type_defn->SetWidth(254);
    in_layer->CreateField(type_defn);

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

    OGRFieldDefn *type_defn {nullptr};
    type_defn->SetName("LOCKED");
    type_defn->SetType(OFTString);
    type_defn->SetWidth(254);
    in_layer->CreateField(type_defn);

    for (OGRFeatureUniquePtr &feature : in_layer) {
        feature->SetField("LOCKED", "T");
        in_layer->SetFeature(feature.release());
    }
}

unsigned ShapeEditor::find_field_index(const std::string field_name, OGRLayer *in_layer) {
    /*
     * Find the index of field
     * @param in_layer: The layer to search
     * @param field_name: Field to search for
     */
    unsigned field_idx {NULL};
    OGRFeatureDefn *lyr_def {in_layer->GetLayerDefn()};
    int field_count {lyr_def->GetFieldCount()};
    for (int idx {0}; idx <= field_count; idx ++) {
        OGRFieldDefn field_def {lyr_def->GetFieldDefn(idx)};
        std::string field_name {field_def.GetNameRef()};
        if (field_name == field_name) {
            field_idx = idx;  // This is the field index that will be changed
        }
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
