#include <motmotCore.hpp>

motmotCore::motmotCore(QObject *parent, int argc, char *argv[]) {
    pCamManager = std::make_unique<cameraManager>(this);
    pImuManager = std::make_unique<imuManager>(this);
    
    std::string vocabPath = argv[1];
    std::string configPath = argv[2];
    bool if_visualize = true;
    
    std::string if_visualize_str = argv[3];
    std::cout << if_visualize_str;
    
    if (if_visualize_str == "false")
        if_visualize = false;
    
    pOrbSlamManager = std::make_unique<orbSlamManager>(this, argv[1], argv[2], if_visualize);

    QObject::connect(pImuManager.get(), &imuManager::sendCurrentImuFrame, pOrbSlamManager.get(), &orbSlamManager::recImuData);
    QObject::connect(pCamManager.get(), &cameraManager::sendCurrentFrame, pOrbSlamManager.get(), &orbSlamManager::recCamData);

    pOrbSlamManager->start();
    pImuManager->startImu(argv[4]);
    pImuManager->start();

    pCamManager->startCam(640, 480, 15);
    pCamManager->start();
}

motmotCore::~motmotCore() {
    pCamManager->quit();
    pCamManager->wait();
}
