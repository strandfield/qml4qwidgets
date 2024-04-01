
#include "runner.h"

#include <QApplication>

#include <iostream>

void showHelp()
{
  std::cout << "Quark displays forms created with QtDesigner and controlled with QML." << std::endl;
  std::cout << std::endl;
  std::cout << "USAGE:" << std::endl;
  std::cout << "quark [--watch] myform.ui [mycontroller.qml]" << std::endl;
  std::cout << "quark" << std::endl;
  std::cout << std::endl;
  std::cout << "DESCRIPTION" << std::endl;
  std::cout << "The .ui and .qml files may be passed as command line arguments." << std::endl;
  std::cout << "If no qml file is specified, quark tries to deduce one from\n";
  std::cout << "the name of the ui file." << std::endl;
  std::cout << "If --watch is passed, quark watches for changes in the\n";
  std::cout << "input files and will reload them automatically on changes." << std::endl;
  std::cout << "If quark is invoked without any command line arguments, \n";
  std::cout << "it will look for a file named 'main.ui' in the working directory." << std::endl;
}

int main(int argc, char* argv[])
{
  QApplication app{ argc, argv };

  if (QApplication::arguments().contains("--help") || QApplication::arguments().contains("-h")) {
    showHelp();
    return 0;
  }

  QuarkRunner runner;

  if (!runner.setupFromCommandLine(QApplication::arguments())) {
    return 1;
  }

  return app.exec();
}
