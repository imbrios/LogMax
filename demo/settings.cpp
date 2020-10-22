#include "settings.h"
#include "ui_settings.h"
#include <QSysInfo>
#include <QStorageInfo>
#include <QNetworkInterface>
#include <QSettings>
#include <QProcess>
#include <QDebug>

settings::settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settings)
{
    ui->setupUi(this);

    QPixmap close(":/icons/images/icons-close-48.png");
    QIcon closeIcon(close);
    ui->btnExit->setIcon(closeIcon);
    ui->btnExit->setIconSize(close.rect().size());

    QString cpuInfo;
    QString netInfo[5];
    int pyhcount=0;

    cpuInfo.append(QString("Type:%1\nArch:%2\nOs:%3\nKernel:%4\nName:%5").arg(QSysInfo::buildAbi().toUpper(),
                                                 QSysInfo::buildCpuArchitecture(),
                                                 QSysInfo::kernelType(),
                                                 QSysInfo::kernelVersion(),
                                                 QSysInfo::machineHostName().toUpper()));
    qDebug() << QSysInfo::buildAbi().toUpper() \
             << QSysInfo::buildCpuArchitecture() \
             << QSysInfo::currentCpuArchitecture() \
             << QSysInfo::kernelType() \
             << QSysInfo::kernelVersion() \
             << QSysInfo::machineHostName() \
             << QSysInfo::prettyProductName() \
             << QSysInfo::productType() \
             << QSysInfo::productVersion();

    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
         if (storage.isValid() && storage.isReady()) {
             if (!storage.isReadOnly()) {
                 qDebug() << "Name:" << storage.name() \
                        << "Root:" << storage.rootPath() \
                        << "FileSys:" << storage.fileSystemType() \
                        << "Disk Spaces:" \
                        << storage.bytesAvailable()/1000000000 <<  "GB" \
                        << "/" \
                        << storage.bytesTotal()/1000000000 <<  "GB" ;
             }
         }
     }

    foreach(QNetworkInterface networkInterface, QNetworkInterface::allInterfaces())
        {
            if (networkInterface.flags().testFlag(QNetworkInterface::IsUp) && !networkInterface.flags().testFlag(QNetworkInterface::IsLoopBack))
            {
                foreach (QNetworkAddressEntry entry, networkInterface.addressEntries())
                {
                    if ( entry.ip().toString().contains(".")){
                        qDebug() << "Interface:"<< networkInterface.name() \
                                 << "IP:"<< entry.ip().toString() \
                                 << "MAC:"  << networkInterface.hardwareAddress();
                        netInfo[pyhcount].append(QString("Interface:%1\nIP:%2\nMAC:%3").arg(networkInterface.name(),
                                                                                            entry.ip().toString(),
                                                                                            networkInterface.hardwareAddress()));
                        pyhcount++;

                    }
                }
            }
        }
    ui->lblCPU->setText(cpuInfo);
    ui->lblNetwork->setText(netInfo[0]); // set first valid interface
}

settings::~settings()
{
    delete ui;
}

void settings::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}
void settings::on_btnExit_clicked()
{
    this->close();
}
