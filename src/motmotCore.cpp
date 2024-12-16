#include <motmotCore.hpp>

motmotCore::motmotCore(QObject *parent, int argc, char *argv[]) : QObject(parent) {
    
    motmotCore::setSignalHandlerObject(this);
    
    std::string vocabPath = argv[1];
    std::string configPath = argv[2];
    bool if_visualize = true;
    
    std::string if_visualize_str = argv[3];
    
    if (if_visualize_str == "false")
        if_visualize = false;
    
    pCamManager = std::make_unique<cameraManager>();
    pImuManager = std::make_unique<imuManager>();    
    pOrbSlamManager = std::make_unique<orbSlamManager>(this);
    
    pCamManagerThread = std::make_unique<QThread>();
    pImuManagerThread = std::make_unique<QThread>();
    
    pCamManager->moveToThread(pCamManagerThread.get());
    pImuManager->moveToThread(pImuManagerThread.get());
    
    QObject::connect(pCamManagerThread.get(), SIGNAL(finished()), pCamManager.get(), SLOT(deleteLater()));
    QObject::connect(pImuManagerThread.get(), SIGNAL(finished()), pImuManager.get(), SLOT(deleteLater()));
    
    QObject::connect(this, &motmotCore::startCam, pCamManager.get(), &cameraManager::startCam);
    QObject::connect(this, &motmotCore::stopCam, pCamManager.get(), &cameraManager::stopCam, Qt::DirectConnection);
    QObject::connect(this, &motmotCore::startImu, pImuManager.get(), &imuManager::startImu);
    QObject::connect(this, &motmotCore::stopImu, pImuManager.get(), &imuManager::stopImu);

    QObject::connect(pImuManager.get(), &imuManager::sendCurrentImuFrame, pOrbSlamManager.get(), &orbSlamManager::recImuData);
    QObject::connect(pCamManager.get(), &cameraManager::sendCurrentFrame, pOrbSlamManager.get(), &orbSlamManager::recCamData);

    pOrbSlamManager->initORBSLAM3(argv[1], argv[2], if_visualize);
    
    pImuManagerThread->start();
    emit startImu(argv[4]);
        
    pCamManagerThread->start();
    emit startCam(640, 480, 30);
    //pCamManager->startCam(640, 480, 30);
}

motmotCore::~motmotCore() {}

motmotCore *motmotCore::appCoreInstance;
void motmotCore::handleSignal(int param) {
    std::cout << "Initiating Application Stop Procedure" << std::endl;
    pOrbSlamManager->stopORBSLAM3();
    emit stopCam();
    emit stopImu();
    QTimer::singleShot(1000, this, &motmotCore::closeApp);
}

void motmotCore::closeApp() {
    QObject::disconnect(this, &motmotCore::startCam, pCamManager.get(), &cameraManager::startCam);
    QObject::disconnect(this, &motmotCore::stopCam, pCamManager.get(), &cameraManager::stopCam);
    QObject::disconnect(this, &motmotCore::startImu, pImuManager.get(), &imuManager::startImu);
    QObject::disconnect(this, &motmotCore::stopImu, pImuManager.get(), &imuManager::stopImu);
    
    QObject::disconnect(pImuManager.get(), &imuManager::sendCurrentImuFrame, pOrbSlamManager.get(), &orbSlamManager::recImuData);
    QObject::disconnect(pCamManager.get(), &cameraManager::sendCurrentFrame, pOrbSlamManager.get(), &orbSlamManager::recCamData);
    
    if (pCamManagerThread && pCamManager) {
        pCamManagerThread->quit();
        pCamManagerThread->wait();
    }
    
    if (pImuManagerThread && pImuManager) {
        pImuManagerThread->quit();
        pImuManagerThread->wait();
    }
    
    if (pOrbSlamManagerThread && pOrbSlamManager) {
        pOrbSlamManagerThread->quit();
        pOrbSlamManagerThread->wait();
    }
    
    std::cout << "Stopping All Threads" << std::endl;
    QCoreApplication::quit();
}
