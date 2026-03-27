#include "GalleryWindow.h"

#include <QApplication>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    application.setOrganizationName(QStringLiteral("Averra"));
    application.setApplicationName(QStringLiteral("AverraGallery"));

    GalleryWindow window;
    window.show();

    return application.exec();
}
