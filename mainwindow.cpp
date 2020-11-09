#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QFile>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonTest_clicked()
{
    static const QString DBNAME = "test.db";

    if (QFile::exists(DBNAME))
        QFile::remove(DBNAME);

    // create and open database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    // db.setHostName("hostname");
    db.setDatabaseName(DBNAME);
    // db.setUserName("UserName");
    // db.setPassword("password");
    if (!db.open()) {
        qDebug() << "database open failed.";
        return;
    }

    // create table
    static const QString TABLENAME = "test";
    static const QString SQL1 = "CREATE TABLE %1(id INTEGER, name TEXT, memo TEXT)";
    // QSqlQuery q1 = db.exec(SQL1.arg(TABLENAME));
    QSqlQuery q1(db);
    if (!q1.exec(SQL1.arg(TABLENAME))) {
        qDebug() << q1.lastError().text();
        return;
    }
    // check table exists
    if (!db.tables().contains(TABLENAME)) {
        qDebug() << "table create failed.";
        return;
    }

    // insert record
    QSqlQuery q2(db);
    static const QString SQL2 = "INSERT INTO %1 VALUES(1, 'taro', 'memo1')";
    if (!q2.exec(SQL2.arg(TABLENAME.arg(TABLENAME)))) {
        qDebug() << "table create failed.";
        return;
    }

    // execBatch
    static const QString SQL3 = "INSERT INTO %1 VALUES(?, ?, ?)";
    QSqlQuery q3(db);
    q3.prepare(SQL3.arg(TABLENAME));
    QVariantList v1 = { 2, 3, 4, 5 };
    q3.addBindValue(v1);
    QVariantList v2 = { "jiro", "saburo", "shiro", "hanako" };
    q3.addBindValue(v2);
    QVariantList v3 = { "memo2", "memo3", "memo4", "memo5" };
    q3.addBindValue(v3);
    if (!q3.execBatch()) {
        qDebug() << q3.lastError();
        return;
    }

    // select record
    static const QString SQL4 = "SELECT * FROM %1";
    QSqlQuery q4(db);
    if (!q4.exec(SQL4.arg(TABLENAME))) {
        qDebug() << q4.lastError();
        return;
    }
    while (q4.next()) {
        QStringList record;
        for (int i = 0; i < q4.record().count(); ++i)
            record << q4.record().value(i).toString();
        qDebug() << record.join(" ");
    }

    db.close();
}
