#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    /*QFile logFile(QString("/VSS/log"));

    if(logFile.open(QIODevice::WriteOnly))
    {
        logFile.write("CBIT Module log file\n");
        logFile.close();
    }*/

    //Logging("Starting CBIT\n");
    ui->setupUi(this);

    ui->showV1->setEnabled(false);
    ui->showV2->setEnabled(false);
    ui->showV1->setStyleSheet("color: rgb(198,189,193)");
    ui->showV2->setStyleSheet("color: rgb(198,189,193)");
    ui->showV1->setIconSize(QSize(75,55));
    ui->showV2->setIconSize(QSize(75,55));


    ///////////////////////////////////////////////////////////////////////////////////
    /// Initialise png for signal
    ///
    ///////////////////////////////////////////////////////////////////////////////////

    pixNS.load("/VSS/Signal/NS.png");
    pixPS.load("/VSS/Signal/PS.png");
    pixMS.load("/VSS/Signal/MS.png");
    pixGS.load("/VSS/Signal/GS.png");
    pixES.load("/VSS/Signal/ES.png");

    ///////////////////////////////////////////////////////////////////////////////////
    /// Initialise png for button
    ///
    ///////////////////////////////////////////////////////////////////////////////////

    pixV1Off.load("/VSS/button/V1Off.png");
    pixV1On.load("/VSS/button/V1On.png");
    pixV2Off.load("/VSS/button/V2Off.png");
    pixV2On.load("/VSS/button/V2On.png");
    pixP1Off.load("/VSS/button/P1Off.png");
    pixP1On.load("/VSS/button/P1On.png");
    pixP2Off.load("/VSS/button/P2Off.png");
    pixP2On.load("/VSS/button/P2On.png");
    pixP3Off.load("/VSS/button/P3Off.png");
    pixP3On.load("/VSS/button/P3On.png");
    pixP4Off.load("/VSS/button/P4Off.png");
    pixP4On.load("/VSS/button/P4On.png");

    ///////////////////////////////////////////////////////////////////////////////////
    /// Open Crash Report
    /// Reading of Crash File to check program crash before
    ///
    ///////////////////////////////////////////////////////////////////////////////////

    //Logging("Reading Crash Report\n");
    int crashValue = 0;
    QFile fileCrashReport(QString("/VSS/CrashReport"));
    if (fileCrashReport.open(QIODevice::ReadOnly))
    {
       QTextStream in(&fileCrashReport);
       while (!in.atEnd())
       {
          crash_value = QString(fileCrashReport.readAll()).split('\n');
       }
       fileCrashReport.close();

       if (crash_value.isEmpty() == false && (crash_value.size() == 1 || crash_value.size() == 2))
       {
            crashValue = crash_value[0].toInt();
            //Logging("Crash Report Read successful\n");

            if(crashValue==0)
            {
                //Logging("CBIT Program did not crash before\n");
                crash = false;
                //Logging("Updating Crash Report Status\n");
                QFile fileCrashReport2(QString("/VSS/CrashReport"));
                if(fileCrashReport2.open(QIODevice::WriteOnly | QIODevice::Text))
                {
                    QTextStream stream1(&fileCrashReport2);
                    stream1 << "1";
                    fileCrashReport2.close();
                    //Logging("Updated Crash Report Status Completed\n");
                }
            }
            else
            {
                crash = true;
                //Logging("CBIT Program Crashed upon Startup...Please check the system.\n");
            }
       }
       else
       {
           //Logging("Crash Report Parameter is wrong. Please check crash report file\n");
       }
    }
    else
    {
        //Logging("Crash Report does not exist. Please create a Crash Report file\n");
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    /// Open Encoder Config
    /// Reading of Encoder Config
    ///
    ////////////////////////////////////////////////////////////////////////////////////////

    //Logging("Reading encoder config\n");

    QFile fileEncoder(QString("/VSS/encoder"));

    if (fileEncoder.open(QIODevice::ReadOnly))
    {

       QTextStream in(&fileEncoder);
       while (!in.atEnd())
       {
          encoder_URL = QString(fileEncoder.readAll()).split('\n');
       }
       fileEncoder.close();

       if (encoder_URL.isEmpty() == false && encoder_URL.size() == 3)
       {
            encoder1 = encoder_URL[0].split(',');
            encoder2 = encoder_URL[1].split(',');
            //Logging("Encoder config read successful\n");
       }
       else
       {
           //Logging("Encoder config Parameter is wrong. Please check encoder config file\n");
       }
    }
    else
    {
        //Logging("Encoder config file does not exist. Please check encoder config file\n");
    }

    /////////////////////////////////////////////////////////////////////////////////////
    /// Open IP address Config
    /// Get equipment IP address values
    ///
    /////////////////////////////////////////////////////////////////////////////////////

    //Logging("Reading IPaddress Config\n");

    QFile fileIPAdd(QString("/VSS/IPadd"));

    /*Open file and read file*/
    if (fileIPAdd.open(QIODevice::ReadOnly))
    {

       QTextStream in(&fileIPAdd);
       while (!in.atEnd())
       {
          IP_address = QString(fileIPAdd.readAll()).split('\n');
       }
       fileIPAdd.close();

       if (IP_address.isEmpty() == false && IP_address.size() == 10)
       {
            routerIP = IP_address[0].split(':');        //get ip address of router
            manageSwitchIP = IP_address[1].split(':');  //get ip address of managed switch
            vEncoder1IP = IP_address[2].split(':');     //get ip address of encoder 1
            vEncoder2IP = IP_address[3].split(':');     //get ip address of encoder 2
            simIP = IP_address[4].split(':');           //get sim card ip address
            manageSwitchP1IP = IP_address[5].split(':');           //get ip address of managed switch P1
            manageSwitchP2IP = IP_address[6].split(':');           //get ip address of managed switch P2
            manageSwitchP3IP = IP_address[7].split(':');           //get ip address of managed switch P3
            manageSwitchP4IP = IP_address[8].split(':');           //get ip address of managed switch P4

            //Logging("IPaddress config read successful\n");
       }
       else
       {
           //Logging("IPaddress config Parameter is wrong. Please check ipaddress config file\n");
       }

    }
    else
    {
        //Logging("IPaddress config file does not exist. Please check IPaddress config file\n");
    }

    /////////////////////////////////////////////////////////////////////////////////////
    /// Open SNMP Config
    /// Get SNMP Parameter
    ///
    /////////////////////////////////////////////////////////////////////////////////////

/*    QFile fileSNMPConfig(QString("/VSS/snmpconf"));

    if (fileSNMPConfig.open(QIODevice::ReadOnly))
    {
       QTextStream in(&fileSNMPConfig);
       while (!in.atEnd())
       {
          snmp_Conf = QString(fileSNMPConfig.readAll()).split('\n');
       }
       fileSNMPConfig.close();
       if (snmp_Conf.isEmpty() == false && snmp_Conf.size() == 2)
       {
            snmpConf = snmp_Conf[0].split(',');
            Logging("SNMP config read successful\n");
       }
       else
       {
           Logging("SNMP config Parameter is wrong. Please check SNMP config file\n");
       }

    }
    else
    {
        Logging("SNMP config file does not exist. Please check SNMP file\n");
    }

    */
    /////////////////////////////////////////////////////////////////////////////////////
    /// Open Timer Config
    /// Get timer values
    ///
    /////////////////////////////////////////////////////////////////////////////////////

    //Logging("Reading Timer Config\n");

    QFile fileTimer(QString("/VSS/timer"));  //read file for timer

    int timerValue1 = 0;
    float fTimerValue = 0.00;
    float fTimerValue4 = 0.00;

    if (fileTimer.open(QIODevice::ReadOnly))
    {

       QTextStream in(&fileTimer);
       while (!in.atEnd())
       {
          timer_value = QString(fileTimer.readAll()).split('\n');
       }
       fileTimer.close();

       if (timer_value.isEmpty() == false && timer_value.size() == 6)
       {

           if(!crash)
           {
               timerValue = timer_value[0].toInt();
               //Logging("Timer config read successful\n");
           }

           else
           {
               timerValue = 0;
               systemCheck = true;
               CheckVPNandIPAddress();
               CheckLTERouter();
               CheckManagedSwitch();
               CheckManagedSwitchP1();
               CheckManagedSwitchP2();
               CheckManagedSwitchP3();
               CheckManagedSwitchP4();
               CheckEncoder1();
               CheckEncoder2();
               CheckBandwidth();
               CheckSignal();
               //Logging("Timer config read successful\n");
           }
           timerValue1 = timer_value[1].toInt();
           timerValue2 = timer_value[2].toInt();
           timerValue3 = timer_value[3].toInt();
           timerValue4 = timer_value[4].toInt();

           fTimerValue = timer_value[0].toFloat();
           fTimerValue4 = timer_value[4].toFloat();
       }
       else
       {
           //Logging("Timer config Parameter is wrong. Please check timer config file\n");
       }

    }
    else
    {
        //Logging("Timer config file does not exist. Please check timer config file\n");
    }


    timeTotal = qRound(fTimerValue / fTimerValue4);

    QString timeDisplayedString = QString::number(timeTotal);
    timeDisplayedLabel = "in " + timeDisplayedString + " minutes...";
    ui->countdown_label_2->setGeometry(150,120,281,41);
    ui->countdown_label_2->setText(timeDisplayedLabel);

    timerSystemCheck = new QTimer();//timer2
    timerSystemCheck->singleShot(timerValue, this, SLOT(StartSystemCheck()));

    timerLTERouter = new QTimer();//timer
    connect(timerLTERouter, SIGNAL(timeout()), this, SLOT(CheckLTERouter()));
    timerLTERouter->start(timerValue1);

    timerVPN = new QTimer();//timer1
    connect(timerVPN, SIGNAL(timeout()), this, SLOT(CheckVPNandIPAddress()));
    timerVPN->start(timerValue1);

    timerSwitch = new QTimer();//timer3
    connect(timerSwitch, SIGNAL(timeout()), this, SLOT(CheckManagedSwitch()));
    timerSwitch->start(timerValue1);

    timerEncoder1 = new QTimer();//timer4
    connect(timerEncoder1, SIGNAL(timeout()), this, SLOT(CheckEncoder1()));
    timerEncoder1->start(timerValue1);

    timerEncoder2 = new QTimer();//timer5
    connect(timerEncoder2, SIGNAL(timeout()), this, SLOT(CheckEncoder2()));
    timerEncoder2->start(timerValue1);

    timerBandwidth = new QTimer();//timer6
    connect(timerBandwidth, SIGNAL(timeout()), this, SLOT(CheckBandwidth()));
    timerBandwidth->start(timerValue2);

    timerSignal = new QTimer();//timer9
    connect(timerSignal, SIGNAL(timeout()), this, SLOT(CheckSignal()));
    timerSignal->start(timerValue2);

    timerStartupPage = new QTimer();//timer8
    connect(timerStartupPage, SIGNAL(timeout()), this, SLOT(UpdateStartUpPage()));
    timerStartupPage->start(timerValue4);

    timerClock = new QTimer();
    connect(timerClock, SIGNAL(timeout()), this, SLOT(UpdateClock()));
    timerClock->start(1000);

    timerSwitchP1 = new QTimer();//timer10
    connect(timerSwitchP1, SIGNAL(timeout()), this, SLOT(CheckManagedSwitchP1()));
    timerSwitchP1->start(timerValue1);

    timerSwitchP2 = new QTimer();//timer11
    connect(timerSwitchP2, SIGNAL(timeout()), this, SLOT(CheckManagedSwitchP2()));
    timerSwitchP2->start(timerValue1);

    timerSwitchP3 = new QTimer();//timer12
    connect(timerSwitchP3, SIGNAL(timeout()), this, SLOT(CheckManagedSwitchP3()));
    timerSwitchP3->start(timerValue1);

    timerSwitchP4 = new QTimer();//timer13
    connect(timerSwitchP4, SIGNAL(timeout()), this, SLOT(CheckManagedSwitchP4()));
    timerSwitchP4->start(timerValue1);

}

MainWindow::~MainWindow()
{
    delete ui;
}

/****************************************************************************************
 * Function Name:
 * Args:
 * Returns:
 * Task:
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
/****************************************************************************************
 * Function Name: StartSystemCheck
 * Args: NULL
 * Returns: NULL
 * Task:
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::StartSystemCheck()
{
    ui->widget->setVisible(false);
    ui->countdown_label->setVisible(false);
    ui->countdown_label_2->setVisible(false);
    ui->countdown_label_3->setVisible(false);
    timerStartupPage->stop();
    systemCheck = true;
}

/****************************************************************************************
 * Function Name: CheckVPNResult
 * Args: NULL
 * Returns: NULL
 * Task: To check VPN tunnel established result from slot
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CheckVPNResult()
{
    //Logging("Retrieving VPN Tunnel Status....\n");
    disconnect(&snmpGet_VPNtunnel, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckVPNResult()));

    QByteArray result_snmp = snmpGet_VPNtunnel.readAll();

    snmpGet_VPNtunnel.kill();
    params_SNMPget.clear();

    if (result_snmp.isNull())
    {
        if(vpnconnected)
        {
            ui->VPN_label->setText("<font color='red'>DISCONNECTED</font>");
            //Logging("Unable to poll VPN Tunnel result even though VPN was established before. Please check connection/router status/SNMP config.\n");
            streaming = false;
        }
        else
        {
            //Logging("Unable to poll VPN Tunnel result. Please check connection/router status/SNMP config.\n");
            ui->VPN_label->setText("Connecting...");
            streaming = false;
        }
    }
    else
    {
        QString result_String_snmp = QString(result_snmp);
        QStringList VPN_Tunnel_Result = result_String_snmp.split("=");

        QString VPN_Tunnel_Result1 = VPN_Tunnel_Result[1].remove(QRegExp("\\n"));

        VPN_Tunnel_Result1.replace(" ","");

        QStringList VPN_Tunnel_Result2 = VPN_Tunnel_Result1.split("i");

        QFile file2(QString("/VSS/snmpResult"));
        if(file2.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream stream(&file2);
            stream << VPN_Tunnel_Result2[0];
            file2.close();
        }

        if (VPN_Tunnel_Result2[0] == "INTEGER:1")
        {
            ui->VPN_label->setText("<font color='green'>CONNECTED</font>");
            //Logging("VPN Tunnel is UP.\n");
            vpnconnected = true;
            streaming = true;
        }
        else
        {
            if(vpnconnected)
            {
                ui->VPN_label->setText("<font color='red'>DISCONNECTED</font>");
                //Logging("VPN Tunnel is DOWN even though VPN tunnel was established before.\n");
                streaming = false;
            }
            else
            {
                ui->VPN_label->setText("Connecting...");
                //Logging("VPN Tunnel is establishing/not establish.\n");
                streaming = false;
            }
        }

    }
}

/****************************************************************************************
 * Function Name: CheckIPAddResult
 * Args: NULL
 * Returns: NULL
 * Task: To retrieve all component IP Addresses from file
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CheckIPAddResult()
{
    //Logging("Retrieving SIM card IP Address....\n");
    disconnect(&snmpGet_IPAdd, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckIPAddResult()));
    QByteArray result_snmpIP = snmpGet_IPAdd.readAll();

    snmpGet_IPAdd.kill();
    params_SNMPIPAcquire.clear();

    if (result_snmpIP.isNull())
    {
        //Logging("Unable to poll SIM card IP address. Please check connection/router status/SNMP config.\n");
        ui->IPAdd_label->setText("<font color='red'>IP Address: Acquiring . . .</font>");
    }

    else
    {
        QString IPresult_String_snmp = QString(result_snmpIP);
        bool IPavailable = IPresult_String_snmp.contains(simIP[1]);

        if(IPavailable)
        {
            ui->IPAdd_label->setText("<font color='green'>IP Address: Acquired</font>");
            //Logging("SIM card IP acquired successfully.\n");
            ui->Bandwidth_label->setVisible(true);
            ipAcquired = true;
            ipFailTime = 0;
        }
        else
        {
            //if (ipAcquired && (signal_result_snmp.isNull() == false || signal_level > 0))
            if (ipAcquired)
            {
                ipFailTime++;
                if (ipFailTime < 10 && (signal_result_snmp.isNull() == false || signal_level > 0))      //check ip fail to acquire is it due to poor signal/no reception? Answer: No
                {
                    ui->IPAdd_label->setText("<font color='green'>IP Address: Acquired</font>");
                    //Logging("SIM card IP acquired fail for " + QVariant(ipFailTime).toString() + " time NOT due to poor signal or no reception.\n");
                    ui->Bandwidth_label->setVisible(true);
                }
                else if (ipFailTime < 10 && (signal_result_snmp.isNull() || signal_level == 0)) //check ip fail to acquire is it due to poor signal/no reception? Answer: Yes
                {
                    ui->IPAdd_label->setText("<font color='green'>IP Address: Acquired</font>");
                    //Logging("SIM card IP acquired fail for " + QVariant(ipFailTime).toString() + " time due to poor signal or no reception.\n");
                    ui->Bandwidth_label->setVisible(true);
                }

                else if(ipFailTime == 10)
                {
                    ui->IPAdd_label->setText("<font color='red'>IP Address: Acquiring...</font>");
                    //Logging("SIM card IP acquired fail for " + QVariant(ipFailTime).toString() + " time.\n");
                    ui->Bandwidth_label->setVisible(false);
                }
            }
            else
            {
                ui->IPAdd_label->setText("<font color='red'>IP Address: Acquiring . . .</font>");
                ui->Bandwidth_label->setVisible(true);
            }
        }

    }

}

/****************************************************************************************
 * Function Name: CheckVPNandIPAddress
 * Args: NULL
 * Returns: NULL
 * Task: to trigger check on VPN status and 4G Sim card IP address
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CheckVPNandIPAddress()
{

    if (systemCheck)
    {

        //checking of VPN status
        //Logging("Polling VPN Tunnel Status....\n");
        connect(&snmpGet_VPNtunnel, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckVPNResult()));
        //params_SNMPget << "-v3" << "-l" << "authPriv" << "-u" << "vssUser" << "-a" << "SHA" << "-A" << snmpConf[2] << "-x" << "AES" << "-X" << snmpConf[3] << snmpConf[0] << snmpConf[1];
        params_SNMPget << "-v3" << "-l" << "authPriv" << "-u" << "vssUser" << "-a" << "SHA" << "-A" << "5nmpHTXUIS#1" << "-x" << "AES" << "-X" << "5nmpHTXUIS#1" << "192.168.10.1" << ".1.3.6.1.4.1.9.9.171.1.3.2.1.51";
        //Logging("Command enter for check VPN Tunnel Status: " + execSNMP + " " + params_SNMPget.join(" ") + "\n");
        snmpGet_VPNtunnel.start(execSNMP,params_SNMPget);


        //check whether IP address acquired
        //Logging("Polling SIM card IP address....\n");
        connect(&snmpGet_IPAdd, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckIPAddResult()));
        //snmpwalk -v3 -l authPriv -u vssUser -a SHA -A 5nmpHTXUIS#1 -x AES -X 5nmpHTXUIS#1 192.168.10.1 .1.3.6.1.4.1.9.9.171.1.3.2.1.51
        //params_SNMPIPAcquire << "-v3" << "-l" << "authPriv" << "-u" << "vssUser" << "-a" << "SHA" << "-A" << snmpConf[2] << "-x" << "AES" << "-X" << snmpConf[3] << snmpConf[0] << snmpConf[4];
        params_SNMPIPAcquire << "-v3" << "-l" << "authPriv" << "-u" << "vssUser" << "-a" << "SHA" << "-A" << "5nmpHTXUIS#1" << "-x" << "AES" << "-X" << "5nmpHTXUIS#1" << "192.168.10.1" << ".1.3.6.1.2.1.4.20.1.1";
        //Logging("Command enter for check SIM card IP: " + execSNMPIPAcquire + " " + params_SNMPIPAcquire.join(" ") + "\n");
        snmpGet_IPAdd.start(execSNMPIPAcquire,params_SNMPIPAcquire);

    }
}

/****************************************************************************************
 * Function Name: CheckBandwidthResult
 * Args: NULL
 * Returns: NULL
 * Task: to get bandwidth result from slot
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CheckBandwidthResult()
{
    //Logging("Calculating Bandwidth Result....\n");
    disconnect(&snmpGet_Bandwidth, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckBandwidthResult()));
    QByteArray bandwidth_result_snmp = snmpGet_Bandwidth.readAll();

    snmpGet_Bandwidth.kill();
    params_SNMPBandwidth.clear();

    if (bandwidth_result_snmp.isNull())
    {
        //Logging("Unable to poll bandwidth result. Please check connection/router status/SNMP config.\n");
        ui->Bandwidth_label->setText("<font color='white'>Bandwidth: 0.00 Mbps </font>");
    }

    else
    {
        QString bandwidth_result_String_snmp = QString(bandwidth_result_snmp);
        QStringList Bandwidth_Result = bandwidth_result_String_snmp.split(":");

        if (Bandwidth_Result.isEmpty() == false && Bandwidth_Result.size() == 2)
        {
            QString Bandwidth_Result1 = Bandwidth_Result[1].remove(QRegExp("\\n"));

            //upon startup
            if(startUp)
            {
                currentvalue = Bandwidth_Result1.toFloat();
                previousvalue = Bandwidth_Result1.toFloat();
                ui->Bandwidth_label->setText("Bandwidth: 0.00 Mbps");
                //Logging("System just start up. No bandwidth utilized.\n");
                startUp = false;
            }
            //after first start up
            else
            {
                currentvalue = Bandwidth_Result1.toFloat();
                bandwidth = ((currentvalue - previousvalue) * 8) / (1024*1024*(timerValue2/1000));
                QString bandwidthString = QString::number(bandwidth, 'f', 2);
                QString bandwidthresultlabel = "Bandwidth: " + bandwidthString + " Mbps";
                //Logging("Bandwidth Utilized: " + bandwidthString + " Mbps.\n");
                ui->Bandwidth_label->setText(bandwidthresultlabel);
                previousvalue = currentvalue;
            }
        }
        else
        {
            //Logging("No Bandwidth value.\n");
        }
    }
}

/****************************************************************************************
 * Function Name: CheckBandwidth
 * Args: NULL
 * Returns: NULL
 * Task: To trigger slot to check bandwidth
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CheckBandwidth()
{

    if (systemCheck)
    {
        //Logging("Polling Bandwidth Utilizied....\n");
        connect(&snmpGet_Bandwidth, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckBandwidthResult()));
        //params_SNMPBandwidth << "-v3" << "-l" << "authPriv" << "-u" << "vssUser" << "-a" << "SHA" << "-A" << snmpConf[2] << "-x" << "AES" << "-X" << snmpConf[3] << snmpConf[0] << snmpConf[5];
        params_SNMPBandwidth << "-v3" << "-l" << "authPriv" << "-u" << "vssUser" << "-a" << "SHA" << "-A" << "5nmpHTXUIS#1" << "-x" << "AES" << "-X" << "5nmpHTXUIS#1" << "192.168.10.1" << ".1.3.6.1.2.1.2.2.1.16.6";
        //Logging("Command enter for checking bandwidth: " + execSNMPBandwidth + " " + params_SNMPBandwidth.join(" ") + "\n");
        snmpGet_Bandwidth.start(execSNMPBandwidth,params_SNMPBandwidth);
    }
}

/****************************************************************************************
 * Function Name: CheckSignalResult
 * Args: NULL
 * Returns: NULL
 * Task: To check the signal result from slot
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CheckSignalResult()
{
    //Logging("Tabulating Signal Data Result from Router....\n");
    disconnect(&snmpGet_Signal, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckSignalResult()));
    //QByteArray signal_result_snmp = snmpGet_Signal.readAll();
    signal_result_snmp = snmpGet_Signal.readAll();
    snmpGet_Signal.kill();
    params_SNMPSignal.clear();

    if (signal_result_snmp.isNull())
    {
        ui->signal_label->setPixmap(pixNS);
        //Logging("Unable to poll signal result. Please check connection/router status/SNMP config.\n");
    }

    else
    {
            QString signal_result_String_snmp = QString(signal_result_snmp);
            QStringList Signal_Result = signal_result_String_snmp.split(":");

            if (Signal_Result.isEmpty() == false && Signal_Result.size() == 2)
            {
                //Logging("SNMP Polled successfully\n");
                QString Signal_Result1 = Signal_Result[1].remove(QRegExp("\\n"));
                Signal_Result1.replace(" ","");

                signal_level = Signal_Result1.toInt();

                if (signal_level < -100)
                {
                    ui->signal_label->setPixmap(pixPS);
                    ui->signal_label->setScaledContents(true);
                    //Logging("Poor Signal -> Less than -100dBm\n");
                }
                else if (-99 <= signal_level && signal_level < -89)
                {
                    ui->signal_label->setPixmap(pixMS);
                    ui->signal_label->setScaledContents(true);
                    //Logging("Medium Signal -> More than -99dBm and less than -89dBm\n");
                }
                else if (-89 <= signal_level && signal_level < -69)
                {
                    ui->signal_label->setPixmap(pixGS);
                    ui->signal_label->setScaledContents(true);
                    //Logging("Good Signal -> More than -89dBm and less than -69dBm\n");
                }
                else if (-69 <= signal_level)
                {
                    ui->signal_label->setPixmap(pixES);
                    ui->signal_label->setScaledContents(true);
                    //Logging("Excellent Signal -> More than -69dBm\n");
                }
                else if (signal_level == 0)
                {
                    ui->signal_label->setPixmap(pixNS);
                    ui->signal_label->setScaledContents(true);
                    //Logging("No Signal -> Please check antenna\n");
                }
            }
            else
            {
                //Logging("No Value for Signal.\n");
            }

    }

}

/****************************************************************************************
 * Function Name: checkSignal
 * Args: NULL
 * Returns: NULL
 * Task: To trigger slot to check signal
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CheckSignal()
{

    if (systemCheck)
    {
        //Logging("Polling Signal Data from Router....\n");
        connect(&snmpGet_Signal, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckSignalResult()));
        //params_SNMPSignal << "-v3" << "-l" << "authPriv" << "-u" << "vssUser" << "-a" << "SHA" << "-A" << snmpConf[2] << "-x" << "AES" << "-X" << snmpConf[3] << snmpConf[0] << snmpConf[6];
        params_SNMPSignal << "-v3" << "-l" << "authPriv" << "-u" << "vssUser" << "-a" << "SHA" << "-A" << "5nmpHTXUIS#1" << "-x" << "AES" << "-X" << "5nmpHTXUIS#1" << "192.168.10.1" << ".1.3.6.1.4.1.9.9.661.1.3.4.1.1.1.4002";
        //Logging("Command enter for checking signal: " + execSNMPSignal + " " + params_SNMPSignal.join(" ") + "\n");
        snmpGet_Signal.start(execSNMPSignal,params_SNMPSignal);
    }
}

/****************************************************************************************
 * Function Name: CheckLTERouterPingResult
 * Args: NULL
 * Returns: NULL
 * Task: To check the ping result of Cisco Cellular Router from slot
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CheckLTERouterPingResult()
{
    //Logging("Retrieving Router Ping Result....\n");
    QByteArray result_VPN = ping_VPN.readAll();
    disconnect(&ping_VPN, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckLTERouterPingResult()));

    ping_VPN.kill();
    params_VPN.clear();

    if (result_VPN.isNull())
    {
        ui->Router_label->setText("<font color='red'>OFFLINE</font>");
        //Logging("Unable to ping Router\n");
        ping_VPN.terminate();
        params_VPN.clear();
    }
    else
    {
        ping_VPN.terminate();
        params_VPN.clear();

        QString result1_VPN = QString(result_VPN);
        QStringList result2_VPN = result1_VPN.split("\n");

        QString vpnStatus = result2_VPN[result2_VPN.length()-6];
        QStringList vpnStatus1 = vpnStatus.split("ttl");
        if (vpnStatus1.length() == 1)
        {
            ui->Router_label->setText("<font color='red'>OFFLINE</font>");
            //Logging("Router is offline\n");
        }
        else
        {
            ui->Router_label->setText("<font color='green'>ONLINE</font>");
           //Logging("Router is online\n");
        }
    }

}

/****************************************************************************************
 * Function Name: CheckLTERouter
 * Args: NULL
 * Returns: NULL
 * Task: To trigger slot to check Cisco Cellular Router
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CheckLTERouter()
{
    if (systemCheck)
    {
        //Logging("Pinging Router....\n");
        connect(&ping_VPN, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckLTERouterPingResult()));

        //VPN router ping test
        if (routerIP.isEmpty() == false && routerIP.size() == 2)
        {
            params_VPN << "-c" << "4" << routerIP[1];
            //Logging("Command enter for pinging Router: " + exec + " " + params_VPN.join(" ") + "\n");
            ping_VPN.start(exec,params_VPN);
        }
        else
        {
            //Logging("Router IP can't be found. Please check IP conf file.\n");
        }
    }

}

/****************************************************************************************
 * Function Name: CheckManagedSwitchPingResult
 * Args: NULL
 * Returns: NULL
 * Task: To check the MOXA managed switch ping result from slot
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CheckManagedSwitchPingResult()
{
    //Logging("Retrieving Managed Switch Ping Result....\n");
    QByteArray result_Switch = ping_Switch.readAll();

    disconnect(&ping_Switch, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckManagedSwitchPingResult()));
    ping_Switch.kill();
    params_Switch.clear();

    if (result_Switch.isNull())
    {
        ui->Switch_label->setText("<font color='red'>OFFLINE</font>");
        ping_Switch.terminate();
        params_Switch.clear();
        //Logging("Unable to ping Managed Switch\n");
    }
    else
    {

        ping_Switch.terminate();
        params_Switch.clear();

        QString result1_Switch = QString(result_Switch);
        QStringList result2_Switch = result1_Switch.split("\n");

        QString switchStatus = result2_Switch[result2_Switch.length()-6];
        QStringList switchStatus1 = switchStatus.split("ttl");
        if (switchStatus1.length() == 1)
        {
            ui->Switch_label->setText("<font color='red'>OFFLINE</font>");
            //Logging("Managed Switch is offline\n");
        }
        else
        {
            ui->Switch_label->setText("<font color='green'>ONLINE</font>");
            //Logging("Managed Switch is online\n");
        }
    }
}

/****************************************************************************************
 * Function Name: CheckManagedSwitch
 * Args: NULL
 * Returns: MULL
 * Task: To trigger slot to check on the managed switch
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CheckManagedSwitch()
{
    if (systemCheck)
    {
        //Logging("Pinging Managed Switch....\n");
        connect(&ping_Switch, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckManagedSwitchPingResult()));

        //managed switch ping test
        if (manageSwitchIP.isEmpty() == false && manageSwitchIP.size() == 2)
        {
        params_Switch << "-c" << "4" << manageSwitchIP[1];
        //Logging("Command enter for pinging Managed Switch: " + exec + " " + params_Switch.join(" ") + "\n");
        ping_Switch.start(exec,params_Switch);
        }
        else
        {
            //Logging("Managed Switch IP can't be found. Please check IP conf file.\n");
        }
    }
}

/****************************************************************************************
 * Function Name: CheckEncoder1PingResult
 * Args: NULL
 * Returns: NULL
 * Task: To check on the ping result of the encoder 1 from slot
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CheckEncoder1PingResult()
{
    //Logging("Retrieving Encoder 1 Ping Result....\n");
    QByteArray result_Encoder1 = ping_Encoder1.readAll();
    disconnect(&ping_Encoder1, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckEncoder1PingResult()));
    ping_Encoder1.kill();
    params_Encoder1.clear();

    if (result_Encoder1.isNull())
    {
        if(!streaming)
        {
            ui->Encoder1_label->setText("<font color='red'>OFFLINE</font>");
            //Logging("Unable to ping Encoder 1 and VPN Tunnel is not established\n");
            ui->showV1->setEnabled(false);
            ui->showV1->setIcon(pixV1Off);
            ui->showV1->setStyleSheet("color: rgb(198,189,193)");
            ui->showV1->setIconSize(QSize(75,55));
        }
        else
        {
            ui->Encoder1_label->setText("<font color='red'>NOT STREAMING</font>");
            //Logging("Unable to ping Encoder 1 and VPN Tunnel is established\n");
            ui->showV1->setEnabled(false);
            ui->showV1->setIcon(pixV1Off);
            ui->showV1->setStyleSheet("color: rgb(198,189,193)");
            ui->showV1->setIconSize(QSize(75,55));
        }
    }
    else
    {
        ui->showV1->setEnabled(false);
        ui->showV1->setIcon(pixV1Off);
        ui->showV1->setIconSize(QSize(75,55));

        QString result1_Encoder1 = QString(result_Encoder1);
        QStringList result2_Encoder1 = result1_Encoder1.split("\n");

        QString encoder1Status = result2_Encoder1[result2_Encoder1.length()-6];
        QStringList encoder1Status1 = encoder1Status.split("ttl");
        if (encoder1Status1.length() == 1)
        {
            if(!streaming)
            {
                ui->Encoder1_label->setText("<font color='red'>OFFLINE</font>");
                //Logging("Encoder 1 is offline and VPN Tunnel is not established\n");
                ui->showV1->setEnabled(false);
                ui->showV1->setIcon(pixV1Off);
                ui->showV1->setStyleSheet("color: rgb(198,189,193)");
                ui->showV1->setIconSize(QSize(75,55));
            }
            else
            {
                ui->Encoder1_label->setText("<font color='red'>NOT STREAMING</font>");
                //Logging("Encoder 1 is offline and VPN Tunnel is established\n");
                ui->showV1->setEnabled(false);
                ui->showV1->setIcon(pixV1Off);
                ui->showV1->setStyleSheet("color: rgb(198,189,193)");
                ui->showV1->setIconSize(QSize(75,55));
            }
        }
        else
        {
            if(!streaming)
            {
                ui->Encoder1_label->setText("<font color='green'>ONLINE</font>");
                //Logging("Encoder 1 is online and VPN Tunnel is not established\n");
                ui->showV1->setStyleSheet("color: rgb(35,38,39)");
                ui->showV1->setIcon(pixV1On);
                ui->showV1->setEnabled(true);
                ui->showV1->setIconSize(QSize(75,55));
            }
            else
            {
                ui->Encoder1_label->setText("<font color='green'>STREAMING</font>");
                //Logging("Encoder 1 is online and VPN Tunnel is established\n");
                ui->showV1->setStyleSheet("color: rgb(35,38,39)");
                ui->showV1->setIcon(pixV1On);
                ui->showV1->setEnabled(true);
                ui->showV1->setIconSize(QSize(75,55));
            }
        }
    }
}

/****************************************************************************************
 * Function Name: CheckEncoder1
 * Args: NULL
 * Returns: NULL
 * Task: To trigger slot to check on Encoder 1
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CheckEncoder1()
{
    if (systemCheck)
    {
        //Logging("Pinging Encoder 1....\n");
        connect(&ping_Encoder1, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckEncoder1PingResult()));

        //Encoder 1 ping test
        if (vEncoder1IP.isEmpty() == false && vEncoder1IP.size() == 2)
        {
            params_Encoder1 << "-c" << "4" << vEncoder1IP[1];
            //Logging("Command enter for pinging Encoder 1: " + exec + " " + params_Encoder1.join(" ") + "\n");
            ping_Encoder1.start(exec,params_Encoder1);
        }
        else
        {
            //Logging("Encoder 1 IP can't be found. Please check IP conf file.\n");
        }
    }
}

/****************************************************************************************
 * Function Name: CheckEncoder2PingResult
 * Args: NULL
 * Returns: NULL
 * Task: To check on the ping result of encder 2 from slot
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CheckEncoder2PingResult()
{
    //Logging("Retrieving Encoder 2 Ping Result....\n");
    QByteArray result_Encoder2 = ping_Encoder2.readAll();

    disconnect(&ping_Encoder2, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckEncoder2PingResult()));

    ping_Encoder2.kill();
    params_Encoder2.clear();

    if (result_Encoder2.isNull())
    {
        if(!streaming)
        {
            ui->Encoder2_label->setText("<font color='red'>OFFLINE</font>");
            //Logging("Unable to ping Encoder 2 and VPN Tunnel is not established\n");
            ui->showV2->setEnabled(false);
            ui->showV2->setIcon(pixV2Off);
            ui->showV2->setStyleSheet("color: rgb(198,189,193)");
            ui->showV2->setIconSize(QSize(75,55));

        }
        else
        {
            ui->Encoder2_label->setText("<font color='red'>NOT STREAMING</font>");
            //Logging("Unable to ping Encoder 2 and VPN Tunnel is established\n");
            ui->showV2->setEnabled(false);
            ui->showV2->setIcon(pixV2Off);
            ui->showV2->setStyleSheet("color: rgb(198,189,193)");
            ui->showV2->setIconSize(QSize(75,55));

        }
    }
    else
    {
        ui->showV2->setEnabled(false);
        ui->showV2->setIcon(pixV2Off);
        ui->showV2->setIconSize(QSize(75,55));

        QString result1_Encoder2 = QString(result_Encoder2);
        QStringList result2_Encoder2 = result1_Encoder2.split("\n");

        QString encoder2Status = result2_Encoder2[result2_Encoder2.length()-6];
        QStringList encoder2Status1 = encoder2Status.split("ttl");
        if (encoder2Status1.length() == 1)
        {
            if(!streaming)
            {
                ui->Encoder2_label->setText("<font color='red'>OFFLINE</font>");
                //Logging("Encoder 2 is offline and VPN Tunnel is not established\n");
                ui->showV2->setEnabled(false);
                ui->showV2->setIcon(pixV2Off);
                ui->showV2->setStyleSheet("color: rgb(198,189,193)");
                ui->showV2->setIconSize(QSize(75,55));
            }
            else
            {
                ui->Encoder2_label->setText("<font color='red'>NOT STREAMING</font>");
                //Logging("Encoder 2 is offline and VPN Tunnel is established\n");
                ui->showV2->setEnabled(false);
                ui->showV2->setIcon(pixV2Off);
                ui->showV2->setStyleSheet("color: rgb(198,189,193)");
                ui->showV2->setIconSize(QSize(75,55));
            }
        }
        else
        {
            if(!streaming)
            {
                ui->Encoder2_label->setText("<font color='green'>ONLINE</font>");
                //Logging("Encoder 2 is online and VPN Tunnel is not established\n");
                ui->showV2->setStyleSheet("color: rgb(35,38,39)");
                ui->showV2->setIcon(pixV2On);
                ui->showV2->setEnabled(true);
                ui->showV2->setIconSize(QSize(75,55));
            }
            else
            {
                ui->Encoder2_label->setText("<font color='green'>STREAMING</font>");
                //Logging("Encoder 2 is online and VPN Tunnel is established\n");
                ui->showV2->setStyleSheet("color: rgb(35,38,39)");
                ui->showV2->setIcon(pixV2On);
                ui->showV2->setEnabled(true);
                ui->showV2->setIconSize(QSize(75,55));
            }
        }
    }
}

/****************************************************************************************
 * Function Name: CheckEncoder2
 * Args: NULL
 * Returns: NULL
 * Task: To trigger slot to check on encoder 2 status
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CheckEncoder2()
{
    if (systemCheck)
    {
        //Logging("Pinging Encoder 2....\n");
        connect(&ping_Encoder2, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckEncoder2PingResult()));

        //Encoder 2 ping test
        if (vEncoder2IP.isEmpty() == false && vEncoder2IP.size() == 2)
        {
            params_Encoder2 << "-c" << "4" << vEncoder2IP[1];
            //Logging("Command enter for pinging Encoder 1: " + exec + " " + params_Encoder2.join(" ") + "\n");
            ping_Encoder2.start(exec,params_Encoder2);
        }
        else
        {
            //Logging("Encoder 2 IP can't be found. Please check IP conf file.\n");
        }

    }
}

/****************************************************************************************
 * Function Name: CloseV1
 * Args: NULL
 * Returns: NULL
 * Task: To close video 1 vlc window
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CloseV1()
{
    show_V1.terminate();
    //ui->showV1->setStyleSheet("color: rgb(35,38,39)");
    //ui->showV1->setIcon(pixVOn);
    //ui->showV1->setEnabled(true);
    //Logging("Video 1 Closed\n");
}

/****************************************************************************************
 * Function Name: CloseV2
 * Args: NULL
 * Returns: NULL
 * Task: To close video 2 vlc window
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CloseV2()
{
    show_V2.terminate();
    //ui->showV2->setStyleSheet("color: rgb(35,38,39)");
    //ui->showV2->setIcon(pixVOn);
    //ui->showV2->setEnabled(true);
    //Logging("Video 2 Closed\n");
}

/****************************************************************************************
 * Function Name: UpdateStartUpPage
 * Args: NULL
 * Returns: NULL
 * Task: To update the initial startup page
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::UpdateStartUpPage()
{
    timeElapsed++;
    timeDisplayed = timeTotal - timeElapsed;
    if(timeDisplayed > 0)
    {
        QString timeDisplayedString1 = QString::number(timeDisplayed);
        timeDisplayedLabel = "in " + timeDisplayedString1 + " minutes...";
        ui->countdown_label_2->setGeometry(150,120,281,41);
        ui->countdown_label_2->setText(timeDisplayedLabel);
    }
    if(timeDisplayed <= 0)
    {
        ui->countdown_label_2->setGeometry(90,120,281,41);
        ui->countdown_label_2->setText("in less than a minutes...");
    }
}

/****************************************************************************************
 * Function Name: on_showV1_clicked
 * Args: NULL
 * Returns: NULL
 * Task: To launch vlc player for video 1 stream
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::on_showV1_clicked()
{
    //Logging("Video 1 button Pressed\n");
    if (encoder1.isEmpty() == false && encoder1.size() == 2)
    {
        params_callVLC << "--x11-display" << ":0" << encoder1[1];
        //Logging("Command enter to show V1: " + execVLC + " " + params_callVLC.join(" ") + "\n");
        show_V1.start(execVLC,params_callVLC);
        ui->showV1->setEnabled(false);
        ui->showV2->setEnabled(false);
        timerCloseV = new QTimer();
        timerCloseV->singleShot(timerValue3, this, SLOT(CloseV1()));
    }
    else
    {
        //Logging("Encoder 1 link can't be found. Please check encoder conf file.\n");
    }

}

/****************************************************************************************
 * Function Name: on_showV2_clicked
 * Args: NULL
 * Returns: NULL
 * Task: To launch vlc player for video 2 stream
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::on_showV2_clicked()
{
    //Logging("Video 2 button Pressed\n");
    if (encoder2.isEmpty() == false && encoder2.size() == 2)
    {
    params_callVLC2 << "--x11-display" << ":0" << encoder2[1];
    //Logging("Command enter to show V2: " + execVLC + " " + params_callVLC2.join(" ") + "\n");
    show_V2.start(execVLC,params_callVLC2);
    ui->showV1->setEnabled(false);
    ui->showV2->setEnabled(false);
    timerCloseV = new QTimer();//timer7
    timerCloseV->singleShot(timerValue3, this, SLOT(CloseV2()));
    }
    else
    {
        //Logging("Encoder 2 link can't be found. Please check encoder conf file.\n");
    }
}

/****************************************************************************************
 * Function Name: Logging
 * Args: logPara: QString&
 * Returns: NULL
 * Task: To log events into file
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 * 04/10/22             Tan Kia Wei Remove function
 * **************************************************************************************/
/*void MainWindow::Logging(const QString& logPara)
{
    QFile logFile1(QString("/VSS/log"));
    QDateTime date = QDateTime::currentDateTime();
    QString currentTime = date.toString("dd.MM.yyyy hh:mm:ss.zzz");
    QByteArray currentTimeMsg = currentTime.toLocal8Bit();

    if(logFile1.open(QIODevice::Append))
    {
        QByteArray msg = logPara.toUtf8();
        logFile1.write(currentTimeMsg + " : " + msg);
        logFile1.close();
    }
}*/

/****************************************************************************************
 * Function Name: UpdateClock
 * Args: NULL
 * Returns: NULL
 * Task: To update the clock in the GUI
 * Created by: Tan Kia Wei
 * Date Created: 1st June 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::UpdateClock()
{
    QDateTime date1 = QDateTime::currentDateTime();
    QString currentDate = date1.toString("dd/MM/yyyy");
    QString currentTime1 = date1.toString("hh:mm:ss");
    ui->time_label->setText(currentTime1);
    ui->date_label->setText(currentDate);

}

/****************************************************************************************
 * Function Name: CheckManagedSwitchP1
 * Args: NULL
 * Returns: NULL
 * Task: To trigger slot to check on managed switch Port 1
 * Created by: Tan Kia Wei
 * Date Created: 1st July 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CheckManagedSwitchP1()
{
    if (systemCheck)
    {
        //Logging("Pinging Managed Switch P1....\n");
        connect(&ping_SwitchP1, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckManagedSwitchP1PingResult()));

        //managed switch P1 ping test
        if (manageSwitchP1IP.isEmpty() == false && manageSwitchP1IP.size() == 2)
        {
        params_SwitchP1 << "-c" << "4" << manageSwitchP1IP[1];
        //Logging("Command enter for pinging Managed Switch P1: " + exec + " " + params_SwitchP1.join(" ") + "\n");
        ping_SwitchP1.start(exec,params_SwitchP1);
        }
        else
        {
            //Logging("Managed Switch P1 IP can't be found. Please check IP conf file.\n");
        }
    }
}

/****************************************************************************************
 * Function Name: CheckManagedSwitchP1PingResult
 * Args: NULL
 * Returns: NULL
 * Task: To check the ping result of the equipment connected to port 1 of managed switch from slot
 * Created by: Tan Kia Wei
 * Date Created: 1st July 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CheckManagedSwitchP1PingResult()
{
    //Logging("Retrieving Managed Switch P1 Ping Result....\n");
    QByteArray result_SwitchP1 = ping_SwitchP1.readAll();

    disconnect(&ping_SwitchP1, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckManagedSwitchP1PingResult()));
    ping_SwitchP1.kill();
    params_SwitchP1.clear();

    if (result_SwitchP1.isNull())
    {
        ui->P1_label->setPixmap(pixP1Off);
        //ui->groupBox_2->P1
        ui->P1_label->setScaledContents(true);
        ping_SwitchP1.terminate();
        params_SwitchP1.clear();
        //Logging("Unable to ping Managed Switch P1\n");
    }
    else
    {
        ping_SwitchP1.terminate();
        params_SwitchP1.clear();

        QString result1_SwitchP1 = QString(result_SwitchP1);
        QStringList result2_SwitchP1 = result1_SwitchP1.split("\n");

        QString switchStatusP1 = result2_SwitchP1[result2_SwitchP1.length()-6];
        QStringList switchStatusP1_1 = switchStatusP1.split("ttl");
        if (switchStatusP1_1.length() == 1)
        {
            ui->P1_label->setPixmap(pixP1Off);
            ui->P1_label->setScaledContents(true);
            //Logging("Managed Switch P1 is offline\n");
        }
        else
        {
            ui->P1_label->setPixmap(pixP1On);
            ui->P1_label->setScaledContents(true);
            //Logging("Managed Switch P1 is online\n");
        }
    }
}

/****************************************************************************************
 * Function Name: CheckManagedSwitchP2
 * Args: NULL
 * Returns: NULL
 * Task: To trigger slot to check on managed switch Port 2
 * Created by: Tan Kia Wei
 * Date Created: 1st July 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CheckManagedSwitchP2()
{
    if (systemCheck)
    {
        //Logging("Pinging Managed Switch P2....\n");
        connect(&ping_SwitchP2, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckManagedSwitchP2PingResult()));

        //managed switch P2 ping test
        if (manageSwitchP2IP.isEmpty() == false && manageSwitchP2IP.size() == 2)
        {
            params_SwitchP2 << "-c" << "4" << manageSwitchP2IP[1];
            //Logging("Command enter for pinging Managed Switch P2: " + exec + " " + params_SwitchP2.join(" ") + "\n");
            ping_SwitchP2.start(exec,params_SwitchP2);
        }
        else
        {
            //Logging("Managed Switch P2 IP can't be found. Please check IP conf file.\n");
        }
    }
}

/****************************************************************************************
 * Function Name: CheckManagedSwitchP2PingResult
 * Args: NULL
 * Returns: NULL
 * Task: To check the ping result of the equipment connected to port 2 of managed switch from slot
 * Created by: Tan Kia Wei
 * Date Created: 1st July 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CheckManagedSwitchP2PingResult()
{
    //Logging("Retrieving Managed Switch P2 Ping Result....\n");
    QByteArray result_SwitchP2 = ping_SwitchP2.readAll();

    disconnect(&ping_SwitchP2, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckManagedSwitchP2PingResult()));
    ping_SwitchP2.kill();
    params_SwitchP2.clear();

    if (result_SwitchP2.isNull())
    {
        ui->P2_label->setPixmap(pixP2Off);
        ui->P2_label->setScaledContents(true);
        ping_SwitchP2.terminate();
        params_SwitchP2.clear();
        //Logging("Unable to ping Managed Switch P2\n");
    }
    else
    {

        ping_SwitchP2.terminate();
        params_SwitchP2.clear();

        QString result1_SwitchP2 = QString(result_SwitchP2);
        QStringList result2_SwitchP2 = result1_SwitchP2.split("\n");

        QString switchStatusP2 = result2_SwitchP2[result2_SwitchP2.length()-6];
        QStringList switchStatusP2_1 = switchStatusP2.split("ttl");
        if (switchStatusP2_1.length() == 1)
        {
            ui->P2_label->setPixmap(pixP2Off);
            ui->P2_label->setScaledContents(true);
            //Logging("Managed Switch P2 is offline\n");
        }
        else
        {
            ui->P2_label->setPixmap(pixP2On);
            ui->P2_label->setScaledContents(true);
            //Logging("Managed Switch P2 is online\n");
        }
    }
}

/****************************************************************************************
 * Function Name: CheckManagedSwitchP3
 * Args: NULL
 * Returns: NULL
 * Task: To trigger slot to check on managed switch Port 3
 * Created by: Tan Kia Wei
 * Date Created: 1st July 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CheckManagedSwitchP3()
{
    if (systemCheck)
    {
        //Logging("Pinging Managed Switch P3....\n");
        connect(&ping_SwitchP3, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckManagedSwitchP3PingResult()));
        //managed switch P3 ping test
        if (manageSwitchP3IP.isEmpty() == false && manageSwitchP3IP.size() == 2)
        {
            params_SwitchP3 << "-c" << "4" << manageSwitchP3IP[1];
            //Logging("Command enter for pinging Managed Switch P3: " + exec + " " + params_SwitchP3.join(" ") + "\n");
            ping_SwitchP3.start(exec,params_SwitchP3);
        }
        else
        {
           //Logging("Managed Switch P3 IP can't be found. Please check IP conf file.\n");
        }
    }
}

/****************************************************************************************
 * Function Name: CheckManagedSwitchP3PingResult
 * Args: NULL
 * Returns: NULL
 * Task: To check the ping result of the equipment connected to port 3 of managed switch from slot
 * Created by: Tan Kia Wei
 * Date Created: 1st July 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CheckManagedSwitchP3PingResult()
{
    //Logging("Retrieving Managed Switch P3 Ping Result....\n");
    QByteArray result_SwitchP3 = ping_SwitchP3.readAll();
    disconnect(&ping_SwitchP3, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckManagedSwitchP3PingResult()));
    ping_SwitchP3.kill();
    params_SwitchP3.clear();
    if (result_SwitchP3.isNull())
    {
        ui->P3_label->setPixmap(pixP3Off);
        ui->P3_label->setScaledContents(true);
        ping_SwitchP3.terminate();
        params_SwitchP3.clear();
        //Logging("Unable to ping Managed Switch P3\n");
    }
    else
    {
        ping_SwitchP3.terminate();
        params_SwitchP3.clear();
        QString result1_SwitchP3 = QString(result_SwitchP3);
        QStringList result2_SwitchP3 = result1_SwitchP3.split("\n");
        QString switchStatusP3 = result2_SwitchP3[result2_SwitchP3.length()-6];
        QStringList switchStatusP3_1 = switchStatusP3.split("ttl");
        if (switchStatusP3_1.length() == 1)
        {
            ui->P3_label->setPixmap(pixP3Off);
            ui->P3_label->setScaledContents(true);
            //ui->Switch_label->setText("<font color='red'>OFFLINE</font>");
            //Logging("Managed Switch P3 is offline\n");
        }
        else
        {
            ui->P3_label->setPixmap(pixP3On);
            ui->P3_label->setScaledContents(true);
            //ui->Switch_label->setText("<font color='green'>ONLINE</font>");
            //Logging("Managed Switch P3 is online\n");
        }
    }
}

/****************************************************************************************
 * Function Name: CheckManagedSwitchP4
 * Args: NULL
 * Returns: NULL
 * Task: To trigger slot to check on managed switch Port 4
 * Created by: Tan Kia Wei
 * Date Created: 1st July 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CheckManagedSwitchP4()
{
    if (systemCheck)
    {
        //Logging("Pinging Managed Switch P4....\n");
        connect(&ping_SwitchP4, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckManagedSwitchP4PingResult()));
        //managed switch P4 ping test
        if (manageSwitchP4IP.isEmpty() == false && manageSwitchP4IP.size() == 2)
        {
            params_SwitchP4 << "-c" << "4" << manageSwitchP4IP[1];
            //Logging("Command enter for pinging Managed Switch P4: " + exec + " " + params_SwitchP4.join(" ") + "\n");
            ping_SwitchP4.start(exec,params_SwitchP4);
        }
        else
        {
            //Logging("Managed Switch P4 IP can't be found. Please check IP conf file.\n");
        }
    }
}

/****************************************************************************************
 * Function Name: CheckManagedSwitchP4PingResult
 * Args: NULL
 * Returns: NULL
 * Task: To check the ping result of the equipment connected to port 4 of managed switch from slot
 * Created by: Tan Kia Wei
 * Date Created: 1st July 2022
 * Date Modified        Name        Changes made
 *
 * **************************************************************************************/
void MainWindow::CheckManagedSwitchP4PingResult()
{
    //Logging("Retrieving Managed Switch P4 Ping Result....\n");
    QByteArray result_SwitchP4 = ping_SwitchP4.readAll();
    disconnect(&ping_SwitchP4, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(CheckManagedSwitchP4PingResult()));
    ping_SwitchP4.kill();
    params_SwitchP4.clear();
    if (result_SwitchP4.isNull())
    {
        ui->P4_label->setPixmap(pixP4Off);
        ui->P4_label->setScaledContents(true);
        ping_SwitchP4.terminate();
        params_SwitchP4.clear();
        //Logging("Unable to ping Managed Switch P4\n");
    }
    else
    {
        ping_SwitchP4.terminate();
        params_SwitchP4.clear();
        QString result1_SwitchP4 = QString(result_SwitchP4);
        QStringList result2_SwitchP4 = result1_SwitchP4.split("\n");
        QString switchStatusP4 = result2_SwitchP4[result2_SwitchP4.length()-6];
        QStringList switchStatusP4_1 = switchStatusP4.split("ttl");
        if (switchStatusP4_1.length() == 1)
        {
            ui->P4_label->setPixmap(pixP4Off);
            ui->P4_label->setScaledContents(true);
            //ui->Switch_label->setText("<font color='red'>OFFLINE</font>");
            //Logging("Managed Switch P4 is offline\n");
        }
        else
        {
            ui->P4_label->setPixmap(pixP4On);
            ui->P4_label->setScaledContents(true);
            //ui->Switch_label->setText("<font color='green'>ONLINE</font>");
            //Logging("Managed Switch P4 is online\n");
        }
    }
}

void MainWindow::on_showP1_clicked()
{
    //Logging("P1 button Pressed\n");
}
