#include <cameraManager.hpp>

cameraManager::cameraManager() {
    
    qRegisterMetaType<cv::Mat>("cv::Mat");
}

cameraManager::~cameraManager() {}

void cameraManager::startCam(int width, int height, int framerate) {

    cam = std::make_unique<lccv::PiCamera>();
    
    cam->options->video_width=width;
    cam->options->video_height=height;
    cam->options->framerate=framerate;
    cam->options->verbose=true;

    isRunning = true;
    
    cv::Mat currentFrame(cam->options->video_height, cam->options->video_width, CV_8UC3);
    
    qDebug() << "Starting Camera";
    cam->startVideo();

    while(isRunning) {

        if(!cam->getVideoFrame(currentFrame, 1000))
            qDebug() << "Cam Timeout error";
        else {
            emit sendCurrentFrame(currentFrame.clone());
            //qDebug() << "Cam Frame";
            //cv::imwrite("output.jpeg", currentFrame);
        }
    }
    
    cam->stopVideo();
}

void cameraManager::stopCam() {
    isRunning = false;
}
