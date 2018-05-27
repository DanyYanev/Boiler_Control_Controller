#ifndef JSONBUILDER_H
#define JSONBUILDER_H

#include <Arduino.h>
#include <ArduinoJson.h>

class JsonBuilder{
    public:
        JsonBuilder(String, String);
        void add(String, String);
        bool getAlternated();
        String getJson();
        void flush();

    private:
        bool alternated;
        StaticJsonBuffer<500> jsonBuffer;
        JsonObject * root;
        JsonArray * values_attributes;
        String token;
        String controller_token;
};

#endif // JSONBUILDER_H
