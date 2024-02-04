#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class PaintView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:

      void closeEvent(QCloseEvent *event) override;
private slots:
    void open();
    void save();
    void penColor();
    void penWidth();
    void about();

private:
    Ui::MainWindow *ui;
    void createActions();
    void createMenus();
    void createTools();
    bool maybeSave();
    bool saveFile (const QByteArray &fileFormate);
    PaintView *paintView;
    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;

    QAction *openAct;
    QAction *exitAct;
    QList<QAction *> saveAsActs;
    QAction *penWidthAct;
    QAction *clearScreenAct;
    QAction *printAct;
    QAction *penColorAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

};


#endif // MAINWINDOW_H
