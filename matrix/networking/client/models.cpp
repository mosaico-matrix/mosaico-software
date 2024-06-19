//#ifndef MODELS_CPP
//#define MODELS_CPP
//#include <iostream>
//#include <string>
//#include <vector>
//#include "../../external/json/json.hpp"
//#include "../../external/rpi-rgb-led-matrix/include/graphics.h"
//using namespace rgb_matrix;
//using json = nlohmann::json;
//namespace ApiModels {
//
//
//    /*
//    |--------------------------------------------------------------------------
//    | Texts
//    |--------------------------------------------------------------------------
//    */
//
//    struct TextLine {
//    public:
//        std::string text;
//        unsigned short fontHeight; // changed from font_height to fontHeight
//        std::string color; // hex
//
//        Color getMatrixColor() {
//            std::string hex = color;
//            if (hex[0] == '#') {
//                hex = hex.substr(1);
//            }
//            int r, g, b;
//            std::sscanf(hex.c_str(), "%02x%02x%02x", &r, &g, &b);
//            return Color(r, g, b);
//        }
//    };
//    // Define from_json function inside the class
//    inline void from_json(const json &j, TextLine &textLine) {
//        j.at("text").get_to(textLine.text);
//        j.at("font_height").get_to(textLine.fontHeight); // changed from font_height to fontHeight
//        j.at("color").get_to(textLine.color);
//    }
//
//    struct Text {
//    public:
//        std::string name;
//        unsigned short marginTop;
//        unsigned short marginBottom;
//        unsigned short marginLeft;
//        unsigned short marginRight;
//        std::vector<TextLine> textLines;
//    };
//
//    // Define from_json function inside the class
//    inline void from_json(const json &j, Text &text) {
//        j.at("name").get_to(text.name);
//        j.at("margin_top").get_to(text.marginTop);
//        j.at("margin_bottom").get_to(text.marginBottom);
//        j.at("margin_left").get_to(text.marginLeft);
//        j.at("margin_right").get_to(text.marginRight);
//        j.at("text_lines").get_to(text.textLines);
//    }
//
//    struct BoardLine {
//        int id;
//        int boardId;
//        std::string name;
//        bool isDone;
//        std::string color;
//    };
//
//    // Define from_json function inside the class
//    inline void from_json(const json &j, BoardLine &boardLine) {
//        j.at("id").get_to(boardLine.id);
//        j.at("board_id").get_to(boardLine.boardId);
//        j.at("name").get_to(boardLine.name);
//        j.at("is_done").get_to(boardLine.isDone);
//        j.at("color").get_to(boardLine.color);
//    }
//
//    /*
//    |--------------------------------------------------------------------------
//    | Boards
//    |--------------------------------------------------------------------------
//    */
//    struct Board {
//        int id;
//        std::string name;
//        bool isTodo;
//        int userId;
//        std::string nameColor;
//        std::vector<BoardLine> boardLines;
//    };
//
//    // Define from_json function inside the class
//    inline void from_json(const json &j, Board &board) {
//        j.at("id").get_to(board.id);
//        j.at("name").get_to(board.name);
//        j.at("is_todo").get_to(board.isTodo);
//        j.at("user_id").get_to(board.userId);
//        j.at("name_color").get_to(board.nameColor);
//        j.at("board_lines").get_to(board.boardLines);
//    }
//
//}
//#endif
