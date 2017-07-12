#include "errordialog.h"
#include "ui_errordialog.h"

ErrorDialog::ErrorDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ErrorDialog)
{
	ui->setupUi(this);
	setWindowOpacity(0.99);
	setWindowTitle("错误");
	setStyleSheet(
				"QPushButton{border-radius:5px;border:1px solid gray;font-size:20px;}"
				"QPushButton:pressed{font-size: 17px;}"
				"QPushButton:hover{background-color:red;}"
				"QLabel{color:black;}"
				  );
}

ErrorDialog::~ErrorDialog()
{
	delete ui;
}

void ErrorDialog::on_pushButton_clicked()
{
	close();
	delete this;
}
