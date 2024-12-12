#include <cameraManager.hpp>

cameraManager::cameraManager(QObject *parent) : QThread(parent) {}

cameraManager::~cameraManager() {}

void cameraManager::startCam(int width, int height, int framerate) {

    cam.options->video_width=width;
    cam.options->video_height=height;
    cam.options->framerate=framerate;

    isRunning = true;  
}

void cameraManager::stopCam() {

    isRunning = false;
}

void cameraManager::run() {

    qDebug() << "Starting Cmaera";
    cam.startVideo();

    while(isRunning) {

        if(!cam.getVideoFrame(currentFrame, 1000))
            qDebug() << "Cam Timeout error";
        else
            emit sendCurrentFrame(currentFrame.clone());

    }

    cam.stopVideo();
    qDebug() << "Stopping Cmaera";
}