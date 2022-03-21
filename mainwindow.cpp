#include "mainwindow.h"
#include "ui_mainwindow.h"
void MainWindow::readConfigFile(void){
    QFile config(QCoreApplication::applicationDirPath() + QString("/keys.cbor"));
    if(!config.open(QIODevice::ReadOnly)) {
        qDebug() << "File open failed!";
    } else {
        QJsonDocument root_Doc(QJsonDocument(QCborValue::fromCbor(config.readAll()).toMap().toJsonObject()));
        QJsonObject obj = root_Doc.object();
        QJsonObject data = obj["data"].toObject();
        QStringList list = data.keys();
        foreach(QString key, list){
            titles.append(key);
            values.append(data[key].toString());
        }
    }
    config.close();
}
void MainWindow::updateConfigFile(void){
    QFile config(QCoreApplication::applicationDirPath() + QString("/keys.cbor"));
    if(!config.open(QIODevice::WriteOnly)) {
        qDebug() << "File open failed!";
    } else {
        QJsonDocument jdoc;
        QJsonObject obj;
        QJsonObject data;
        for(int i=0; i<titles.size(); i++){
            if(values[i].compare("-1")==0 || values[i].compare("-2")==0){
                continue;
            }else{
                data[titles[i]] = values[i];
            }
        }
        obj["data"]=data;
        jdoc.setObject(obj);
        config.write(QCborValue::fromJsonValue(obj).toCbor());
    }
    config.close();
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusbar->setVisible(false);
    setWindowOpacity(0.8);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setFixedSize(this->width(),this->height());
    hotkey = new QHotkey (QKeySequence("Ctrl+K"), true, this);
    QObject::connect(hotkey, &QHotkey::activated, this, [&](){
        move(cursor().pos());
        showNormal();
    });

    tp = new QSystemTrayIcon(this);
    tp->setIcon(QIcon("://statics/key.png"));
    action = new QAction("&退出(Exit)");
    connect(action, &QAction::triggered, this, [&](){
        tp->setVisible(false);
        close();
        emit pageClose();
    });
    menu = new QMenu();
    menu->addAction(action);
    tp->setContextMenu(menu);

    readConfigFile();

    model = new QStringListModel();
    titles.append("添加");
    values.append("-1");
    titles.append("取消");
    values.append("-2");
    model->setStringList(titles);

    ui->listview->setModel(model);

    connect(ui->listview, &QListView::clicked, this, [&](QModelIndex qModelIndex){this->clicked(qModelIndex);});
    tp->show();
}
void MainWindow::clicked(QModelIndex qModelIndex){
    QString content = this->values[qModelIndex.row()];
    int x = this->cursor().pos().x()-1;
    int y = this->cursor().pos().y()-1;
    this->cursor().setPos(x,y);
    if (content.compare("-1") == 0){
        addKey();
        return ;
    }else if (content.compare("-2") == 0){
        this->showMinimized();
        return;
    }else{
        this->showMinimized();
        clickAt();
        QClipboard *board = QApplication::clipboard();
        board->setText(content);
        copyText();
    }

}
void MainWindow::removeItem(QModelIndex qModelIndex){
    if(qModelIndex.row() != values.size()-1){
        this->values.removeAt(qModelIndex.row());
        this->titles.removeAt(qModelIndex.row());
        updateModel();
    }
}
void MainWindow:: updateModel(void){
    model->setStringList(titles);
    updateConfigFile();
}
MainWindow::~MainWindow()
{
    delete ui;
    delete tp;
    delete menu;
    delete action;
    delete model;
    delete hotkey;
}

void MainWindow::keyPressEvent(QKeyEvent *event){

    if(event->key() == Qt::Key_Return){
        this->clicked(ui->listview->currentIndex());
    }else if(event->key() == Qt::Key_Delete){
        this->removeItem(ui->listview->currentIndex());
    }
}
void MainWindow::addKey(){
    QDialog dialog(this);
    QFormLayout form(&dialog);
    form.addRow(new QLabel("User input:"));
    // Value1
    QString value1 = QString("Name: ");
    QLineEdit *spinbox1 = new QLineEdit(&dialog);
    form.addRow(value1, spinbox1);
    // Value2
    QString value2 = QString("Value: ");
    QLineEdit *spinbox2 = new QLineEdit(&dialog);
    form.addRow(value2, spinbox2);
    // Add Cancel and OK button
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Process when OK button is clicked
    if (dialog.exec() == QDialog::Accepted && spinbox1->text().compare("") != 0) {

        this->titles.push_front(spinbox1->text());
        this->values.push_front(spinbox2->text());
        updateModel();
    }
}
