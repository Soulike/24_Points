#ifndef GAMEMAINWINDOW_H
#define GAMEMAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include<QString>
#include<answerdialog.h>
#include<errordialog.h>

namespace Ui {
class GameMainWindow;
}

class Card;
class GameMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	friend class AnswerDialog;
	explicit GameMainWindow(QWidget *parent = 0);
	~GameMainWindow();

private slots:
	void on_shuffleButton_clicked();

	void clear();//清空所有数据，重置界面显示
	void clear_equation();//清空所有内部数据，不改变界面显示
	void backspace();
	void equal();
	void input_plus();
	void input_minus();
	void input_times();
	void input_divide();
	void input_left();
	void input_right();

	void on_card1_clicked();
	void on_card2_clicked();
	void on_card3_clicked();
	void on_card4_clicked();

	void on_answerButton_clicked();

private:
	Ui::GameMainWindow *ui;
	QVector<Card> cards;//四张牌
	QString equation,equation_for_show;
	int score;//得分

	void putCards();//随机产生四张牌放在四个位置中
	double calculate(QString & equation);//主运算函数
	void check_erase_signal();//检测是否需要覆盖运算符
	void append(QString c);//在文本框内追加文字，改变数据
	void showErrorDialog();

	const int PRECISION = 11;//转换精度
};


class Card
{
public:
	friend class GameMainWindow;
	Card()
	{
		this->value = 0;
		this->background = "";
		checked = false;
	}
	int getValue() const {return value;}
	const QString & getBackground() const {return background;}

	static Card getRandomCard();//产生随机类型的扑克对象

	Card & operator=(const Card & c);
	bool operator ==(const Card & c) const;
private:
	int value;
	QString background;
	const QVector<QString> types ={"club","diamond","heart","spade"};//四种类型字符串
	QString equation,equaltion_for_show;
	bool checked;
};

#endif // GAMEMAINWINDOW_H
