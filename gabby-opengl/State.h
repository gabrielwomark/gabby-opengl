#ifndef STATE_H
#define STATE_H

#include "BlockAtlas.h"
#include "Shader.h"
#include "Window.h"
#include "World.h"

struct State {
    class Window *window;
};

extern struct State state;
#endif
