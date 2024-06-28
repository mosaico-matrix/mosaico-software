//#ifndef PIXEL_MOVER_CPP
//#define PIXEL_MOVER_CPP
//#include "../../../external/rpi-rgb-led-matrix/include/led-matrix.h"
//#include "../../../external/rpi-rgb-led-matrix/include/graphics.h"
//#include <getopt.h>
//#include <termios.h>
//#include <unistd.h>
//#include <deque>
//#include <iostream>
//#include "game.cpp"
//
//using namespace rgb_matrix;
//
//// Simple class that generates a rotating block on the screen.
//class PixelMover : public Game {
//
//private:
//    std::deque<std::pair<int, int> > trail;
//    int x_pos;
//    int y_pos;
//    int trail_len;
//    Color front_color;
//    Color back_color;
//    bool output_is_terminal;
//
//    static uint8_t quantize(float c) {
//        return c < 0 ? 0 : c > 255 ? 255 : roundf(c);
//    }
//    static Color interpolate(const Color &c1, const Color &c2, float fraction) {
//        float c2_fraction = 1 - fraction;
//        return { quantize(c1.r * fraction + c2.r * c2_fraction),
//                 quantize(c1.g * fraction + c2.g * c2_fraction),
//                 quantize(c1.b * fraction + c2.b * c2_fraction)};
//    }
//
//public:
//    PixelMover() : Game(), x_pos(0), y_pos(0), trail_len(0), front_color(0, 255, 0), back_color(0, 0, 0), output_is_terminal(isatty(STDOUT_FILENO)) {
//        trail.push_back(std::make_pair(x_pos, y_pos));
//    }
//
////    void renderFrame() override {
////
////        matrix()->Clear();
////        int distance_from_head = trail.size();
////        for (std::deque<std::pair<int, int> >::const_reverse_iterator it = trail.rbegin(); it != trail.rend(); ++it) { // Using reverse iterators to draw from tail -> head
////            distance_from_head--;
////            Color c = interpolate(front_color, back_color, 1.0 - 1.0f * distance_from_head / trail.size());
////            matrix()->SetPixel(it->first, it->second, c.r, c.g, c.b); // Setting pixels using x and y coordinates from the pair
////        }
////
////        const char c = tolower(getInput());
////        switch (c) {
////            case 'w': case 'k':   // Up
////                if (y_pos > 0) {
////                    y_pos--;
////                    trail.push_back(std::make_pair(x_pos, y_pos));
////                }
////                break;
////            case 's': case 'j':  // Down
////                if (y_pos < matrix()->height() - 1) {
////                    y_pos++;
////                    trail.push_back(std::make_pair(x_pos, y_pos));
////                }
////                break;
////            case 'a': case 'h':  // Left
////                if (x_pos > 0) {
////                    x_pos--;
////                    trail.push_back(std::make_pair(x_pos, y_pos));
////                }
////                break;
////            case 'd': case 'l':  // Right
////                if (x_pos < matrix()->width() - 1) {
////                    x_pos++;
////                    trail.push_back(std::make_pair(x_pos, y_pos));
////                }
////                break;
////                // All kinds of conditions which we use to exit
////            case 0x1B:           // Escape
////            case 'q':            // 'Q'uit
////            case 0x04:           // End of file
////            case 0x00:           // Other issue from getch()
////                break;
////        }
////
////        while ((int)trail.size() > trail_len + 1)
////            trail.pop_front();   // items on front are the oldest dots
////
////    }
//};
//
//#endif