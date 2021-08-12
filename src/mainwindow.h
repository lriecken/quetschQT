#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "floatingimagewidget.h"
#include "imageencoder.h"
#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QImageWriter>
#include <QFileInfo>
#include <QDir>
#include "about.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openButton_clicked();
    void encodedReady(QImage image, int size, int width, int height, int scaledWidth, int scaledHeight, int quality);
    void on_scaleSlider_sliderMoved(int position);

    void on_qualitySlider_sliderMoved(int position);

    void on_scaleSlider_sliderReleased();

    void on_qualitySlider_sliderReleased();

    void on_formatCombo_currentIndexChanged(int index);
    void encodeWithCurrentSetting();

    void on_saveButton_clicked();


    void on_about_clicked();

private:
    Ui::MainWindow *ui;
    FloatingImageWidget *floatingImageWidget;
    ImageEncoder *imageEncoder;
    bool freshLoaded = false;
    QString currentFileName;
    About *aboutWindow = nullptr;

    void updateStatusBar(int size, int width, int height, int scaledWidth, int scaledHeight, int quality);
};
#endif // MAINWINDOW_H
