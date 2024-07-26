#include "virtualboard.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <limits>
#include "ai.h"

namespace minimaxgame
{

	MainWindow::MainWindow(QWidget *parent)
		: QMainWindow(parent)
		, m_ui{ new Ui::MainWindow{} }

	{
		m_ui->setupUi(this);
		//m_ui->graphicsView->setScene(m_board->GetScene());
		//m_ui->graphicsView->mapFromGlobal(QCursor::pos());
	}

	MainWindow::~MainWindow()
	{
		delete m_ui;
    }


	void MainWindow::on_actionSaveGame_triggered(bool)
	{
		if (!m_ui->widget)
			return;


		QFileDialog dialog(this);
		dialog.setWindowTitle(QString{ "Save Current Game" });
		dialog.setFileMode(QFileDialog::AnyFile);
		dialog.setAcceptMode(QFileDialog::AcceptSave);
		dialog.setNameFilter(QString("Moves (*.txt)"));
		QStringList fileNames;
		if (dialog.exec())
		{
			fileNames = dialog.selectedFiles();
			m_ui->widget->SaveCurrentGame(fileNames.at(0));
		}

	}

	void MainWindow::on_actionLoadGame_triggered(bool)
	{
		if (!m_ui->widget)
			return;

		QFileDialog dialog(this);
		dialog.setWindowTitle(QString{ "Load Game" });
		dialog.setFileMode(QFileDialog::AnyFile);
		dialog.setAcceptMode(QFileDialog::AcceptOpen);
		dialog.setNameFilter(QString("Moves (*.txt)"));
		QStringList fileNames;
		if (dialog.exec())
		{
			fileNames = dialog.selectedFiles();
			m_ui->widget->LoadGame(fileNames.at(0));
		}

	}


	void MainWindow::on_actionP1_Occ_triggered()
	{
		VirtualBoard vb{ m_ui->widget };

		qDebug() << vb.EvaluatePlayer1();
	}


	void MainWindow::on_actionP2_Occ_triggered()
	{
		VirtualBoard vb{ m_ui->widget };

		qDebug() << vb.EvaluatePlayer2();
	}


	void MainWindow::on_actionPvP_triggered(bool)
	{
		auto board = dynamic_cast<Board *>(m_ui->widget);
		if (!board)
			return;

		board->NewPvPGame();
	}


	void MainWindow::on_actionPvAi_as_Blue_triggered()
	{
		auto board = dynamic_cast<Board *>(m_ui->widget);
		if (!board)
			return;

		board->NewPvAiGame(AiMove::Player2);
	}


	void MainWindow::on_actionPvAi_as_Red_triggered()
	{
		auto board = dynamic_cast<Board *>(m_ui->widget);
		if (!board)
			return;

		board->NewPvAiGame(AiMove::Player1);
	}


	void MainWindow::on_actionAvA_triggered()
	{
		auto board = dynamic_cast<Board *>(m_ui->widget);
		if (!board)
			return;

		board->NewAvAGame();
	}


	void MainWindow::on_actionPlayer_as_Blue_triggered()
	{

		if (!m_ui->widget)
			return;

		auto board = dynamic_cast<Board *>(m_ui->widget);
		if (!board)
			return;

		QFileDialog dialog(this);
		dialog.setWindowTitle(QString{ "Select Tournament File" });
		dialog.setFileMode(QFileDialog::AnyFile);
		dialog.setAcceptMode(QFileDialog::AcceptOpen);
		dialog.setNameFilter(QString("Tournament File (*.txt)"));
		QStringList fileNames;
		if (dialog.exec())
		{
			fileNames = dialog.selectedFiles();
			board->NewTournamentPlayerAsBlue(fileNames.at(0));
		}
	}


	void MainWindow::on_actionPlayer_as_Red_triggered()
	{

		if (!m_ui->widget)
			return;

		auto board = dynamic_cast<Board *>(m_ui->widget);
		if (!board)
			return;

		QFileDialog dialog(this);
		dialog.setWindowTitle(QString{ "Select Tournament File" });
		dialog.setFileMode(QFileDialog::AnyFile);
		dialog.setAcceptMode(QFileDialog::AcceptOpen);
		dialog.setNameFilter(QString("Tournament File (*.txt)"));
		QStringList fileNames;
		if (dialog.exec())
		{
			fileNames = dialog.selectedFiles();
			board->NewTournamentPlayerAsRed(fileNames.at(0));
		}

	}


	void MainWindow::on_actionAi_as_Blue_triggered()
	{


		if (!m_ui->widget)
			return;

		auto board = dynamic_cast<Board *>(m_ui->widget);
		if (!board)
			return;

		QFileDialog dialog(this);
		dialog.setWindowTitle(QString{ "Select Tournament File" });
		dialog.setFileMode(QFileDialog::AnyFile);
		dialog.setAcceptMode(QFileDialog::AcceptOpen);
		dialog.setNameFilter(QString("Tournament File (*.txt)"));
		QStringList fileNames;
		if (dialog.exec())
		{
			fileNames = dialog.selectedFiles();
			board->NewTournamentAiAsBlue(fileNames.at(0));
		}
	}


	void MainWindow::on_actionAi_as_Red_triggered()
	{


		if (!m_ui->widget)
			return;

		auto board = dynamic_cast<Board *>(m_ui->widget);
		if (!board)
			return;

		QFileDialog dialog(this);
		dialog.setWindowTitle(QString{ "Select Tournament File" });
		dialog.setFileMode(QFileDialog::AnyFile);
		dialog.setAcceptMode(QFileDialog::AcceptOpen);
		dialog.setNameFilter(QString("Tournament File (*.txt)"));
		QStringList fileNames;
		if (dialog.exec())
		{
			fileNames = dialog.selectedFiles();
			board->NewTournamentAiAsRed(fileNames.at(0));
		}
	}

}
