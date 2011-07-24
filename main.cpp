#include "mainwindow.h"
// Needed Symbian specific headers
#ifdef Q_OS_SYMBIAN
#include <eikenv.h>
#include <eikappui.h>
#include <aknenv.h>
#include <aknappui.h>
#endif

#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.setOrientation(MainWindow::ScreenOrientationLockLandscape);
    mainWindow.showExpanded();
#ifdef Q_OS_SYMBIAN

            CAknAppUi* appUi = dynamic_cast<CAknAppUi*>( CEikonEnv::Static()->AppUi());

          TRAPD(error,
          if (appUi) {
              // Lock application orientation into landscape
              appUi->SetOrientationL(CAknAppUi::EAppUiOrientationLandscape);
          }
          );
          #endif

    return app.exec();
}
