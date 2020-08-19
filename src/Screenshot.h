//
// Created by Tigermouthbear on 8/16/20.
//

#ifndef TIGERCAPTURE_SCREENSHOT_H
#define TIGERCAPTURE_SCREENSHOT_H

#include <ctime>
#include <string>
#include <QPixmap>
#include "Config.h"
#include "Uploader.h"

class Screenshot {
private:
    Config* config;

    QPixmap pixmap;

public:
    Screenshot(Config* config);
    ~Screenshot();

    void Take();
    void Take(int x, int y, int width, int height);
    void Crop(int x, int y, int width, int height);
    void Save();

    QImage Image();
};


#endif //TIGERCAPTURE_SCREENSHOT_H
