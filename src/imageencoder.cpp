#include "imageencoder.h"

ImageEncoder::ImageEncoder(QObject *parent) : QObject(parent)
{

}

bool ImageEncoder::open(QString filename) {
    QFile f(filename);
    f.open(QFile::ReadOnly);
    auto bytes = f.readAll();
    f.close();
    if (bytes.size() == 0) {
        return false;
    }
    QBuffer buffer;
    buffer.setData(bytes);
    QImageReader im(&buffer);
    image = im.read();
    if (!image.isNull()) {
        encode(quality, scale, format);
        return true;
    }
    return false;

}

void ImageEncoder::encode(int quality, float scale, FORMATS format) {
    this->quality = quality;
    this->scale = scale;
    this->format = format;
    if (!image.isNull()) {
        QImage scaled = image.scaled((float)image.width()*scale, (float)image.height()*scale);
        QByteArray bufferArray;
        QBuffer outBuffer(&bufferArray);
        outBuffer.open(QIODevice::WriteOnly);
        switch (format) {
        case JPEG:
            scaled.save(&outBuffer, "JPEG", quality);
            break;
        case PNG:
            scaled.save(&outBuffer, "PNG", quality);
            break;
        case WEBP:
            scaled.save(&outBuffer, "WEBP", quality);
            break;
        }


        QBuffer inBuffer;
        inBuffer.setData(bufferArray);
        QImageReader im(&inBuffer);
        QImage candidate = im.read();
        encodedReady(candidate, inBuffer.size(), image.width(), image.height(), scaled.width(), scaled.height(), quality);
    }
}

bool ImageEncoder::save(QString filename) {
    QImage scaled = image.scaled((float)image.width()*scale, (float)image.height()*scale);
    bool result = false;
    switch (format) {
    case JPEG:
        result = scaled.save(filename, "JPEG", quality);
        break;
    case PNG:
        result = scaled.save(filename, "PNG", quality);
        break;
    case WEBP:
        result = scaled.save(filename, "WEBP", quality);
        break;
    }

    return result;
}
