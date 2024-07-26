#ifndef MINIMAXGAME_MAINWINDOW_H
#define MINIMAXGAME_MAINWINDOW_H

#include <QMainWindow>
#include "board.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


namespace minimaxgame
{
	class MainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = nullptr);
		MainWindow(const MainWindow &) = delete;
		MainWindow &operator=(const MainWindow &) = delete;
        ~MainWindow();

	private slots:
		void on_actionPvP_triggered(bool);
		void on_actionSaveGame_triggered(bool);


		void on_actionLoadGame_triggered(bool);

		void on_actionP1_Occ_triggered();
		void on_actionP2_Occ_triggered();

		void on_actionPvAi_as_Blue_triggered();

		void on_actionPvAi_as_Red_triggered();

		void on_actionAvA_triggered();

		void on_actionPlayer_as_Blue_triggered();

		void on_actionPlayer_as_Red_triggered();

		void on_actionAi_as_Blue_triggered();

		void on_actionAi_as_Red_triggered();

	private:
		Ui::MainWindow *m_ui;
	};
}
#endif
