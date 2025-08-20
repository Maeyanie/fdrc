#include <QDebug>
#include <QFileOpenEvent>

#include "app.h"
#include "window.h"

App::App(int& argc, char *argv[]) :
    QApplication(argc, argv), window(new Window())
{
    if (argc > 1)
        window->load_drc(argv[1]);
    else
        window->load_drc(":gl/bunny_gltf.drc");
    window->show();
}

App::~App()
{
    delete window;
}

bool App::event(QEvent* e)
{
    if (e->type() == QEvent::FileOpen)
    {
        window->load_drc(static_cast<QFileOpenEvent*>(e)->file());
        return true;
    }
    else
    {
        return QApplication::event(e);
    }
}
