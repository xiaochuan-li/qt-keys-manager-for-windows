#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QStringListModel>
#include <QString>
#include <QIcon>
#include <Qt>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include <QModelIndex>
#include <QHotkey>
#include <QClipboard>
#include <QKeyEvent>
#include "winTools.h"
#include <QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QCborValue>
#include <QCborMap>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;

}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void pageClose(void);
public slots:
    void clicked(QModelIndex qModelIndex);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void quit();
protected:
    void keyPressEvent(QKeyEvent *event);
    void removeItem(QModelIndex qModelIndex);
    void readConfigFile(void);
    void updateConfigFile(void);
    void addKey(void);
    void updateModel(void);
private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *tp;
    QMenu *menu;
    QAction *action;
    QStringListModel *model;
    QHotkey *hotkey;
    QStringList titles;
    QStringList values;
};
#endif // MAINWINDOW_H
