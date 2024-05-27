//#ifndef ANIMATION_DRAWER_CPP
//#define ANIMATION_DRAWER_CPP
//
//#include "drawable.cpp"
//#include "ppm-drawer.cpp"
//#include <dirent.h>
//#include <vector>
//#include <string>
//
//class AnimationDrawer : public Drawable {
//private:
//    std::vector<PPMDrawer*> ppmDrawers; // Store PPMDrawer objects for each frame
//    size_t currentFrameIndex; // Keep track of the current frame index
//
//public:
//    AnimationDrawer(const char *dirName) : Drawable(), currentFrameIndex(0) {
//        // Load all PPM files from the directory
//        DIR *dir;
//        struct dirent *ent;
//
//        std::string animationDirPath = Configs::getAnimationPath(dirName).c_str();
//
//        // Open the directory
//        if ((dir = opendir(animationDirPath.c_str())) != NULL) {
//
//            // Iterate through files in the directory
//            while ((ent = readdir(dir)) != NULL) {
//                std::string filename = ent->d_name;
//
//                // Check if the file ends with ".ppm"
//                if (filename.length() >= 4 && filename.substr(filename.length() - 4) == ".ppm") {
//                    // Create a PPMDrawer for each PPM file and store it in the vector
//                    PPMDrawer *drawer = new PPMDrawer(("animations/" +std::string(dirName)+ "/"+filename).c_str());
//                    drawer->setCanvas(c);
//                    ppmDrawers.push_back(drawer);
//                }
//            }
//            closedir(dir);
//        }
//    }
//
//    void drawNextFrame() {
//        if (ppmDrawers.empty()) return;
//
//        // Render the current frame using PPMDrawer
//        ppmDrawers[currentFrameIndex]->draw();
//
//        // Move to the next frame
//        currentFrameIndex = (currentFrameIndex + 1) % ppmDrawers.size();
//    }
//
//    ~AnimationDrawer() {
//        for (PPMDrawer *drawer : ppmDrawers) {
//            delete drawer;
//        }
//    }
//
//private:
//};
//
//#endif
