#include <motmotCore.hpp>

#include <signal.h>

#include <QApplication>

int main(int argc, char *argv[]) {
       
    if (argc < 5) {
        std::cout << std::endl
            << "Usage: ./MotMot path_to_vocabulary path_to_settings if_visualize(true or false) imu_port"
            << std::endl;
        return 0;
    }
    
    QApplication motmotApp(argc, argv);

    motmotCore core(QCoreApplication::instance(), argc, argv);
    
    struct sigaction hup;
    hup.sa_handler = core.callSignalHandler;
    sigemptyset(&hup.sa_mask);
    hup.sa_flags = 0;
    hup.sa_flags |= SA_RESTART;
    if (sigaction(SIGINT, &hup, 0))
        return 1;

    int retVal = QCoreApplication::exec();
    return retVal;
}
