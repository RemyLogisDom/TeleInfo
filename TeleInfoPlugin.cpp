#include <QtWidgets>

#include "TeleInfoPlugin.h"
#include "../common.h"



TeleInfoPlugin::TeleInfoPlugin(QWidget *parent) : QWidget(parent)
{
    ui = new QWidget();
    QGridLayout *layout = new QGridLayout(ui);
    QWidget *w = new QWidget();
    mui = new Ui::TeleInfoUI;
    mui->setupUi(w);
    layout->addWidget(w);
    mui->logTxt->hide();
    mui->deviceTable->setColumnCount(9);
    mui->deviceTable->setHorizontalHeaderItem(0, new QTableWidgetItem("RomID"));
    mui->deviceTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Parameter"));
    mui->deviceTable->setHorizontalHeaderItem(2, new QTableWidgetItem("Mode"));
    mui->deviceTable->setHorizontalHeaderItem(3, new QTableWidgetItem("Offset"));
    mui->deviceTable->setHorizontalHeaderItem(4, new QTableWidgetItem("Interval"));
    mui->deviceTable->setHorizontalHeaderItem(5, new QTableWidgetItem("Read"));
    mui->deviceTable->setHorizontalHeaderItem(6, new QTableWidgetItem("Value"));
    mui->deviceTable->setHorizontalHeaderItem(7, new QTableWidgetItem("Last Value"));
    mui->deviceTable->setHorizontalHeaderItem(8, new QTableWidgetItem("Set Value"));
    connect(mui->checkBoxLog, SIGNAL(stateChanged(int)), this, SLOT(logEnable(int)));
    connect(mui->editIP, SIGNAL(textChanged(QString)), this, SLOT(IPEdited(QString)));
    connect(mui->editPort, SIGNAL(textChanged(QString)), this, SLOT(PortEdited(QString)));
    connect(&socketThread, SIGNAL(tcpStatusChange()), this, SLOT(tcpStatusChange()));
    connect(&socketThread, SIGNAL(read(QString)), this, SLOT(read(QString)));
    connect(mui->Start, SIGNAL(clicked()), this, SLOT(Start()));
    connect(mui->Stop, SIGNAL(clicked()), this, SLOT(Stop()));
    tcpIconUnconnectedState.addPixmap(QPixmap(QString::fromUtf8(":/images/images/disconnected.png")), QIcon::Normal, QIcon::Off);
    tcpIconHostLookupState.addPixmap(QPixmap(QString::fromUtf8(":/images/images/connecting.png")), QIcon::Normal, QIcon::Off);
    tcpIconConnectingState.addPixmap(QPixmap(QString::fromUtf8(":/images/images/connecting.png")), QIcon::Normal, QIcon::Off);
    tcpIconConnectedState.addPixmap(QPixmap(QString::fromUtf8(":/images/images/connected.png")), QIcon::Normal, QIcon::Off);
    tcpIconClosingState.addPixmap(QPixmap(QString::fromUtf8(":/images/images/disconnecting.png")), QIcon::Normal, QIcon::Off);
    connect(mui->editName, SIGNAL(editingFinished()), this, SLOT(on_editName_editingFinished()));
    connect(mui->AddButton, SIGNAL(clicked()), this, SLOT(on_AddButton_clicked()));
    connect(mui->RemoveButton, SIGNAL(clicked()), this, SLOT(on_RemoveButton_clicked()));
    mui->RemoveButton->setEnabled(false);
    QObject::connect(mui->deviceTable, &QTableWidget::cellClicked, [this]() { mui->RemoveButton->setEnabled(false); });
    QObject::connect(mui->deviceTable->verticalHeader(), &QHeaderView::sectionClicked, [this]() { mui->RemoveButton->setEnabled(true); });
}


TeleInfoPlugin::~TeleInfoPlugin()
{
}


QObject *TeleInfoPlugin::getObject()
{
    return this;
}


QWidget *TeleInfoPlugin::widgetUi()
{
    return ui;
}

QWidget *TeleInfoPlugin::getDevWidget(QString)
{
    return nullptr;
}

void TeleInfoPlugin::setConfigFileName(const QString fileName)
{
    configFileName = fileName;
    configFileName.chop(3);
    configFileName.append(".cfg");
    mui->labelInterfaceName->setToolTip(configFileName);
}

void TeleInfoPlugin::readDevice(const QString &)
{
}


QString TeleInfoPlugin::getDeviceCommands(const QString &)
{
    return "";
}



QString TeleInfoPlugin::getParameters(QString search, const QString &str)
{
    QStringList data = str.split("\n");
    foreach (QString s, data)
    {
        if (s.startsWith(search))
        {
            int coma = s.indexOf("(", 1);
            if (coma) {
                if (s.endsWith(")"))
                {
                    QString result = s.mid(coma + 1, s.length() - coma - 2);
                    if (!result.isEmpty())
                    {
                    if (result.mid(0, 4) == "HEX:")
                            {
                                    QString Hex;
                                    Hex.append(result.remove(0, 4));
                                    QTextCodec *Utf8Codec = QTextCodec::codecForName("UTF-8");
                                    QByteArray F = QByteArray::fromHex(Hex.toLatin1());
                                    result = Utf8Codec->toUnicode(F);
                            }
                    }
                    return result;
                }
            }
        }
    }
    return "";
}

void TeleInfoPlugin::saveConfig()
{
    QFile file(configFileName);
    QByteArray configdata;
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out.setGenerateByteOrderMark(true);
        QString str;
        QDateTime now = QDateTime::currentDateTime();
        str += "Configuration file " + now.toString() + "\n";
        str += saveformat("IPadress", socketThread.ip);
        str += saveformat("Port", QString("%1").arg(socketThread.port));
        str += saveformat("Name", mui->editName->text());
        str += saveformat("TimeOut", QString("%1").arg(mui->spinBoxTimeOut->value()));
        int index = 1;
        foreach (teleInfoItem *dev, teleInfoDevices)
        {
            QString data;
            data.append(dev->ParameterList->currentText() + "/");    // ParameterList
            data.append(dev->counterMode->currentText() + "/");    // counterMode
            data.append(dev->interval->currentText() + "/");    // interval
            data.append(dev->offset->text());    // offset
            str += saveformat(QString("Device_%1").arg(index++, 3, 10, QChar('0')), data);
        }
        out << str;
        file.close();
    }
}


void TeleInfoPlugin::readConfig()
{
    QFile file(configFileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString configData;
        configData.append(in.readAll());
        file.close();
        setipaddress(getvalue("IPadress", configData));
        setport(getvalue("Port", configData));
        QString Name = getvalue("Name", configData);
        if (!Name.isEmpty()) mui->editName->setText(Name);
        emit(updateInterfaceName(this, Name));
        bool ok;
        int t = getvalue("TimeOut", configData).toInt(&ok);
        if (ok) mui->spinBoxTimeOut->setValue(t);
        int count = 1;
        QString devID = QString("Device_%1").arg(count, 3, 10, QChar('0'));
        QString data = getParameters(devID, configData);
        while (!data.isEmpty())
        {
            teleInfoItem *newDev = addDevice(buildRomID(count));
            QStringList parameters = data.split("/");
            if (newDev && parameters.count() > 3) {
                newDev->ParameterList->setCurrentText(parameters.at(0));
                newDev->counterMode->setCurrentText(parameters.at(1));
                newDev->interval->setCurrentText(parameters.at(2));
                newDev->offset->setText(parameters.at(3));
            }
            devID = QString("Device_%1").arg(++count, 3, 10, QChar('0'));
            data = getParameters(devID, configData);
        }
    }
    socketThread.endLessLoop = true;
    socketThread.start();
}


void TeleInfoPlugin::setLockedState(bool state)
{
    mui->AddButton->setEnabled(!state);
    mui->RemoveButton->setEnabled(!state);
    mui->editIP->setEnabled(!state);
    mui->editPort->setEnabled(!state);
    mui->editName->setEnabled(!state);
    mui->spinBoxTimeOut->setEnabled(!state);
}


QString TeleInfoPlugin::getDeviceConfig(QString)
{
    return "";
}


void TeleInfoPlugin::setDeviceConfig(const QString &, const QString &)
{
}


QString TeleInfoPlugin::getName()
{
    return mui->editName->text();
}



void TeleInfoPlugin::logEnable(int state)
{
    if (state) {
        mui->logTxt->show();
    }
    else {
        mui->logTxt->hide();
    }
}


double TeleInfoPlugin::getMaxValue(const QString)
{
    return 65535;
}


bool TeleInfoPlugin::isManual(const QString)
{
    return false;
}


bool TeleInfoPlugin::isDimmable(const QString)
{
    return false;
}


bool TeleInfoPlugin::acceptCommand(const QString)
{
    return false;
}


void TeleInfoPlugin::setStatus(const QString)
{
}


void TeleInfoPlugin::setipaddress(const QString &adr)
{
    if (!adr.isEmpty()) mui->editIP->setText(adr);		// affichage de l'adresse IP
}


void TeleInfoPlugin::setport(const QString &p)
{
    if (!p.isEmpty()) {
        mui->editPort->setText(p);		// affichage de l'adresse IP
        socketThread.port = quint16(p.toInt()); }
    else {
        mui->editPort->setText("1470");		// affichage de l'adresse IP
        socketThread.port = quint16(1470); }
}


void TeleInfoPlugin::IPEdited(QString ip)
{
    socketThread.ip = ip;
}


void TeleInfoPlugin::tcpStatusChange()
{
    switch (socketThread.socketState)
    {
        case QAbstractSocket::UnconnectedState : mui->toolButtonTcpState->setIcon(tcpIconUnconnectedState);
            mui->toolButtonTcpState->setToolTip(tr("Not connected"));
        break;
        case QAbstractSocket::HostLookupState : mui->toolButtonTcpState->setIcon(tcpIconConnectingState);
            mui->toolButtonTcpState->setToolTip(tr("Host lookup"));
        break;
        case QAbstractSocket::ConnectingState : mui->toolButtonTcpState->setIcon(tcpIconConnectingState);
            mui->toolButtonTcpState->setToolTip(tr("Connecting"));
        break;
        case QAbstractSocket::ConnectedState : mui->toolButtonTcpState->setIcon(tcpIconConnectedState);
            mui->toolButtonTcpState->setToolTip(tr("Connected"));
        break;
        case QAbstractSocket::ClosingState : mui->toolButtonTcpState->setIcon(tcpIconClosingState);
            mui->toolButtonTcpState->setToolTip(tr("Disconnecting"));
        break;
        default: break;
    }
}


void TeleInfoPlugin::Start()
{
    OnOff = true;
    socketThread.endLessLoop = true;
    socketThread.start();
}


void TeleInfoPlugin::Stop()
{
    if (socketThread.isRunning()) {
    OnOff = false;
    socketThread.endLessLoop = false; }
}


void TeleInfoPlugin::read(QString data)
{
    log(data);
    /*
ADCO 040522309029 ;
OPTARIF HC.. <
ISOUSC 45 ?
HCHC 036182765 ,
HCHP 068625207 7
PTEC HP..
IINST 003 Z
IMAX 051 E
PAPP 00590 /
HHPHC A ,
MOTDETAT 000000 B
     */
    QStringList dataList = data.split("\r\n");
    foreach (QString str, dataList) {
        while(str.at(0) == "\r") str = str.remove(0, 1);
        while(str.at(0) == "\n") str = str.remove(0, 1);
        while(str.right(1) == "\r") str.chop(1);
        while(str.right(1) == "\n") str.chop(1);
        QStringList d = str.split(" ");
        if (d.count() >= 3) {
            foreach (teleInfoItem *dev, teleInfoDevices)
            {
                int parameter = dev->ParameterList->currentIndex();
                QString data = str;
                // avoid OPTARIF BASE et BASE
                if (parameter == TeleInfoValeur::BASE) data = data.remove("OPTARIF BASE");
                QString Pstr = TeleInfoParamtoStr(parameter);
                int index = data.indexOf(Pstr);
                QString txt;
                QString Value;
                if (index != -1)
                {
                    int l = Pstr.length() + 1;
                    int chkLength = l + TeleInfoValeurLength(parameter) + (horodatage(parameter) * 14) + 2; // +2 to join SP and checksum
                    QString chk = data.mid(index, chkLength);
                    QByteArray byteArray = chk.toUtf8();
                    const char* cString = byteArray.constData();
                    unsigned char checksum_Mode1 = 0;
                    unsigned char checksum_Mode2 = 0;

                    for (int c=0; c<chkLength-1; c++)		// -2 don't count last SP and checksum
                    {
                        if (c<chkLength-2) checksum_Mode1 += *cString;
                        checksum_Mode2 += *cString;
                        txt += QString("[%1]").arg(*cString);
                        cString++;
                    }
                    checksum_Mode1 &= 0x3F;
                    checksum_Mode1 += 0x20;

                    checksum_Mode2 &= 0x3F;
                    checksum_Mode2 += 0x20;

                    Value = data.mid(index + l + (horodatage(parameter) * 14), TeleInfoValeurLength(parameter));

                    if ((checksum_Mode1 == *cString) || (checksum_Mode2 == *cString) )
                    {
                    // checksum valid set value
                        bool setValue = false;
                        dev->readValue->setText(Value);
                        int m = QDateTime::currentDateTime().time().minute();
                        int h = QDateTime::currentDateTime().time().hour();
                        int d = QDateTime::currentDateTime().date().day();
                        int w = QDateTime::currentDateTime().date().weekNumber();
                        int M = QDateTime::currentDateTime().date().month();

                        if (dev->lastMinute == -1) {
                            setValue = true;
                            dev->lastMinute = m;
                            dev->lastHour = h;
                            dev->lastDay = d;
                            dev->lastWeek = w;
                            dev->lastMonth = M;
                        }

                        switch (dev->interval->currentIndex()) {
                            case Constant : setValue = true; break;
                            case t1mn : if (dev->lastMinute != m) setValue = true; break;
                            case t2mn : if (dev->lastMinute % 2 == 0) setValue = true; break;
                            case t5mn : if (dev->lastMinute % 5 == 0) setValue = true; break;
                            case t10mn : if (dev->lastMinute % 10 == 0) setValue = true; break;
                            case t30mn : if (dev->lastMinute % 30 == 0) setValue = true; break;
                            case t1hour : if (dev->lastHour != h) setValue = true; break;
                            case t1day : if (dev->lastDay != d) setValue = true; break;
                            case t1week : if (dev->lastDay != w) setValue = true; break;
                            case t1month : if (dev->lastMonth != M) setValue = true; break;
                        }
                        int lastValue, value, v, offset;
                        if (setValue) {
                            dev->lastMinute = m;
                            dev->lastHour = h;
                            dev->lastDay = d;
                            dev->lastWeek = w;
                            dev->lastMonth = M;
                            bool ok;
                            Value.toDouble(&ok);
                            if (!ok) Value = translateMainValue(dev, Value);
                            switch (dev->counterMode->currentIndex()) {
                               case modeNormal :
                                    dev->lastValue->setText(dev->value->text());
                                    dev->value->setText(Value);
                                    emit(newDeviceValue(dev->RomID, Value));
                                    dev->setValue->setText(Value);
                                    break;
                                case modeRelative :
                                    bool ok;
                                    lastValue = dev->lastValue->text().toInt(&ok);
                                    if (ok) {
                                        value = Value.toInt(&ok);
                                        if (ok) {
                                            v = value - lastValue;
                                            dev->lastValue->setText(dev->value->text());
                                            dev->value->setText(Value);
                                            QString vStr = QString("%1").arg(v);
                                            emit(newDeviceValue(dev->RomID, vStr));
                                            dev->setValue->setText(vStr);
                                        }
                                    }
                                    else {  // first read, set value and last value to the same and send 0
                                        dev->lastValue->setText(Value);
                                        dev->value->setText(Value);
                                        emit(newDeviceValue(dev->RomID, "0"));
                                        dev->setValue->setText("0");
                                    }
                                    break;
                                case modeOffset :
                                    dev->lastValue->setText(dev->value->text());
                                    offset = dev->offset->text().toInt(&ok);
                                    if (ok) {
                                        value = Value.toInt(&ok);
                                        if (ok) {
                                            v = value - offset;
                                            dev->lastValue->setText(dev->value->text());
                                            dev->value->setText(Value);
                                            QString vStr = QString("%1").arg(v);
                                            emit(newDeviceValue(dev->RomID, vStr)); dev->value->setText(vStr);
                                            dev->setValue->setText(vStr);
                                        }
                                    }
                                    break;
                            }
                        }
                    }
                }
            }
        }
    }
}





QString TeleInfoPlugin::translateMainValue(teleInfoItem *dev, QString &str)
{
    QString v = NA;
    int index = dev->ParameterList->currentIndex();
    switch(index)
    {
        case TeleInfoValeur::OPTARIF :
                if (str == "BASE") v = "0";
                else if (str == "HC..") v = "1";
                else if (str == "EJP.") v = "2";
                else v = NA;
                break;
        case TeleInfoValeur::PTEC :
                if (str == "HP..") v = "0";
                else if (str == "HC..") v = "1";
                else if (str == "TH..") v = "2";
                else if (str == "HN..") v = "3";
                else if (str == "PM..") v = "4";
                else if (str == "HCJB") v = "5";
                else if (str == "HCJW") v = "6";
                else if (str == "HCJR") v = "7";
                else if (str == "HPJB") v = "8";
                else if (str == "HPJW") v = "9";
                else if (str == "HPJR") v = "10";
                else v = NA;
                break;
    case TeleInfoValeur::DEMAIN :
                if (str == "----") v = "0";
                else if (str == "BLEU") v = "1";
                else if (str == "BLAN") v = "2";
                else if (str == "ROUG") v = "3";
                break;
    }
    return v;
}



QString TeleInfoPlugin::ip2Hex(const QString &ip)
{
    bool ok;
    int p1 = ip.indexOf(".");		// get first point position
    if (p1 == -1) return "";
    int p2 = ip.indexOf(".", p1 + 1);	// get second point position
    if (p2 == -1) return "";
    int p3 = ip.indexOf(".", p2 + 1);	// get third point position
    if (p3 == -1) return "";
    int l = ip.length();
    QString N1 = ip.mid(0, p1);
    if (N1 == "") return "";
    QString N2 = ip.mid(p1 + 1, p2 - p1 - 1);
    if (N2 == "") return "";
    QString N3 = ip.mid(p2 + 1, p3 - p2 - 1);
    if (N3 == "") return "";
    QString N4 = ip.mid(p3 + 1, l - p3 - 1);
    if (N4 == "") return "";
    int n1 = N1.toInt(&ok);
    if (!ok) return "";
    int n2 = N2.toInt(&ok);
    if (!ok) return "";
    int n3 = N3.toInt(&ok);
    if (!ok) return "";
    int n4 = N4.toInt(&ok);
    if (!ok) return "";
    return QString("%1%2%3%4").arg(n1, 2, 16, QLatin1Char('0')).arg(n2, 2, 16, QLatin1Char('0')).arg(n3, 2, 16, QLatin1Char('0')).arg(n4, 2, 16, QLatin1Char('0')).toUpper();
}



void TeleInfoPlugin::PortEdited(QString p)
{
    socketThread.port = quint16(p.toInt());
}

void TeleInfoPlugin::log(const QString str)
{
    if (mui->checkBoxLog->isChecked())
    {
        mui->logTxt->clear();
        mui->logTxt->append(QDateTime::currentDateTime().toString("HH:mm:ss"));
        mui->logTxt->append(str);
    }
}


void TeleInfoPlugin::showLog()
{
}



void TeleInfoPlugin::on_editName_editingFinished()
{
    emit(updateInterfaceName(this, mui->editName->text()));
}



bool TeleInfoPlugin::isIpValid(QString str)
{
    QStringList p = str.split(".");
    if (p.count() != 4) return false;
    bool ok;
    int n = p.at(0).toInt(&ok);
    if (!ok | (n < 0) | (n > 255)) return false;
    n = p.at(1).toInt(&ok);
    if (!ok | (n < 0) | (n > 255)) return false;
    n = p.at(2).toInt(&ok);
    if (!ok | (n < 0) | (n > 255)) return false;
    n = p.at(3).toInt(&ok);
    if (!ok | (n < 0) | (n > 255)) return false;
    return true;
}


bool TeleInfoPlugin::isPortValid(QString str)
{
    bool ok;
    int port = str.toInt(&ok);
    if (!ok | (port < 0) | (port > 65535)) return false;
    return true;
}



QString TeleInfoPlugin::buildRomID(int n)
{
    QString portHex = QString("%1").arg(socketThread.port, 4, 16, QLatin1Char('0')).toUpper();
    QString ipHex = ip2Hex(socketThread.ip);
    QString id = QString("%1").arg(n, 3, 10, QLatin1Char('0')).toUpper();
    QString RomID = ipHex +  portHex + id + "TI";
    return RomID;
}




teleInfoItem *TeleInfoPlugin::addDevice(QString id)
{
// Get table size
    bool ok;
    int count = id.right(5).left(3).toInt(&ok) - 1;
    if (count > mui->deviceTable->rowCount()) count = mui->deviceTable->rowCount();
// add one line
    mui->deviceTable->insertRow(count);
// Generate RomID
    if (id.isEmpty()) id = buildRomID(count + 1);
// add device in the QHash store
    teleInfoItem *newDev = new teleInfoItem(id);
    if (!newDev) return nullptr;
    teleInfoDevices.insert(count, newDev);
// add RomID widget in the table
    QTableWidgetItem *item = new QTableWidgetItem(id);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);  // set RomID widget not editable
    mui->deviceTable->setItem(count, 0, item);
// add ParameterList combo input item
    mui->deviceTable->setCellWidget(count, 1, newDev->ParameterList);
// add counterMode input item
    mui->deviceTable->setCellWidget(count, 2, newDev->counterMode);
// set Offset item
    mui->deviceTable->setItem(count, 3, newDev->offset);
// add Read Interval combo input item
    mui->deviceTable->setCellWidget(count, 4, newDev->interval);
// set readValue item
    mui->deviceTable->setItem(count, 5, newDev->readValue);
// set Value item
    mui->deviceTable->setItem(count, 6, newDev->value);
// set Last Value item
    mui->deviceTable->setItem(count, 7, newDev->lastValue);
// set setValue item
    mui->deviceTable->setItem(count, 8, newDev->setValue);
    emit(newDevice(this, id));
    return newDev;
}


void TeleInfoPlugin::on_AddButton_clicked()
{
    if (!isIpValid(socketThread.ip)) {
        QMessageBox msgBox;
        msgBox.setText("IP address is not valid");
        msgBox.exec();
        return;
    }
    if (!isPortValid(mui->editPort->text())) {
        QMessageBox msgBox;
        msgBox.setText("Port is not valid");
        msgBox.exec();
        return;
    }
    int count = 1;
    bool found = true;
    while (found) {
        found = false;
        QString id = buildRomID(count);
        foreach (teleInfoItem *dev, teleInfoDevices) {
            if (dev->RomID == id) found = true;  }
        if (found) count++;
    }
    addDevice(buildRomID(count));
    //qDebug() << QString ("Count %1").arg(count);
    //sortDevicesList();
}


void TeleInfoPlugin::on_RemoveButton_clicked()
{
    QModelIndexList indexList = mui->deviceTable->selectionModel()->selectedIndexes();
    int row = 0;
    foreach (QModelIndex index, indexList) { row = index.row(); }
    foreach (teleInfoItem *dev, teleInfoDevices) {
        if (dev->RomID == mui->deviceTable->item(row, 0)->text()) {
            teleInfoDevices.removeAll(dev);
            mui->deviceTable->removeRow(row);
            mui->RemoveButton->setEnabled(false);
            break;
        }
    }
    // renumber devices
    int id = 0;
    foreach (teleInfoItem *dev, teleInfoDevices) {
            dev->RomID = buildRomID(id+1);
            mui->deviceTable->item(id++, 0)->setText(dev->RomID);
        }
}
