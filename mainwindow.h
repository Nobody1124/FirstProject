#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QTimer>
#include <QWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    Q_SLOT void StartSystemCheck();
    Q_SLOT void CheckLTERouter();
    Q_SLOT void CheckLTERouterPingResult();
    Q_SLOT void CheckVPNandIPAddress();
    Q_SLOT void CheckVPNResult();
    Q_SLOT void CheckIPAddResult();
    Q_SLOT void CheckManagedSwitch();
    Q_SLOT void CheckManagedSwitchPingResult();
    Q_SLOT void CheckEncoder1();
    Q_SLOT void CheckEncoder1PingResult();
    Q_SLOT void CheckEncoder2();
    Q_SLOT void CheckEncoder2PingResult();
    Q_SLOT void CheckBandwidth();
    Q_SLOT void CheckBandwidthResult();
    Q_SLOT void CheckSignal();
    Q_SLOT void CheckSignalResult();
    Q_SLOT void CloseV1();
    Q_SLOT void CloseV2();
    Q_SLOT void UpdateStartUpPage();
    //Q_SLOT void Logging(const QString& logPara);
    Q_SLOT void UpdateClock();
    Q_SLOT void CheckManagedSwitchP1();
    Q_SLOT void CheckManagedSwitchP1PingResult();

    Q_SLOT void CheckManagedSwitchP2();
    Q_SLOT void CheckManagedSwitchP2PingResult();
    Q_SLOT void CheckManagedSwitchP3();
    Q_SLOT void CheckManagedSwitchP3PingResult();
    Q_SLOT void CheckManagedSwitchP4();
    Q_SLOT void CheckManagedSwitchP4PingResult();


    void on_showV1_clicked();

    void on_showV2_clicked();

    void on_showP1_clicked();

private:
    Ui::MainWindow *ui;

    bool systemCheck = false;
    bool streaming = false;
    bool vpnconnected = false;
    bool ipAcquired = false;
    bool startUp = true;
    bool crash = false;

    int ipFailTime = 0;
    int signal_level = 0;

    int timerValue = 0;
    int timerValue2 = 0;
    int timerValue3 = 0;
    int timerValue4 = 0;
    float previousvalue = 0.000;
    float currentvalue = 0.000;
    float bandwidth = 0.000;

    float timeTotal = 0.000;
    float timeElapsed = 0.000;
    float timeDisplayed = 0.000;

    QTimer *timerLTERouter;//timer
    QTimer *timerVPN;//timer1
    QTimer *timerSystemCheck;//timer2
    QTimer *timerSwitch;//timer3
    QTimer *timerEncoder1;//timer4
    QTimer *timerEncoder2;//timer5
    QTimer *timerBandwidth;//timer6
    QTimer *timerCloseV;//timer7
    QTimer *timerStartupPage;//timer8
    QTimer *timerSignal;//timer9
    QTimer *timerClock;//timer for clock
    QTimer *timerSwitchP1;//timer10
    QTimer *timerSwitchP2;//timer11
    QTimer *timerSwitchP3;//timer12
    QTimer *timerSwitchP4;//timer13

    QString timeDisplayedLabel;
    QStringList crash_value;
    QStringList IP_address;
    QStringList timer_value;
    QStringList encoder_URL;
    //QStringList snmp_Conf;
    //QStringList snmpConf;
    QStringList encoder1;
    QStringList encoder2;
    QStringList routerIP;
    QStringList manageSwitchIP;
    QStringList manageSwitchP1IP;
    QStringList manageSwitchP2IP;
    QStringList manageSwitchP3IP;
    QStringList manageSwitchP4IP;
    QStringList vEncoder1IP;
    QStringList vEncoder2IP;
    QStringList simIP;
    QString exec = "ping";
    QStringList params_VPN;
    QStringList params_Switch;
    QStringList params_SwitchP1;
    QStringList params_SwitchP2;
    QStringList params_SwitchP3;
    QStringList params_SwitchP4;
    QStringList params_Encoder1;
    QStringList params_Encoder2;
    QString execSNMP = "snmpwalk";
    QString execSNMPIPAcquire = "snmpwalk";
    QString execSNMPBandwidth = "snmpget";
    QString execSNMPSignal = "snmpget";
    QStringList params_SNMPget;
    QStringList params_SNMPIPAcquire;
    QStringList params_SNMPBandwidth;
    QStringList params_SNMPSignal;
    QStringList params_callVLC;
    QStringList params_callVLC2;
    QString execVLC = "vlc";

    QProcess show_V1;
    QProcess show_V2;
    QProcess snmpGet_VPNtunnel;
    QProcess snmpGet_IPAdd;
    QProcess snmpGet_Bandwidth;
    QProcess snmpGet_Signal;
    QProcess ping_VPN;
    QProcess ping_Switch;
    QProcess ping_Encoder1;
    QProcess ping_Encoder2;
    QProcess ping_SwitchP1;
    QProcess ping_SwitchP2;
    QProcess ping_SwitchP3;
    QProcess ping_SwitchP4;

    QPixmap pixNS;
    QPixmap pixPS;
    QPixmap pixMS;
    QPixmap pixGS;
    QPixmap pixES;

    QPixmap pixV1Off;
    QPixmap pixV1On;
    QPixmap pixV2Off;
    QPixmap pixV2On;
    QPixmap pixP1Off;
    QPixmap pixP1On;
    QPixmap pixP2Off;
    QPixmap pixP2On;
    QPixmap pixP3Off;
    QPixmap pixP3On;
    QPixmap pixP4Off;
    QPixmap pixP4On;

    QByteArray signal_result_snmp;

};

#endif // MAINWINDOW_H
