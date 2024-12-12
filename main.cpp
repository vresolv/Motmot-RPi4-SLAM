#include <motmotCore.hpp>

#include <QCoreApplication>

int main(int argc, char *argv[]) {
    
    if (argc < 5) {
        std::cout << std::endl
            << "Usage: ./MotMot path_to_vocabulary path_to_settings if_visualize(true or false) imu_port"
            << std::endl;
        return 0;
    }
    
    QCoreApplication motmotApp(argc, argv);

    motmotCore core(nullptr, argc, argv);

    return motmotApp.exec();
}
