#pragma once

#include <vector>
#include "../../../external/json/json.hpp"
#include <iostream>

using json = nlohmann::json;

struct DynamicWidgetMetadata {

    std::string author;
    std::string name;
    std::string version;
    std::string software_version;
    int fps;

    void dump()
    {
        std::cout << "author: " << author << std::endl;
        std::cout << "name: " << name << std::endl;
        std::cout << "version: " << version << std::endl;
        std::cout << "fps: " << fps << std::endl;
    }

};

inline void from_json(const json& j, DynamicWidgetMetadata& metadata) {
    j.at("author").get_to(metadata.author);
    j.at("name").get_to(metadata.name);
    j.at("version").get_to(metadata.version);
    j.at("fps").get_to(metadata.fps);
}
