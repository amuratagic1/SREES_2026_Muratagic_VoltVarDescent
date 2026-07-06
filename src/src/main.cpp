#include "Application.h"
#include <gui/WinMain.h>

int main(int argc, const char * argv[])
{
    Application app(argc, argv);
    app.init("EN"); // use "BA" for Bosnian
    return app.run();
}
