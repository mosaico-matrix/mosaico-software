#pragma once

#include <vector>
#include "../../../external/json/json.hpp"
#include <iostream>


using json = nlohmann::json;

struct InputField {
    std::string name;
    std::string type;
    bool required;
    json defaultValue;
};

struct UserInput {
    bool nameable;
    std::vector<InputField> formTemplate;
};

struct DynamicRunnerMetadata {
    std::string author;
    std::string name;
    double version;
    int fps;
    bool passive;
    UserInput userInput;

    void dump()
    {
        std::cout << "author: " << author << std::endl;
        std::cout << "name: " << name << std::endl;
        std::cout << "version: " << version << std::endl;
        std::cout << "fps: " << fps << std::endl;
        std::cout << "passive: " << passive << std::endl;
        std::cout << "nameable: " << userInput.nameable << std::endl;
        std::cout << "form_template: " << std::endl;
        for (auto& field : userInput.formTemplate) {
            std::cout << "  name: " << field.name << std::endl;
            std::cout << "  type: " << field.type << std::endl;
            std::cout << "  required: " << field.required << std::endl;
            std::cout << "  default: " << field.defaultValue << std::endl;
        }
    }
};

inline void from_json(const json& j, InputField& inputField) {
    j.at("name").get_to(inputField.name);
    j.at("type").get_to(inputField.type);
    j.at("required").get_to(inputField.required);
    if (j.find("default") != j.end()) {
        j.at("default").get_to(inputField.defaultValue);
    }
}

inline void from_json(const json& j, UserInput& userInput) {
    j.at("nameable").get_to(userInput.nameable);
    j.at("form_template").get_to(userInput.formTemplate);
}

inline void from_json(const json& j, DynamicRunnerMetadata& metadata) {
    j.at("author").get_to(metadata.author);
    j.at("name").get_to(metadata.name);
    j.at("version").get_to(metadata.version);
    j.at("fps").get_to(metadata.fps);
    j.at("passive").get_to(metadata.passive);
    j.at("user_input").get_to(metadata.userInput);
}
