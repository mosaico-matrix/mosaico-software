#ifndef CONFIGS_CPP
#define CONFIGS_CPP

#include <string>

static const std::string PROJECT_ROOT = "./";

class Configs {

private:

public:

    static std::string getProjectRoot() {
        return PROJECT_ROOT;
    }

    static std::string getPythonScriptPath() {
        return Configs::getProjectRoot() + "../networking";
    }

    static std::string getPythonStreamPath() {
        return Configs::getProjectRoot() + "../stream";
    }

//    static std::string getWidgetsPath()
//    {
//        return Configs::getProjectRoot() + "
//    }

    static std::string getFontsPath() {
        return Configs::getProjectRoot() + "matrix/drawables/fonts/";
    }

    static std::string getFontPath(const std::string &fontName) {
        return Configs::getFontsPath() + fontName;
    }

    static std::string getImagesPath() {
        return Configs::getProjectRoot() + "assets/images/";
    }

    static std::string getImagePath(const std::string &imageName) {
        return Configs::getImagesPath() + imageName;
    }

    static std::string getAnimationsPath() {
        return Configs::getProjectRoot() + "assets/images/animations/";
    }

    static std::string getAnimationPath(const std::string &animationName) {
        return Configs::getAnimationsPath() + animationName;
    }
};

#endif