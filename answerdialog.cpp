#include "answerdialog.h"
#include "ui_answerdialog.h"

AnswerDialog::AnswerDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AnswerDialog)
{
	ui->setupUi(this);
	setWindowOpacity(0.98);
	setWindowTitle("答案");
	setStyleSheet(
				"QDialog{background-color:rgb(200,200,200);}"
				"QTextEdit{border:1px solid gray;border-radius:10px;padding:10px 10px;background:transparent;color:black;font-size:20px;}"
				"QPushButton{border-radius:5px;border:1px solid gray;font-size:25px;}"
				"QPushButton:pressed{font-size: 23px;}"
				"QPushButton:hover{background-color:red;}"
				);
}

AnswerDialog::~AnswerDialog()
{
	delete ui;
}

void AnswerDialog::changeTextEdit(QString str)
{
	ui->textEdit->setText(str);
}

void AnswerDialog::on_pushButton_clicked()
{
	close();
	delete this;
}
