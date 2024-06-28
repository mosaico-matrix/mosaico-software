//#ifndef FLAPPY_BIRD_CPP
//#define FLAPPY_BIRD_CPP
//
//#include "game.cpp"
//#include "../drawables/drawable.h"
//#include "../drawables/drawable-shape.cpp"
//
//class FlappyBird : public Game {
//
//private:
//    int getPipeHeight()
//    {
//        return 20 + rand() % 30;
//    }
//
//public:
//
//    std::list<DrawableRectangle*> pipes;
//    void renderNextCanvasLayer(CanvasLayer *canvas)
//    {
//        // Check if we need to add a new pipe
//        if(pipes.empty())
//        {
//            auto pipe = new DrawableRectangle(10, getPipeHeight());
//            registerDrawable(pipe);
//
//
//            pipe->setColor(GREEN_COLOR);
//            pipe->moveTo(canvas->width(), canvas->height() - pipe->height);
//            pipe->animateTo(-pipe->width, pipe->getY(),200);
//            pipes.push_back(pipe);
//        }
//
//        // Check if head pipe is out of screen
//        if(pipes.front()->getX() < 0)
//        {
//            pipes.pop_front();
//        }
//    }
//
//
//
//
//};
//
//#endif