
#include "runner.h"

#include <QApplication>

int main(int argc, char* argv[])
{
  QApplication app{ argc, argv };

  QuarkRunner runner;

  if (!runner.setupFromCommandLine(QApplication::arguments())) {
    return 1;
  }

  return app.exec();
}
