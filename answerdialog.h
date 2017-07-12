#ifndef ANSWERDIALOG_H
#define ANSWERDIALOG_H

#include <QDialog>

namespace Ui {
class AnswerDialog;
}

class AnswerDialog : public QDialog
{
	Q_OBJECT

public:
	friend class GameMainWindow;
	explicit AnswerDialog(QWidget *parent = 0);
	~AnswerDialog();
	void changeTextEdit(QString str);

private slots:
	void on_pushButton_clicked();

private:
	Ui::AnswerDialog *ui;
};

#endif // ANSWERDIALOG_H
