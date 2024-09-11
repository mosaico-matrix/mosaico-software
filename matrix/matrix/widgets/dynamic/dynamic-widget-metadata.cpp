#pragma once

#include <vector>
#include "../../../external/json/json.hpp"
#include <iostream>

using json = nlohmann::json;

struct DynamicWidgetMetadata {

    std::string name;
    std::string widget_version;
    std::string software_version;
    int fps;

    void dump()
    {
        std::cout << "name: " << name << std::endl;
        std::cout << "widget_version: " << widget_version << std::endl;
        std::cout << "software_version: " << software_version << std::endl;
        std::cout << "fps: " << fps << std::endl;
    }

};

inline void from_json(const json& j, DynamicWidgetMetadata& metadata) {
    j.at("name").get_to(metadata.name);
    j.at("widget_version").get_to(metadata.widget_version);
    j.at("software_version").get_to(metadata.software_version);
    j.at("fps").get_to(metadata.fps);
}
