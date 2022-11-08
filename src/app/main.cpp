#include "application.h"
#include <ctime>
#include <random>
#include <chrono>

int main()
{
    std::srand((unsigned int)std::time(0));
    Application app(1280, 720, "Platformer");
    app.start();

    return 0;
}
