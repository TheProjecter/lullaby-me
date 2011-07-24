// checksum 0xa193 version 0x30001
/*
  This file was generated by the Mobile Qt Application wizard of Qt Creator.
  MainWindow is a convenience class containing mobile device specific code
  such as screen orientation handling.
  It is recommended not to modify this file, since newer versions of Qt Creator
  may offer an updated version of it.
*/

//DONE: Pad 0 before single digit
//TOD0: Make music stop when timer stops

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qfile.h"
#include "qtextstream.h"

#include <QtCore/QCoreApplication>
#include <QDateTime>
#include <QTimer>
#include <QDebug>
#include <QThread>

QString colorOn = "background-color: rgb(0, 255, 0); color: white;";
QString colorOff = "background-color: rgb(170, 170, 170); color: white;";
QString borders = "border-radius: 0px; border-top-right-radius: 7px; border-bottom-left-radius: 7px;";

QString fileString ="alarms.txt";
QFile file(fileString);

QStringList *timelist;
QStringList *daylist;
QStringList *soundlist;
QStringList *switchlist;
QProcess process;


bool bAdd = false;
int nPage, nUnit;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showFullScreen();

   // load_alarms();
   // init_alarms();
    time_checker();

    //process = new QProcess(this);

    //Setup the timer object with an interval of one second
    countdown = new QTimer(); //Construct the timer
    countdown->setInterval(1000); //One second interval
    countdown->setSingleShot(false); //Multiple shot. This means that the signal timeout will be signed each second
    connect(countdown,SIGNAL(timeout()),this,SLOT(timeOut())); //Connects the timeout signal to my slot timeOut

}

void  MainWindow::timeOut()
{
   // int Hours;
    int Minutes;
    int Seconds;
    if (startMilliseconds - 1000 >= 0) //If not timeout
    {
        startMilliseconds = startMilliseconds - 1000; //Reduce the milliseconds with 1 second (1000)
        //Convert milliseconds to H:M:S
    //    Hours = startMilliseconds / (1000*60*60);
        Minutes = (startMilliseconds % (1000*60*60)) / (1000*60);
        Seconds = ((startMilliseconds % (1000*60*60)) % (1000*60)) / 1000;

        QString s; // temporary QString object for sprintf
        ui->lcdNumber_4->setText(s.sprintf("%02d", Minutes));
        ui->lcdNumber_2->setText(s.sprintf("%02d", Seconds));

        if (startMilliseconds <=0) //If timeout
        {
            countdown->stop(); //Stop the timer
            ui->lcdNumber_2->setText("00");

            //turn off radio
            //turn off media player
            process.kill();
            process.close();


        }
    }
    else
    {
        countdown->stop(); //Stop the timer
        ui->lcdNumber_2->setText("00");

        //turn off radio
        //turn off media player
        process.kill();
        process.close();
    }
}

void MainWindow::on_btn_start_clicked(){
    int minutes = 0;
    int seconds = 0;

    minutes = ui->cb_min->value() * 1000 * 60;
    seconds = (ui->cb_sec->value()+1) * 1000;

    seconds = seconds + minutes;

    startMilliseconds=seconds;

    countdown->start(); //Start the timer
    process.start("mpx.exe");


}

void MainWindow::time_checker()
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
}

void MainWindow::update()
{
    QString cDay = QDate::shortDayName(QDate::currentDate().dayOfWeek());
    QString cHour = QString::number(QTime::currentTime().hour());
    QString cMin = QString::number(QTime::currentTime().minute());

    for(int i = 0; i < timelist->length(); i++){
        QString sTime = timelist->at(i);
        QString sHour = sTime.split(":")[0];
        QString sMin = sTime.split(":")[1];
        if(sHour == cHour){
            if(sMin == cMin){
                QString days = daylist->at(i);
                if(days.contains(cDay)){
                    QString status = switchlist->at(i);
                    if(status == "On"){
                        // app url provided by Gian
                        process.start("mpx.exe");
                    }
                }
            }
        }
    }
    if(ui->lcdNumber_2->text() != "0"){
        ui->lcdNumber_2->setText(ui->lcdNumber_2->text());
    }
    else{
        process.kill();
    }

    time_checker();
}

void MainWindow::load_alarms(){
    timelist = new QStringList();
    daylist = new QStringList();
    soundlist = new QStringList();
    switchlist = new QStringList();

    if (file.open(QIODevice::ReadOnly |QIODevice::Text)) {
        QTextStream t(&file); // use a text stream
        while(!t.atEnd()){ // read until end of file
            QString line = t.readLine();
            timelist->append(line);
            line = t.readLine();
            daylist->append(line);
            line = t.readLine();
            soundlist->append(line);
            line = t.readLine();
            switchlist->append(line);
        } // close while
        file.close();
    } // close if
}

void MainWindow::init_alarms(){
    nPage = 0;
    show_alarms();
}

void MainWindow::show_alarms(){
    int nCur = nPage * 6;
    if(nPage != 0){
        ui->btn_left->setVisible(true);
    }
    else{
        ui->btn_left->setVisible(false);
    }

    ui->frm_1->setVisible(false);
    ui->frm_2->setVisible(false);
    ui->frm_3->setVisible(false);
    ui->frm_4->setVisible(false);
    ui->frm_5->setVisible(false);
    ui->frm_6->setVisible(false);
    ui->btn_right->setVisible(false);

    if(timelist->length() > 0 + nCur){
        ui->lab_time_1->setText(timelist->at(0 + nCur));
        ui->lab_day_1->setText(daylist->at(0 + nCur));
        if((switchlist->at(0 + nCur)).toStdString() == "On"){
            ui->lab_switch_1->setStyleSheet(colorOn + borders);
        }else{
            ui->lab_switch_1->setStyleSheet(colorOff + borders);
        }
        ui->frm_1->setVisible(true);
        if(timelist->length() > 1 + nCur){
            ui->lab_time_2->setText(timelist->at(1 + nCur));
            ui->lab_day_2->setText(daylist->at(1 + nCur));
            if((switchlist->at(1 + nCur)).toStdString() == "On"){
                ui->lab_switch_2->setStyleSheet(colorOn + borders);
            }else{
                ui->lab_switch_2->setStyleSheet(colorOff + borders);
            }
            ui->frm_2->setVisible(true);
            if(timelist->length() > 2 + nCur){
                ui->lab_time_3->setText(timelist->at(2 + nCur));
                ui->lab_day_3->setText(daylist->at(2 + nCur));
                if((switchlist->at(2 + nCur)).toStdString() == "On"){
                    ui->lab_switch_3->setStyleSheet(colorOn + borders);
                }else{
                    ui->lab_switch_3->setStyleSheet(colorOff + borders);
                }
                ui->frm_3->setVisible(true);
                if(timelist->length() > 3 + nCur){
                    ui->lab_time_4->setText(timelist->at(3 + nCur));
                    ui->lab_day_4->setText(daylist->at(3 + nCur));
                    if((switchlist->at(3 + nCur)).toStdString() == "On"){
                        ui->lab_switch_4->setStyleSheet(colorOn + borders);
                    }else{
                        ui->lab_switch_4->setStyleSheet(colorOff + borders);
                    }
                    ui->frm_4->setVisible(true);
                    if(timelist->length() > 4 + nCur){
                        ui->lab_time_5->setText(timelist->at(4 + nCur));
                        ui->lab_day_5->setText(daylist->at(4 + nCur));
                        if((switchlist->at(4 + nCur)).toStdString() == "On"){
                            ui->lab_switch_5->setStyleSheet(colorOn + borders);
                        }else{
                            ui->lab_switch_5->setStyleSheet(colorOff + borders);
                        }
                        ui->frm_5->setVisible(true);
                        if(timelist->length() > 5 + nCur){
                            ui->lab_time_6->setText(timelist->at(5 + nCur));
                            ui->lab_day_6->setText(daylist->at(5 + nCur));
                            if((switchlist->at(5 + nCur)).toStdString() == "On"){
                                ui->lab_switch_6->setStyleSheet(colorOn + borders);
                            }else{
                                ui->lab_switch_6->setStyleSheet(colorOff + borders);
                            }
                            ui->frm_6->setVisible(true);
                            if(timelist->length() > 6 + nCur){
                                ui->btn_right->setVisible(true);
                            }
                        }
                    }
                }
            }
        }
    }

}

void MainWindow::on_btn_alarm_clicked(){
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_btn_help_clicked(){
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_btn_new_clicked(){
    ui->stackedWidget->setCurrentIndex(1);
    bAdd = true;
}

void MainWindow::on_btn_sleep_clicked(){
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_btn_sun_clicked(){
    bool isCheckable = !ui->btn_sun->isCheckable();
    if(isCheckable){
        ui->btn_sun->setStyleSheet(colorOn);
    }
    else{
        ui->btn_sun->setStyleSheet(colorOff);
    }
    ui->btn_sun->setCheckable(isCheckable);
}

void MainWindow::on_btn_mon_clicked(){
    bool isCheckable = ui->btn_mon->isCheckable();
    if(isCheckable){
        ui->btn_mon->setStyleSheet(colorOff);
    }
    else{
        ui->btn_mon->setStyleSheet(colorOn);
    }
    ui->btn_mon->setCheckable(!isCheckable);
}

void MainWindow::on_btn_tue_clicked(){
    bool isCheckable = ui->btn_tue->isCheckable();
    if(isCheckable){
        ui->btn_tue->setStyleSheet(colorOff);
    }
    else{
        ui->btn_tue->setStyleSheet(colorOn);
    }
    ui->btn_tue->setCheckable(!isCheckable);
}

void MainWindow::on_btn_wed_clicked(){
    bool isCheckable = ui->btn_wed->isCheckable();
    if(isCheckable){
        ui->btn_wed->setStyleSheet(colorOff);
    }
    else{
        ui->btn_wed->setStyleSheet(colorOn);
    }
    ui->btn_wed->setCheckable(!isCheckable);
}

void MainWindow::on_btn_thu_clicked(){
    bool isCheckable = ui->btn_thu->isCheckable();
    if(isCheckable){
        ui->btn_thu->setStyleSheet(colorOff);
    }
    else{
        ui->btn_thu->setStyleSheet(colorOn);
    }
    ui->btn_thu->setCheckable(!isCheckable);
}

void MainWindow::on_btn_fri_clicked(){
    bool isCheckable = ui->btn_fri->isCheckable();
    if(isCheckable){
        ui->btn_fri->setStyleSheet(colorOff);
    }
    else{
        ui->btn_fri->setStyleSheet(colorOn);
    }
    ui->btn_fri->setCheckable(!isCheckable);
}

void MainWindow::on_btn_sat_clicked(){
    bool isCheckable = ui->btn_sat->isCheckable();
    if(isCheckable){
        ui->btn_sat->setStyleSheet(colorOff);
    }
    else{
        ui->btn_sat->setStyleSheet(colorOn);
    }
    ui->btn_sat->setCheckable(!isCheckable);
}


void MainWindow::on_btn_close_clicked(){
    close();
}

void MainWindow::on_btn_save_clicked(){

    QString time, day = "", sound, status;
    time = ui->timeEdit->time().toString();
    if(ui->btn_sun->isCheckable()){
        day += "Sun ";
    }
    if(ui->btn_mon->isCheckable()){
        day += "Mon ";
    }
    if(ui->btn_tue->isCheckable()){
        day += "Tue ";
    }
    if(ui->btn_wed->isCheckable()){
        day += "Wed ";
    }
    if(ui->btn_thu->isCheckable()){
        day += "Thu ";
    }
    if(ui->btn_fri->isCheckable()){
        day += "Fri ";
    }
    if(ui->btn_sat->isCheckable()){
        day += "Sat ";
    }
    sound = ui->cb_sound->currentText();

    status = ui->cb_status->currentText();

    QString toTxt = time + "\n" + day + "\n" + sound + "\n" + status + "\n";

    // write toTxt on text file
    QFile f(fileString);
    if(bAdd){
        if(f.open(QIODevice::Append | QIODevice::Text)){
            QTextStream out(&f);

            out<<toTxt;
            f.close();
        }
    }
    else{
        f.remove(fileString);
        if(f.open(QIODevice::Append | QIODevice::Text)){
            QTextStream out(&f);
            for(int i = 0; i < nPage*6 + nUnit; i++){
                QString oldTxt = timelist->at(i) + "\n";
                oldTxt.append(daylist->at(i) + "\n");
                oldTxt.append(soundlist->at(i) + "\n");
                oldTxt.append(switchlist->at(i) + "\n");
                out<<oldTxt;
            }
            out<<toTxt;
            for(int i = nPage*6 + nUnit + 1; i < timelist->length(); i++){
                QString oldTxt = timelist->at(i) + "\n";
                oldTxt.append(daylist->at(i) + "\n");
                oldTxt.append(soundlist->at(i) + "\n");
                oldTxt.append(switchlist->at(i) + "\n");
                out<<oldTxt;
            }
            f.close();
        }
    }

    reset_buttons();
    load_alarms();
    show_alarms();
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_btn_cancel_clicked(){
    reset_buttons();
    show_alarms();
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::reset_buttons(){
    bAdd = false;
    ui->btn_sun->setStyleSheet(colorOff);
    ui->btn_sun->setCheckable(false);
    ui->btn_mon->setStyleSheet(colorOff);
    ui->btn_mon->setCheckable(false);
    ui->btn_tue->setStyleSheet(colorOff);
    ui->btn_tue->setCheckable(false);
    ui->btn_wed->setStyleSheet(colorOff);
    ui->btn_wed->setCheckable(false);
    ui->btn_thu->setStyleSheet(colorOff);
    ui->btn_thu->setCheckable(false);
    ui->btn_fri->setStyleSheet(colorOff);
    ui->btn_fri->setCheckable(false);
    ui->btn_sat->setStyleSheet(colorOff);
    ui->btn_sat->setCheckable(false);
}

void MainWindow::on_btn_left_clicked(){
    nPage--;
    // load previous set of alarms and display on frm_1 to frm_6
    show_alarms();
}
void MainWindow::on_btn_right_clicked(){
    nPage++;
    // load next set of alarms and display on frm_1 to frm_6
    show_alarms();
}

void MainWindow::on_btn_frm_1_clicked(){
    int nCur = 0 + nPage * 6; // different from show_alarm()
    nUnit = 0;
    QTime time = time.fromString(timelist->at(nCur));
    ui->timeEdit->setTime(time);

    QString days = daylist->at(nCur);
    if(days.contains("Sun")){
        ui->btn_sun->setStyleSheet(colorOn);
        ui->btn_sun->setCheckable(true);
    }
    if(days.contains("Mon")){
        ui->btn_mon->setStyleSheet(colorOn);
        ui->btn_mon->setCheckable(true);
    }
    if(days.contains("Tue")){
        ui->btn_tue->setStyleSheet(colorOn);
        ui->btn_tue->setCheckable(true);
    }
    if(days.contains("Wed")){
        ui->btn_wed->setStyleSheet(colorOn);
        ui->btn_wed->setCheckable(true);
    }
    if(days.contains("Thu")){
        ui->btn_thu->setStyleSheet(colorOn);
        ui->btn_thu->setCheckable(true);
    }
    if(days.contains("Fri")){
        ui->btn_fri->setStyleSheet(colorOn);
        ui->btn_fri->setCheckable(true);
    }
    if(days.contains("Sat")){
        ui->btn_sat->setStyleSheet(colorOn);
        ui->btn_sat->setCheckable(true);
    }

    QString sound = soundlist->at(nCur);
    if(sound == "Radio"){
        ui->cb_sound->setCurrentIndex(0);
    }
    else if(sound == "Default"){
        ui->cb_sound->setCurrentIndex(1);
    }
    else if(sound == "Music Player"){
        ui->cb_sound->setCurrentIndex(2);
    }

    QString status = switchlist->at(nCur);
    if(status == "On"){
        ui->cb_sound->setCurrentIndex(0);
    }
    else if(status == "Off"){
        ui->cb_sound->setCurrentIndex(1);
    }

    ui->stackedWidget->setCurrentIndex(1);
}
void MainWindow::on_btn_frm_2_clicked(){
    int nCur = 1 + nPage * 6; // different from show_alarm()
    nUnit = 1;
    QTime time = time.fromString(timelist->at(nCur));
    ui->timeEdit->setTime(time);

    QString days = daylist->at(nCur);
    if(days.contains("Sun")){
        ui->btn_sun->setStyleSheet(colorOn);
        ui->btn_sun->setCheckable(true);
    }
    if(days.contains("Mon")){
        ui->btn_mon->setStyleSheet(colorOn);
        ui->btn_mon->setCheckable(true);
    }
    if(days.contains("Tue")){
        ui->btn_tue->setStyleSheet(colorOn);
        ui->btn_tue->setCheckable(true);
    }
    if(days.contains("Wed")){
        ui->btn_wed->setStyleSheet(colorOn);
        ui->btn_wed->setCheckable(true);
    }
    if(days.contains("Thu")){
        ui->btn_thu->setStyleSheet(colorOn);
        ui->btn_thu->setCheckable(true);
    }
    if(days.contains("Fri")){
        ui->btn_fri->setStyleSheet(colorOn);
        ui->btn_fri->setCheckable(true);
    }
    if(days.contains("Sat")){
        ui->btn_sat->setStyleSheet(colorOn);
        ui->btn_sat->setCheckable(true);
    }

    QString sound = soundlist->at(nCur);
    if(sound == "Radio"){
        ui->cb_sound->setCurrentIndex(0);
    }
    else if(sound == "Default"){
        ui->cb_sound->setCurrentIndex(1);
    }
    else if(sound == "Music Player"){
        ui->cb_sound->setCurrentIndex(2);
    }

    QString status = switchlist->at(nCur);
    if(status == "On"){
        ui->cb_sound->setCurrentIndex(0);
    }
    else if(status == "Off"){
        ui->cb_sound->setCurrentIndex(1);
    }

    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_btn_frm_3_clicked(){
    int nCur = 2 + nPage * 6; // different from show_alarm()
    nUnit = 2;
    QTime time = time.fromString(timelist->at(nCur));
    ui->timeEdit->setTime(time);

    QString days = daylist->at(nCur);
    if(days.contains("Sun")){
        ui->btn_sun->setStyleSheet(colorOn);
        ui->btn_sun->setCheckable(true);
    }
    if(days.contains("Mon")){
        ui->btn_mon->setStyleSheet(colorOn);
        ui->btn_mon->setCheckable(true);
    }
    if(days.contains("Tue")){
        ui->btn_tue->setStyleSheet(colorOn);
        ui->btn_tue->setCheckable(true);
    }
    if(days.contains("Wed")){
        ui->btn_wed->setStyleSheet(colorOn);
        ui->btn_wed->setCheckable(true);
    }
    if(days.contains("Thu")){
        ui->btn_thu->setStyleSheet(colorOn);
        ui->btn_thu->setCheckable(true);
    }
    if(days.contains("Fri")){
        ui->btn_fri->setStyleSheet(colorOn);
        ui->btn_fri->setCheckable(true);
    }
    if(days.contains("Sat")){
        ui->btn_sat->setStyleSheet(colorOn);
        ui->btn_sat->setCheckable(true);
    }

    QString sound = soundlist->at(nCur);
    if(sound == "Radio"){
        ui->cb_sound->setCurrentIndex(0);
    }
    else if(sound == "Default"){
        ui->cb_sound->setCurrentIndex(1);
    }
    else if(sound == "Music Player"){
        ui->cb_sound->setCurrentIndex(2);
    }

    QString status = switchlist->at(nCur);
    if(status == "On"){
        ui->cb_sound->setCurrentIndex(0);
    }
    else if(status == "Off"){
        ui->cb_sound->setCurrentIndex(1);
    }

    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_btn_frm_4_clicked(){
    int nCur = 3 + nPage * 6; // different from show_alarm()
    nUnit = 3;
    QTime time = time.fromString(timelist->at(nCur));
    ui->timeEdit->setTime(time);

    QString days = daylist->at(nCur);
    if(days.contains("Sun")){
        ui->btn_sun->setStyleSheet(colorOn);
        ui->btn_sun->setCheckable(true);
    }
    if(days.contains("Mon")){
        ui->btn_mon->setStyleSheet(colorOn);
        ui->btn_mon->setCheckable(true);
    }
    if(days.contains("Tue")){
        ui->btn_tue->setStyleSheet(colorOn);
        ui->btn_tue->setCheckable(true);
    }
    if(days.contains("Wed")){
        ui->btn_wed->setStyleSheet(colorOn);
        ui->btn_wed->setCheckable(true);
    }
    if(days.contains("Thu")){
        ui->btn_thu->setStyleSheet(colorOn);
        ui->btn_thu->setCheckable(true);
    }
    if(days.contains("Fri")){
        ui->btn_fri->setStyleSheet(colorOn);
        ui->btn_fri->setCheckable(true);
    }
    if(days.contains("Sat")){
        ui->btn_sat->setStyleSheet(colorOn);
        ui->btn_sat->setCheckable(true);
    }

    QString sound = soundlist->at(nCur);
    if(sound == "Radio"){
        ui->cb_sound->setCurrentIndex(0);
    }
    else if(sound == "Default"){
        ui->cb_sound->setCurrentIndex(1);
    }
    else if(sound == "Music Player"){
        ui->cb_sound->setCurrentIndex(2);
    }

    QString status = switchlist->at(nCur);
    if(status == "On"){
        ui->cb_sound->setCurrentIndex(0);
    }
    else if(status == "Off"){
        ui->cb_sound->setCurrentIndex(1);
    }

    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_btn_frm_5_clicked(){
    int nCur = 4 + nPage * 6; // different from show_alarm()
    nUnit = 4;
    QTime time = time.fromString(timelist->at(nCur));
    ui->timeEdit->setTime(time);

    QString days = daylist->at(nCur);
    if(days.contains("Sun")){
        ui->btn_sun->setStyleSheet(colorOn);
        ui->btn_sun->setCheckable(true);
    }
    if(days.contains("Mon")){
        ui->btn_mon->setStyleSheet(colorOn);
        ui->btn_mon->setCheckable(true);
    }
    if(days.contains("Tue")){
        ui->btn_tue->setStyleSheet(colorOn);
        ui->btn_tue->setCheckable(true);
    }
    if(days.contains("Wed")){
        ui->btn_wed->setStyleSheet(colorOn);
        ui->btn_wed->setCheckable(true);
    }
    if(days.contains("Thu")){
        ui->btn_thu->setStyleSheet(colorOn);
        ui->btn_thu->setCheckable(true);
    }
    if(days.contains("Fri")){
        ui->btn_fri->setStyleSheet(colorOn);
        ui->btn_fri->setCheckable(true);
    }
    if(days.contains("Sat")){
        ui->btn_sat->setStyleSheet(colorOn);
        ui->btn_sat->setCheckable(true);
    }

    QString sound = soundlist->at(nCur);
    if(sound == "Radio"){
        ui->cb_sound->setCurrentIndex(0);
    }
    else if(sound == "Default"){
        ui->cb_sound->setCurrentIndex(1);
    }
    else if(sound == "Music Player"){
        ui->cb_sound->setCurrentIndex(2);
    }

    QString status = switchlist->at(nCur);
    if(status == "On"){
        ui->cb_sound->setCurrentIndex(0);
    }
    else if(status == "Off"){
        ui->cb_sound->setCurrentIndex(1);
    }

    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_btn_frm_6_clicked(){
    int nCur = 5 + nPage * 6; // different from show_alarm()
    nUnit = 5;
    QTime time = time.fromString(timelist->at(nCur));
    ui->timeEdit->setTime(time);

    QString days = daylist->at(nCur);
    if(days.contains("Sun")){
        ui->btn_sun->setStyleSheet(colorOn);
        ui->btn_sun->setCheckable(true);
    }
    if(days.contains("Mon")){
        ui->btn_mon->setStyleSheet(colorOn);
        ui->btn_mon->setCheckable(true);
    }
    if(days.contains("Tue")){
        ui->btn_tue->setStyleSheet(colorOn);
        ui->btn_tue->setCheckable(true);
    }
    if(days.contains("Wed")){
        ui->btn_wed->setStyleSheet(colorOn);
        ui->btn_wed->setCheckable(true);
    }
    if(days.contains("Thu")){
        ui->btn_thu->setStyleSheet(colorOn);
        ui->btn_thu->setCheckable(true);
    }
    if(days.contains("Fri")){
        ui->btn_fri->setStyleSheet(colorOn);
        ui->btn_fri->setCheckable(true);
    }
    if(days.contains("Sat")){
        ui->btn_sat->setStyleSheet(colorOn);
        ui->btn_sat->setCheckable(true);
    }

    QString sound = soundlist->at(nCur);
    if(sound == "Radio"){
        ui->cb_sound->setCurrentIndex(0);
    }
    else if(sound == "Default"){
        ui->cb_sound->setCurrentIndex(1);
    }
    else if(sound == "Music Player"){
        ui->cb_sound->setCurrentIndex(2);
    }

    QString status = switchlist->at(nCur);
    if(status == "On"){
        ui->cb_sound->setCurrentIndex(0);
    }
    else if(status == "Off"){
        ui->cb_sound->setCurrentIndex(1);
    }

    ui->stackedWidget->setCurrentIndex(1);
}



























MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setOrientation(ScreenOrientation orientation)
{
#if defined(Q_OS_SYMBIAN)
    // If the version of Qt on the device is < 4.7.2, that attribute won't work
    if (orientation != ScreenOrientationAuto) {
        const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
        if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702) {
            qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
            return;
        }
    }
#endif // Q_OS_SYMBIAN

    Qt::WidgetAttribute attribute;
    switch (orientation) {
#if QT_VERSION < 0x040702
    // Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
    case ScreenOrientationLockPortrait:
        attribute = static_cast<Qt::WidgetAttribute>(128);
        break;
    case ScreenOrientationLockLandscape:
        attribute = static_cast<Qt::WidgetAttribute>(129);
        break;
    default:
    case ScreenOrientationAuto:
        attribute = static_cast<Qt::WidgetAttribute>(130);
        break;
#else // QT_VERSION < 0x040702
    case ScreenOrientationLockPortrait:
        attribute = Qt::WA_LockPortraitOrientation;
        break;
    case ScreenOrientationLockLandscape:
        attribute = Qt::WA_LockLandscapeOrientation;
        break;
    default:
    case ScreenOrientationAuto:
        attribute = Qt::WA_AutoOrientation;
        break;
#endif // QT_VERSION < 0x040702
    };
    setAttribute(attribute, true);
}

void MainWindow::showExpanded()
{
#ifdef Q_OS_SYMBIAN
    showFullScreen();
#elif defined(Q_WS_MAEMO_5) || defined(Q_WS_MAEMO_6)
    showMaximized();
#else
    show();
#endif
}

void MainWindow::on_btn_hide_clicked()
{
    lower();
}
