#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define DEFAULT_VALUE "None"
#define INTERNET_IS_AVAILABLE "Доступен"
#define INTERNET_IS_NOT_AVAILABLE "Недоступен"
#define CONNECTION_TURNED_ON "Включен"
#define CONNECTION_TURNED_OFF "Выключен"

enum class AddressType
{
    IP,
    MAC
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *m_ui;

    QTimer *m_dataUpdateTimer;
    QTimer *m_progressBarTimer;

protected:
    QString getInternetAccessStatus() const;
    QString getConnectionStatus(QNetworkInterface::InterfaceType iType) const;
    QString getConnectionData(QNetworkInterface::InterfaceType iType, AddressType aType) const;

protected slots:
    void slotUpdateNetworkData();
    void slotUpdateProgressBar();

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();
};

#endif // MAINWINDOW_H
