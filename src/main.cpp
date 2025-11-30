#include "finirig/ui/MainWindow.h"
#include <QApplication>
#include <memory>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    finirig::ui::MainWindow window;
    window.show();

    return app.exec();
}

