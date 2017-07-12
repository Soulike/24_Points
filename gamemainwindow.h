#ifndef GAMEMAINWINDOW_H
#define GAMEMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class GameMainWindow;
}

class GameMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit GameMainWindow(QWidget *parent = 0);
	~GameMainWindow();

private:
	Ui::GameMainWindow *ui;
};

#endif // GAMEMAINWINDOW_H
