#include <QApplication>
#include "GameWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    GameWidget w;
    w.setWindowTitle("Qt6 Breakout");
    w.show();
    
    return a.exec();
}
