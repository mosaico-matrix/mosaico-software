//#ifndef DRAWABLE_TEXT_H
//#define DRAWABLE_TEXT_H
//
//#include "drawable.h"
//#include <vector>
//#include <string>
//#include <map>
//
//enum TextScrollingSpeed {
//    SLOW = 10000,
//    MEDIUM = 5000,
//    FAST = 1000
//};
//
//class DrawableText : public Drawable {
//public:
//    DrawableText();
//
//    void setText(const std::string text);
//
//    void setFontHeight(int fontHeight);
//
//    void setScrollingSpeed(TextScrollingSpeed scrollingSpeed);
//
//    static void loadFonts();
//
//private:
//    std::string text;
//    int fontHeight;
//    TextScrollingSpeed scrollingSpeed;
//
//    void _draw(Canvas *canvas) override;
//
//    static std::map<int, rgb_matrix::Font *> fonts; // Declaration of static member
//};
//
//#endif // DRAWABLE_TEXT_H
