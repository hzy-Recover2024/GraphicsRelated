#include "engine.h"
#include <windows.h>
#include "entry.h"
int main(int argc, char** argv)
{
    GRelated::Entry init;
    auto& painter = GRelated::Engine::getInstance();
    painter.draw();

    return 0;
}