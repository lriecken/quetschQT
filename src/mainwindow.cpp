#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->floatingImageWidget = new FloatingImageWidget(this);
    this->setCentralWidget(floatingImageWidget);


    this->imageEncoder = new ImageEncoder(this);
    connect(this->imageEncoder, &ImageEncoder::encodedReady, this, &MainWindow::encodedReady);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_openButton_clicked()
{

    QString filename = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.jpeg *.webp)"));
    if (!filename.isNull()) {
        freshLoaded = true;
        if (!imageEncoder->open(filename)) {
               QMessageBox(QMessageBox::Information, tr("Error"), tr("Could not load image file."), QMessageBox::Ok).exec();
        } else {
            currentFileName = filename;
        }
    }
}


void MainWindow::on_saveButton_clicked()
{
    if (currentFileName != "") {
        QFileInfo fileInfo(currentFileName);
        QString baseName = fileInfo.baseName();
        QString extension = fileInfo.completeSuffix();
        QString path = fileInfo.absolutePath();
        QDir dir(path);
        int quality = ui->qualitySlider->value();
        auto format = (ImageEncoder::FORMATS)ui->formatCombo->currentIndex();
        QString newFormat;
        switch (format) {
        case ImageEncoder::FORMATS::JPEG:
            newFormat = "jpg"; break;
        case ImageEncoder::FORMATS::PNG:
            newFormat = "png"; break;
        case ImageEncoder::FORMATS::WEBP:
            newFormat = "webp"; break;
        }

        QString sugestedFile = QDir::cleanPath(path + QDir::separator()+ baseName + "_q" + QString::number(quality) + "."+newFormat);


        QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), sugestedFile);
        if (!filename.isNull()) {
            if (!imageEncoder->save(filename)) {
                QMessageBox(QMessageBox::Information, tr("Error"), tr("Could not save image file."), QMessageBox::Ok).exec();
            }
        }

    }

}


void MainWindow::encodedReady(QImage image, int size, int width, int height, int scaledWidth, int scaledHeight, int quality) {
    if (freshLoaded) {
        floatingImageWidget->setImage(image);
        freshLoaded = false;
    } else {
        floatingImageWidget->updateImage(image);
    }
    updateStatusBar(size, width, height, scaledWidth, scaledHeight, quality);

}


void MainWindow::on_scaleSlider_sliderMoved(int position)
{
    float scale =  (float)position / 1000.0;
    if (scale > 0.9) {
        scale = 1;
    }
    ui->scaleLabel->setText(tr("Scale: ") + QString::number(scale));
}


void MainWindow::on_qualitySlider_sliderMoved(int position)
{
    ui->qualityLabel->setText(tr("Quality: ") + QString::number(position));
}

void MainWindow::updateStatusBar(int size, int width, int height, int scaledWidth, int scaledHeight, int quality) {
    QString origDim = QString::number(width)+"x"+QString::number(height);
    QString scaledDim = QString::number(scaledWidth)+"x"+QString::number(scaledHeight);
    QString sizeBytes = QString::number(size) + "b";
    QString sizeKiloBytes = QString::number((float)size/1024.0) + "kb";
    QString sizeMegaBytes = QString::number((float)size/1024.0/1024.0) + "mb";
    QString qualityS = QString::number(quality);
    this->ui->statusbar->showMessage(tr("Original dimensions: ") + origDim + " - " + tr("Scaled dimensions: ") + scaledDim + tr(" Quality: ") + qualityS + " " + sizeBytes + " " + sizeKiloBytes + " " + sizeMegaBytes);


}


void MainWindow::on_scaleSlider_sliderReleased()
{
    encodeWithCurrentSetting();
}


void MainWindow::on_qualitySlider_sliderReleased()
{
    encodeWithCurrentSetting();
}

void MainWindow::on_formatCombo_currentIndexChanged(int index)
{
   encodeWithCurrentSetting();

}

void MainWindow::encodeWithCurrentSetting() {
    float scale =  (float)ui->scaleSlider->value() / 1000.0;
    float quality = ui->qualitySlider->value();
     auto format = (ImageEncoder::FORMATS)ui->formatCombo->currentIndex();
    if (scale > 0.9) {
        scale = 1;
    }
    imageEncoder->encode(quality, scale, format);
}






void MainWindow::on_about_clicked()
{
    if (aboutWindow == nullptr) {
        aboutWindow = new About(this);
    }
    aboutWindow->show();

}

