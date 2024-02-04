#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "paintview.h"
#include <QCloseEvent>
#include <QApplication>
#include <QFileDialog>
#include <QColorDialog>
#include <QByteArray>
#include <QAction>
#include <QInputDialog>
#include <QMessageBox>
#include <QImageWriter>
#include <QSlider>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    paintView = new PaintView();



    setCentralWidget(paintView);
    createActions();
    createMenus();
    createTools();




    setWindowTitle("A Paint Tool");
    qDebug("MAIN WINDOW");
    resize(500,500);

}

MainWindow::~MainWindow()
{

}


void MainWindow::closeEvent(QCloseEvent *event){
    QMainWindow::closeEvent(event);
    if(maybeSave())
    {
        event->accept();
    }else{
        event->ignore();
    }

}

void MainWindow::open() {
    if (maybeSave()) {

        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
        if(fileName.isEmpty() == false)
        {
            paintView->openImage(fileName);
        }
        // Handle the selected file...
    }
}

void MainWindow::save(){
    QAction *action = qobject_cast<QAction *>(sender()); // Fixed typo
    QByteArray fileFormat = action->data().toByteArray(); // Fixed type, also change QByte to QByteArray
    saveFile(fileFormat);
}

void MainWindow::penColor(){
    QColor newColor = QColorDialog::getColor(paintView->penColor());
    if(newColor.isValid())
    {
        paintView->pen->color = newColor;
    }
}



void MainWindow::penWidth(){
    bool ok;
    int newWidth = QInputDialog::getInt(this,
                                        tr("Scribble"),
                                        tr("Select pen width: "),
                                        paintView->penWidth(),
                                        1,50,1,&ok);

    if(ok)
    {
        paintView->pen->width = newWidth;
    }
}

void MainWindow::about(){
    QMessageBox::about(this,
                       tr("About: A Paint App"),
                       tr("<p>The <b>Scriibble</> example is awesome</p>"));
}

void MainWindow::createActions(){
    openAct = new QAction(tr("&Open"), this);

    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct,SIGNAL(triggered()), this, SLOT(open()));

    foreach(QByteArray format, QImageWriter::supportedImageFormats()){
        QString text = tr("%1...").arg(QString(format).toUpper());
        QAction *action = new QAction(text, this);
        action -> setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(save()));
        saveAsActs.append(action);
    }

    printAct = new QAction(tr("&Print..."), this);
    connect(printAct, SIGNAL(triggered()), paintView, SLOT(print()));
    
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this,SLOT(close()));
    
    penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    penWidthAct = new QAction(tr("Pen &Width..."), this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penColor()));

    clearScreenAct = new QAction(tr("&Clear Screen..."), this);
    clearScreenAct->setShortcut(QKeySequence(tr("Ctrl+L")));
    connect(clearScreenAct, SIGNAL(triggered()), paintView, SLOT(penColor()));


    aboutAct = new QAction(tr("&About..."), this);
    connect(aboutAct, SIGNAL(triggered()), SLOT(about()));

    aboutQtAct = new QAction(tr("Acout &Qt..."), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus(){
    saveAsMenu = new QMenu(tr("&Save As"), this);
    foreach(QAction *action, saveAsActs)
    {
        saveAsMenu->addAction(action);
    }

    fileMenu = new QMenu (tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionsMenu = new QMenu(tr("&Options"), this);
    optionsMenu->addAction(penColorAct);
    optionsMenu->addAction(penWidthAct);

    optionsMenu->addSeparator();
    optionsMenu->addAction(clearScreenAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionsMenu);
    menuBar()->addMenu(helpMenu);
}

void MainWindow::createTools()
{
    QColor colors[] = {Qt::red, Qt::green, Qt::blue, Qt::yellow, Qt::black, Qt::white};

    //loop through each color and create a button
    for(int i = 0; i < 6; i++)
    {
        int yOffset = 30;
        int xOffset = 50;
        int spacing =5;
        QPushButton *colorButton = new QPushButton();

        this->layout()->addWidget(colorButton);


        colorButton->setStyleSheet(QString("background-color: %1").arg(colors[i].name()));
        colorButton->setFixedSize(40,20);
        colorButton->setAutoFillBackground(true);
        colorButton->show();

        //add an event to this button so that it sets the pen color
        connect(colorButton, &QPushButton::clicked, [=](){
            paintView->pen->color = colors[i];
        });



        int yPos = i * (24 + spacing) + yOffset;
        //set to the width of the layout - the width of this button
        int xPos = this->width() + xOffset;
        colorButton->move(xPos, yPos);
        //connect the button to the slot so that is sets the pen color
    }

    //setup slider to set pen width
    QSlider *penWidthSlider = new QSlider(Qt::Vertical, this);
    penWidthSlider->setRange(1,50);
    penWidthSlider->setValue(10);
    penWidthSlider->setGeometry(10, 30, 20, 100);

    //set text
    penWidthSlider->setTickPosition(QSlider::TicksBothSides);
    penWidthSlider->setTickInterval(5);
    penWidthSlider->setSingleStep(1);



    //move to the left of the color buttons
    penWidthSlider->move(this->width() + 10, 30);

    //when the slider is moved, set the pen width
    connect(penWidthSlider, &QSlider::valueChanged, [=](){
        paintView->pen->width = penWidthSlider->value();
    });

    //add to layout
    this->layout()->addWidget(penWidthSlider);


    int shapeButtonX = this->width() - 50;


    //add a button to set the pen shape
    QPushButton *penRectangleButton = new QPushButton("[]", this);
    penRectangleButton->setGeometry(shapeButtonX, 50, 30, 30);
    penRectangleButton->show();

    //when the button is clicked, set the pen shape
    connect(penRectangleButton, &QPushButton::clicked, [=](){
        paintView->pen->shape = CoolPen::Shape::Rectangle;
    });

    QPushButton *penCircleButton = new QPushButton("O", this);
    penCircleButton->setGeometry(shapeButtonX, 100, 30, 30);
    penCircleButton->show();

    //when the button is clicked, set the pen shape
    connect(penCircleButton, &QPushButton::clicked, [=](){
        paintView->pen->shape = CoolPen::Shape::Circle;
    });




}




bool MainWindow::maybeSave() {
    if (paintView->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this,
                                   tr("Paint"),
                                   tr("The image has been modified. \n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (ret == QMessageBox::Save) {
            return saveFile("png");
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
        else if (ret == QMessageBox::Discard)
        {

        }
    }
    // Return true if the image is not modified
    return true;
}



bool MainWindow::saveFile(const QByteArray &fileFormat){

    QString initialPath = QDir::currentPath() + "/untitiled." + fileFormat;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    initialPath,
                                                    tr("%1 Files (*.%2);; All Files (*)")
                                                        .arg(QString::fromLatin1(fileFormat.toUpper()))
                                                        .arg(QString::fromLatin1(fileFormat)));

    //exit condition
    if(fileName.isEmpty())
    {
        return false;
    }

    return paintView->saveImage(fileName, fileFormat.constData());
}





