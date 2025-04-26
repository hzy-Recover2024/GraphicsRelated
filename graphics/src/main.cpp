#include "engine.h"
#include <windows.h>
int main(int argc, char** argv)
{
    auto& painter = GRelated::Engine::getInstance();
    painter.draw();

    return 0;
}