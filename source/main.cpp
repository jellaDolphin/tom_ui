#include <QApplication>
#include <QtWidgets/QMessageBox>

#include "qxt/qxtglobalshortcut.h"

#include "source/projectlookup/projectlookup.h"
#include "settings/TomSettings.h"
#include "gotime/ProjectStatusManager.h"
#include "main_window.h"
#include "tray.h"
#include "version.h"

int main(int argc, char *argv[]) {
//    QString command = "tom";
//    bool bash = false;
//
//    if (argc >= 2) {
//        command = QString(argv[1]);
//    }
//
//    if (argc == 3) {
//        bash = QString(argv[2]) == "true";
//    }
//
//    QString appName = "Tom";
//    if (argc == 4) {
//        appName = argv[3];
//    }

#ifdef Q_OS_MAC
    QApplication::setAttribute(Qt::AA_DontShowIconsInMenus);
#endif

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QCoreApplication::setOrganizationName("Tom");
    QCoreApplication::setOrganizationDomain("Tom");
    QCoreApplication::setApplicationVersion(PROJECT_VERSION);
    QApplication::setQuitOnLastWindowClosed(false);

    QApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription(PROJECT_DESCRIPTION);
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOptions({
                              {"tom",        QCoreApplication::translate("main", "Path to the tom executable"),                         "tomPath",    "tom"},
                              {"bash",       QCoreApplication::translate("main", "Defines if the tom executable is to be treated as a Bash file")},
                              {"configName", QCoreApplication::translate("main", "Defines the configuration name, useful to test Tom"), "configName", "Tom"}
                      });

    // Process the actual command line arguments given by the user
    parser.process(app);

    const QString &command = parser.value("tom");
    const bool bash = parser.isSet("bash");
    const QString &configName = parser.value("configName");
    if (!configName.isEmpty()) {
        QCoreApplication::setApplicationName(configName);
    }

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    QApplication::installTranslator(&qtTranslator);

    QTranslator myappTranslator;
    myappTranslator.load(":/translations/tom_" + QLocale::system().name());
    QApplication::installTranslator(&myappTranslator);

    auto *control = new TomControl(command, bash, &app);
    const CommandStatus &status = control->version();
    if (status.isFailed()) {
        const QString &message = QString("The tom command line application was not found or is not working as expected.<br>Path: <i>%1</i><br>Terminating.").arg(command);
        QMessageBox::critical(nullptr, "Configuration error", message);
        QApplication::exit(-1);
        return -1;
    }

#ifdef Q_OS_MAC
    // locate binary next to our binary in the app bundle
    command = QFileInfo(QCoreApplication::applicationFilePath()).dir().filePath("tom");
#endif

    auto config = new TomSettings(&app);
    auto *statusManager = new ProjectStatusManager(control, &app);

    MainWindow mainWindow(control, statusManager, config);
    mainWindow.show();

    new GotimeTrayIcon(control, &mainWindow);

    // update the UI with the current settings
    config->triggerUpdate();

    const QKeySequence shortcut(Qt::ShiftModifier + Qt::ControlModifier + Qt::Key_P);
    const QxtGlobalShortcut globalShortcut(shortcut);
    if (globalShortcut.isValid()) {
        QObject::connect(
                &globalShortcut, &QxtGlobalShortcut::activated, &globalShortcut,
                [&] {
                    ProjectLookup::show(control, &mainWindow, nullptr);
                });
    } else {
        qWarning() << "failed to install global shortcut";
    }

    return QApplication::exec();
}
