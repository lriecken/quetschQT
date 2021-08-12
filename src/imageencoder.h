#ifndef IMAGEENCODER_H
#define IMAGEENCODER_H

#include <QObject>
#include <QByteArray>
#include <QImageReader>
#include <QImage>
#include <QBuffer>
#include <QFile>

class ImageEncoder : public QObject
{
    Q_OBJECT
public:
    enum FORMATS {
        JPEG = 0,
        PNG = 1,
        WEBP = 2,
    };
    explicit ImageEncoder(QObject *parent = nullptr);
    bool open(QString filename);
    void encode(int quality, float scale, FORMATS format);

    bool save(QString filename);

signals:
    void encodedReady(QImage image, int size, int width, int height, int scaledWidth, int scaledHeight, int quality);

private:
    int quality = 100;
    float scale = 1;
    FORMATS format = JPEG;
    QString filename;
    QImage image;
signals:

};

#endif // IMAGEENCODER_H
