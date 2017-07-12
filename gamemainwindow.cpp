#include "gamemainwindow.h"
#include "ui_gamemainwindow.h"
#include<cstdlib>
#include<ctime>
#include<QDebug>
#include<cmath>

static bool isSignal(QChar c);//判断c是不是运算符
static bool hasAnswer(double a, double b, double c, double d);
GameMainWindow::GameMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::GameMainWindow)
{
	ui->setupUi(this);
	score = 0;
	for(int i=0;i<4;i++)
		cards.push_back(Card());

	setStyleSheet(
				"GameMainWindow{border-image: url(:/images/background.jpg);}"
				"QLabel{color:white;}"
				"QGroupBox {border:hidden;}"
				"QPushButton {background-color: rgba(255,255,255,0.8); border:1px solid gray}"
				"QPushButton:pressed{font-size: 35px;}"
				"QPushButton:hover{background-color:red;}"
				"#plusButton {border-top-right-radius:25px;}"
				"#rightButton {border-bottom-right-radius:25px;}"
				"#shuffleButton {border-top-left-radius:25px;}"
				"#answerButton {border-bottom-left-radius:25px;}"
				"#output{padding: 0 20px;border-radius : 10px; background-color:rgba(255,255,255,0.8);color:black;}"
				"#card1,#card2,#card3,#card4{background-color:rgba(255,255,255,0.8);border:hidden;border-radius: 5px;}"
				"#card1:pressed,#card2:pressed,#card3:pressed,#card4:pressed{outline:5px inset red;}"
				);
	setWindowTitle("24点");
	connect(ui->clearButton,&QPushButton::clicked,this,clear);
	connect(ui->backspaceButton,&QPushButton::clicked,this,backspace);
	connect(ui->plusButton,&QPushButton::clicked,this,input_plus);
	connect(ui->minusButton,&QPushButton::clicked,this,input_minus);
	connect(ui->timesButton,&QPushButton::clicked,this,input_times);
	connect(ui->divideButton,&QPushButton::clicked,this,input_divide);
	connect(ui->leftButton,&QPushButton::clicked,this,input_left);
	connect(ui->rightButton,&QPushButton::clicked,this,input_right);
	connect(ui->submitButton,&QPushButton::clicked,this,equal);
	putCards();
}

GameMainWindow::~GameMainWindow()
{
	delete ui;
}

void GameMainWindow::putCards()
{
	srand(time(nullptr));
	do
	{
	for(int i=0;i<4;i++)
		cards[i]=Card::getRandomCard();
	while(cards[1] == cards[0])
		cards[1] = Card::getRandomCard();

	while(cards[2] == cards[0] || cards[2]==cards[1])
		cards[2] = Card::getRandomCard();

	while(cards[3] == cards[0] || cards[3] == cards[1] || cards[3] == cards[2])
		cards[3] = Card::getRandomCard();
	}while(!hasAnswer(cards[0].getValue(),cards[1].getValue(),cards[2].getValue(),cards[3].getValue()));

	ui->card1->setStyleSheet(cards[0].getBackground());
	ui->card2->setStyleSheet(cards[1].getBackground());
	ui->card3->setStyleSheet(cards[2].getBackground());
	ui->card4->setStyleSheet(cards[3].getBackground());
}

Card Card::getRandomCard()
{
	Card new_card;
	new_card.value = rand()%9+1;
	new_card.background = "border-image:url(':/images/"+new_card.types[rand()%4]+QString::number(new_card.getValue())+".png')";
	return new_card;
}

Card &Card::operator=(const Card & c)
{
	value = c.getValue();
	background = c.background;
	return *this;
}

bool Card::operator ==(const Card & c) const
{
	if(value == c.value && background == c.background)
		return true;
	else
		return false;
}

void GameMainWindow::on_shuffleButton_clicked()
{
	clear();
	putCards();
}

void GameMainWindow::clear()
{
	clear_equation();
	ui->output->setText(equation_for_show);
}

void GameMainWindow::clear_equation()
{
	equation.clear();
	equation_for_show.clear();
	for(int i=0;i<4;i++)
		cards[i].checked = false;
}

void GameMainWindow::backspace()
{
	if(equation.size()>0)
	{
		equation_for_show.resize(equation.size()-1);
		equation.resize(equation.size()-1);
		ui->output->setText(equation_for_show);
	}
}

void GameMainWindow::input_plus()
{
	check_erase_signal();
	if(!equation.isEmpty())
		append("+");
}

void GameMainWindow::input_minus()
{
	check_erase_signal();
	if(!equation.isEmpty())
		append("-");
}

void GameMainWindow::input_times()
{
	check_erase_signal();
	if(!equation.isEmpty())
	{
		equation_for_show += "×";
		equation += "*";
		ui->output->setText(equation_for_show);
	}
}

void GameMainWindow::input_divide()
{
	check_erase_signal();
	if(!equation.isEmpty())
	{
		equation_for_show += "÷";
		equation += "/";
		ui->output->setText(equation_for_show);
	}
}

void GameMainWindow::input_left()
{
	append("(");
}

void GameMainWindow::input_right()
{
	if(equation_for_show[equation.size()-1] == '(')
	{
		equation_for_show += "0";
		equation += "0";
	}
	append(")");
}

double GameMainWindow::calculate(QString & equation)
{
	const QChar TIMES = '*';
	const QChar DIVIDE ='/';
	const QChar PLUS = '+';
	const QChar MINUS = '-';
	/*查找是否有省略乘号情况*/
	int pos = 0;
	while (pos != -1)
	{
		if (pos > 0)
			if ((equation[pos - 1] >= '0' && equation[pos - 1] <= '9') || equation[pos - 1] == ')')
			{
				equation.insert(pos, '*');
				pos += 2;
			}
		pos++;
		pos = equation.indexOf('(', pos);
	}
	pos = 0;
	while (pos != -1)
	{
		if (pos > 0 && pos<equation.size() - 1)
			if ((equation[pos + 1] >= '0' && equation[pos + 1] <= '9') || equation[pos + 1] == '(')
			{
				equation.insert(pos+1, '*');
				pos += 2;
			}
		pos++;
		pos = equation.indexOf(')', pos);
	}

	/*缺失最后一个操作数则抛出错误*/
	if(isSignal(equation[equation.size()-1]))
		throw "缺少运算数";

	/*根据算式中是否还有括号判断不同的路径*/
	auto first_left = equation.indexOf('(');
	auto last_right = equation.lastIndexOf(')');
	if (first_left == last_right)//如果算式当中已经没有括号
	{
		double result;
		double num1, num2, temp_result;
		QVector<QChar> sigs;
		QVector<double> nums;
		QString current_num;
		for (auto i : equation)
			if (i == TIMES || i == DIVIDE || i == PLUS || i == MINUS)
			{
				if (current_num.size() == 0)
				{
					if (i == PLUS || i == MINUS)
						current_num.push_back(i);
					else
						throw "运算符错误";
				}
				else
				{
					sigs.push_back(i);
					nums.push_back(current_num.toDouble());
					current_num.clear();
				}
			}
			else if ((i >= '0'&&i <= '9') || i == '.')
			{
				current_num += i;
			}
		nums.push_back(current_num.toDouble());//最后一个数

		for (int i = 0; i < sigs.size(); i++)
		{
			if (sigs[i] == TIMES)
			{
				num1 = nums[i];
				num2 = nums[i + 1];
				temp_result = num1*num2;
				nums.remove(i, 2);
				sigs.remove(i);
				nums.insert(nums.begin() + i, temp_result);
				i--;
			}
			else if (sigs[i] == DIVIDE)
			{
				num1 = nums[i];
				num2 = nums[i + 1];
				temp_result = num1 / num2;
				nums.remove(i, 2);
				sigs.remove(i);
				nums.insert(nums.begin() + i, temp_result);
				i--;
			}
		}
		result = nums[0];
		for (int i = 0; i < nums.size() - 1; i++)
			if (sigs[i] == PLUS)
				result += nums[i + 1];
			else if (sigs[i] == MINUS)
				result -= nums[i + 1];
		return result;
	}
	else if (first_left == -1)//括号不配对情况
	{
		//equation.remove(equation.lastIndexOf(')'), 1);
		throw "括号不成对";
	}
	else if (last_right == -1)
	{
		//equation.remove(equation.indexOf('('), 1);
		throw "括号不成对";
	}
	else//抹掉括号
	{
		int left, right, left_2, right_2;
		left = equation.indexOf('(');
		right = equation.lastIndexOf(')');
		left_2 = equation.lastIndexOf('(');
		right_2 = equation.indexOf(')');
		if (left_2 >= left && right_2 <= right && left_2 < right_2)//只有嵌套括号
		{
			QString sub_str = equation.mid(first_left + 1, last_right - first_left - 1);
			double sub_result = calculate(sub_str);
			equation.replace(first_left, last_right - first_left + 1, QString::number(sub_result,'g',PRECISION));
			return calculate(equation);
		}
		else//有分离括号
		{
			left = equation.lastIndexOf('(');//找到最右边的左括号
			right = equation.mid(left, equation.size()).indexOf(')') + left;//最右边的左括号后的最左边的右括号
			//此时left和right之间有单位算式
			QString sub_str = equation.mid(left + 1, right - left - 1);
			double sub_result = calculate(sub_str);
			equation.replace(left, right - left + 1, QString::number(sub_result,'g',PRECISION));
			return calculate(equation);
		}
	}
}

void GameMainWindow::check_erase_signal()
{
	if(isSignal(equation[equation.size()-1]))
	{
		equation_for_show.resize(equation.size()-1);
		equation.resize(equation.size()-1);
	}
}

void GameMainWindow::append(QString c)
{
	equation_for_show += c;
	equation += c;
	ui->output->setText(equation_for_show);
}

void GameMainWindow::showErrorDialog()
{
	ErrorDialog * e = new ErrorDialog(this);
	e->setModal(true);
	e->show();
}

void GameMainWindow::equal()
{
	try
	{
		if(calculate(equation) - 24 < 10E11)
		{
			ui->score->setText(QString::number(++score));
			ui->output->setText("恭喜你答对了！");
			putCards();
		}
		else
		{
			ui->output->setText("答错了，再试试？");
		}
		clear_equation();
	}
	catch(const char * c)
	{
		ui->output->setText(QString(c));
		clear_equation();
	}
}

static bool isSignal(QChar c)
{
	return (c=='+'||c=='-'||c=='*'||c=='/');
}

void GameMainWindow::on_card1_clicked()
{
	if(cards[0].checked == false)
	{
		append(QString::number(cards[0].getValue()));
		cards[0].checked = true;
	}
	else
		showErrorDialog();
}

void GameMainWindow::on_card2_clicked()
{
	if(cards[1].checked == false)
	{
		append(QString::number(cards[1].getValue()));
		cards[1].checked = true;
	}
	else
		showErrorDialog();
}

void GameMainWindow::on_card3_clicked()
{
	if(cards[2].checked == false)
	{
		append(QString::number(cards[2].getValue()));
		cards[2].checked = true;
	}
	else
		showErrorDialog();
}

void GameMainWindow::on_card4_clicked()
{
	if(cards[3].checked == false)
	{
		append(QString::number(cards[3].getValue()));
		cards[3].checked = true;
	}
	else
		showErrorDialog();
}

static double plus(double a, double b)
{
	 return a+b;
}
static double minus(double a, double b)
{
	 return a-b;
}
static double times(double a, double b)
{
	 return a*b;
}
static double divide(double a, double b)
{
	if(b == 0)
	{
		if(a>=0)
			return INFINITY;
		else if(a<0)
			return -INFINITY;
	}
	return a/b;
}
static double (*operators[4])(double,double) = {&plus, &minus, &times, &divide};//函数指针
static bool hasAnswer(double a, double b, double c, double d)//穷举所有情况看是否有解
{
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			for(int k=0; k<4; k++)
			{
				//((a#b)#c)#d
				double answer = operators[k](operators[j](operators[i](a, b), c) , d);
				if(fabs(answer -24)<10E-11)
					return true;
				//(a#(b#c))#d
				answer = operators[k](operators[j](a, operators[i](b, c)) , d);
				if(fabs(answer -24)<10E-11)
					return true;
				//a#((b#c)#d)
				answer = operators[k](a, operators[j](operators[i](b, c) , d));
				if(fabs(answer -24)<10E-11)
					return true;
				//(a#b)#(c#d)
				answer = operators[k](operators[i](a, b), operators[j](c, d));
				if(fabs(answer -24)<10E-11)
					return true;
				//a#(b#(c#d))
				answer = operators[k](a, operators[j](b, operators[i](c, d)));
				if(fabs(answer -24)<10E-11)
					return true;
			}
		}
	}
	return false;
}
static QVector<QString> getAnswer(double a, double b, double c, double d)//穷举所有情况得到解
{
	QString a_s = QString::number(a,'g');
	QString b_s = QString::number(b,'g');
	QString c_s = QString::number(c,'g');
	QString d_s = QString::number(d,'g');
	const QString signal[4] = {"+","-","×","÷"};
	QVector<QString> answers;

	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			for(int k=0; k<4; k++)
			{
				//((a#b)#c)#d
				double answer = operators[k](operators[j](operators[i](a, b), c) , d);
				if(fabs(answer -24)<10E-11)
				{
					answers.push_back("(("+a_s+signal[i]+b_s+")"+signal[j]+c_s+")"+signal[k]+d_s);
				}
				//(a#(b#c))#d
				answer = operators[k](operators[j](a, operators[i](b, c)) , d);
				if(fabs(answer -24)<10E-11)
				{
					answers.push_back("("+a_s+signal[j]+"("+b_s+signal[i]+c_s+"))"+signal[k]+d_s);
				}
				//a#((b#c)#d)
				answer = operators[k](a, operators[j](operators[i](b, c) , d));
				if(fabs(answer -24)<10E-11)
				{
					answers.push_back(a_s+signal[k]+"(("+b_s+signal[i]+c_s+")"+signal[j]+d_s+")");
				}
				//(a#b)#(c#d)
				answer = operators[k](operators[i](a, b), operators[j](c, d));
				if(fabs(answer -24)<10E-11)
				{
					answers.push_back("("+a_s+signal[i]+b_s+")"+signal[k]+"("+c_s+signal[j]+d_s+")");
				}
				//a#(b#(c#d))
				answer = operators[k](a, operators[j](b, operators[i](c, d)));
				if(fabs(answer -24)<10E-11)
				{
					answers.push_back(a_s+signal[k]+"("+b_s+signal[j]+"("+c_s+signal[i]+d_s+"))");
				}
			}
		}
	}
	return answers;
}

void GameMainWindow::on_answerButton_clicked()
{
	AnswerDialog * answerDialog = new AnswerDialog(this);
	answerDialog->setModal(true);
	QVector<QString> answers = getAnswer(cards[0].getValue(),cards[1].getValue(),cards[2].getValue(),cards[3].getValue());
	QString answerList = "<center>"+QString("部分答案，仅供参考")+"\n</center>";
	for(QString answer : answers)
	{
		answerList+="<center>"+answer+"\n</center>";
	}
	answerDialog->changeTextEdit(answerList);
	answerDialog->show();
	ui->score->setText(QString::number(--score));
	clear();
	putCards();
}
