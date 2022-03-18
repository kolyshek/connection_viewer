#include "mainwindow.h"
#include "./ui_mainwindow.h"

QString MainWindow::getInternetAccessStatus() const
{
    QNetworkAccessManager nam;
    QNetworkRequest req(QUrl("http://www.google.com"));
    QNetworkReply *reply = nam.get(req);
    QEventLoop loop;

    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if(reply->bytesAvailable())
    {
        return INTERNET_IS_AVAILABLE;
    }
    else
    {
        return INTERNET_IS_NOT_AVAILABLE;
    }

    return DEFAULT_VALUE;
}

QString MainWindow::getConnectionData(QNetworkInterface::InterfaceType iType, AddressType aType) const
{
    for (const auto &networkInterface: QNetworkInterface::allInterfaces())
    {
        if (!networkInterface.flags().testFlag(QNetworkInterface::IsLoopBack) && networkInterface.type() == iType)
        {
            for (const auto &entry: networkInterface.addressEntries())
            {
                if (entry.ip().toString().contains("."))
                {
                    if (aType == AddressType::IP)
                    {
                        return entry.ip().toString();
                    }
                    else if (aType == AddressType::MAC)
                    {
                        return networkInterface.hardwareAddress();
                    }
                }
            }
        }
    }

    return DEFAULT_VALUE;
}

QString MainWindow::getConnectionStatus(QNetworkInterface::InterfaceType iType) const
{
    for(const auto &networkInterface: QNetworkInterface::allInterfaces())
    {
        if (networkInterface.flags().testFlag(QNetworkInterface::IsRunning)
            && !networkInterface.flags().testFlag(QNetworkInterface::IsLoopBack)
            && networkInterface.type() == iType)
        {
            return CONNECTION_TURNED_ON;
        }
        else
        {
            return CONNECTION_TURNED_OFF;
        }
    }

    return DEFAULT_VALUE;
}

void MainWindow::slotUpdateNetworkData()
{
    m_ui->connectionStatusText->setText(getInternetAccessStatus());

    if (m_ui->dataUpdateCheckBox->isChecked())
    {
        m_ui->ethernetStatusText->setText(getConnectionStatus(QNetworkInterface::Ethernet));
        m_ui->ethernetIPText->setText(getConnectionData(QNetworkInterface::Ethernet, AddressType::IP));
        m_ui->ethernetMACText->setText(getConnectionData(QNetworkInterface::Ethernet, AddressType::MAC));

        m_ui->WiFiStatusText->setText(getConnectionStatus(QNetworkInterface::Wifi));
        m_ui->WiFiIPText->setText(getConnectionData(QNetworkInterface::Wifi, AddressType::IP));
        m_ui->WiFiMACText->setText(getConnectionData(QNetworkInterface::Wifi, AddressType::MAC));
    }
}

void MainWindow::slotUpdateProgressBar()
{
    m_ui->dataUpdateProgressBar->setValue(m_ui->dataUpdateProgressBar->value() + 1);
    if (m_ui->dataUpdateProgressBar->value() >= m_ui->dataUpdateProgressBar->maximum())
    {
        m_ui->dataUpdateProgressBar->setValue(0);
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    setFixedSize(geometry().width(), geometry().height());

    m_dataUpdateTimer = new QTimer();
    connect(m_dataUpdateTimer, SIGNAL(timeout()), this, SLOT(slotUpdateNetworkData()));
    m_dataUpdateTimer->start(1000);

    m_progressBarTimer = new QTimer();
    connect(m_progressBarTimer, SIGNAL(timeout()), this, SLOT(slotUpdateProgressBar()));
    m_progressBarTimer->start(10);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}
