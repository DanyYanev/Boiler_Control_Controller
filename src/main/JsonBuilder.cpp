#include "JsonBuilder.h"

JsonBuilder::JsonBuilder(String token, String controller_token){
    this->token = token;
    this->controller_token = controller_token;
    
    alternated = false;
    root = &jsonBuffer.createObject();
    values_attributes = &root->createNestedArray("values_attributes");
    root->operator[]("token") = token;
    root->operator[]("controller_token") = controller_token;
}

void JsonBuilder::add(String key, String value){
    alternated = true;
 
    bool added = false;
    for(auto value_object : *values_attributes){
        if(key == String(value_object["key"].as<const char*>())){
            value_object["value"] = value;
            added = true;
            break;
        }
    }

    if(!added){
        JsonObject& value_object = values_attributes->createNestedObject();
        value_object["key"] = key;
        value_object["value"] = value;
    }
}

void JsonBuilder::flush(){
    alternated = false;
    jsonBuffer.clear();
    root = &jsonBuffer.createObject();
    values_attributes = &root->createNestedArray("values_attributes");
    root->operator[]("token") = token;
    root->operator[]("controller_token") = controller_token;
}

String JsonBuilder::getJson(){
    String result;
    root->printTo(result);
    return result;
}

bool JsonBuilder::getAlternated(){
    return alternated;
}

void JsonBuilder::setAlternated(bool status){
  alternated = status;
}

