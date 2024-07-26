#include <QDebug>
#include <QFile>
#include "board.h"
#include "ui_board.h"
#include "virtualboard.h"
#include "mainwindow.h"
#include "ai.h"
#include "ui_mainwindow.h"

namespace minimaxgame
{
	Board::Board(QWidget *parent)
		: QWidget{ parent }
		, m_ui{ new Ui::Board{} }
	{
		m_ui->setupUi(this);
		initTiles();
	}
	Board::~Board()
	{
		delete m_ui;
	}

	void Board::initTiles()
	{
		for (size_t i = 0; i < TILE_SIZE; i++)
		{
			for (size_t k = 0; k < TILE_SIZE; k++)
			{
				m_tiles[i][k].SetRowCol(i, k);
				m_tiles[i][k].setParent(this);
				m_ui->gridLayout_2->addWidget(&m_tiles[i][k], i, k, 1, 1);
				connect(&m_tiles[i][k], &TileWidget::TileClicked, this, &Board::TileClicked);
			}
		}
		resetPlayers();
	}

	void Board::resetBoard(bool isBackground)
	{
		for (size_t i = 0; i < TILE_SIZE; i++)
		{
			for (size_t k = 0; k < TILE_SIZE; k++)
			{
				m_tiles[i][k].SetStatus(TileStatus::Idle);
			}
		}
		m_turnNumber = 0;
		m_gameOver = false;
		delete m_gameOverMessage;
		m_gameOverMessage = nullptr;
		m_turn = MoveTurn::Player1Move;
		if (!isBackground)
			m_gameMoves.clear();
		resetPlayers();
		repaint();
	}

	void Board::resetPlayers()
	{
		m_tiles[0][3].SetStatus(TileStatus::Player1);
		m_player1 = &m_tiles[0][3];
		m_tiles[6][3].SetStatus(TileStatus::Player2);
		m_player2 = &m_tiles[6][3];
	}

	bool Board::movePlayer(TileWidget *tile, MoveTurn move, bool isBackground)
	{
		if (m_gameOver)
		{
			auto m = dynamic_cast<Ui::MainWindow *>(parent());
			m->statusbar->showMessage(*m_gameOverMessage);
			//SendStatusMessage(parent(), *m_gameOverMessage);
			return false;
		}

		TileWidget *player = nullptr;
		TileStatus status;

		if (move == MoveTurn::Player1Move)
		{
			player = m_player1;
			status = TileStatus::Player1;
		}
		else if (move == MoveTurn::Player2Move)
		{
			player = m_player2;
			status = TileStatus::Player2;
		}
		else
			return false;


		if (!tile->IsIdle())
			return false;

		if (!IsValidMove(player, tile))
			return false;

		if (noMoveLeft(player))
		{
			m_gameOver = true;
			return false;
		}

		tile->SetStatus(status);
		if (!isBackground)
			tile->repaint();
		player->SetStatus(TileStatus::Idle);
		if (!isBackground)
			player->repaint();
		player = tile;


		if (move == MoveTurn::Player1Move)
		{
			m_player1 = player;
			m_turn = MoveTurn::Player1Block;
		}
		else if (move == MoveTurn::Player2Move)
		{
			m_player2 = player;
			m_turn = MoveTurn::Player2Block;
		}
		else
			return false;

		return true;
	}
	bool Board::blockTile(TileWidget *tile, MoveTurn move, bool isBackground)
	{
		if (!tile->IsIdle())
			return false;

		tile->SetStatus(TileStatus::Blocked);
		if (!isBackground)
			tile->repaint();

		if (move == MoveTurn::Player1Block)
		{
			m_turn = MoveTurn::Player2Move;
			m_turnNumber++;
			if (!isBackground)
				m_gameMoves.append(QString{ "%1;%2;%3;%4;%5;%6;" }
					.arg(m_turnNumber)
					.arg(1)
					.arg(QChar('a' + m_player1->GetCol()))
					.arg(m_player1->GetRow() + 1)
					.arg(QChar('a' + tile->GetCol()))
					.arg(tile->GetRow() + 1)
                );

			if (noMoveLeft(m_player2))
			{
				if (m_gameOverMessage)
					delete m_gameOverMessage;
				m_gameOverMessage = new QString{ "Game is over. Player1 won!" };
                m_gameOver = true;
				qDebug() << "GameOver Player1 won!";
				return false;
			}
		}
		else if (move == MoveTurn::Player2Block)
		{
			m_turn = MoveTurn::Player1Move;
			m_turnNumber++;
			if (!isBackground)
				m_gameMoves.append(QString{ "%1;%2;%3;%4;%5;%6;" }
					.arg(m_turnNumber)
					.arg(2)
					.arg(QChar('a' + m_player2->GetCol()))
					.arg(m_player2->GetRow() + 1)
					.arg(QChar('a' + tile->GetCol()))
					.arg(tile->GetRow() + 1)
                );

			if (noMoveLeft(m_player1))
			{
				if (m_gameOverMessage)
					delete m_gameOverMessage;
				m_gameOverMessage = new QString{ "Game is over. Player2 won!" };
                m_gameOver = true;
				qDebug() << "GameOver Player2 won!";
				return false;
			}
		}
		else
			return false;

		return true;
	}
	bool Board::noMoveLeft(TileWidget *player)
	{
		for (int i = -1; i <= 1; i++)
		{
			int row = player->GetRow() + i;
			if (!(row >= 0 && row < 7))
				continue;
			for (int k = -1; k <= 1; k++)
			{
				int col = player->GetCol() + k;
				if (!(col >= 0 && col < 7))
					continue;

				if (IsValidMove(player, &m_tiles[row][col]))
					return false;
			}
		}

		return true;
	}
	void Board::parseGameMoves()
	{
		resetBoard(true);

		if (m_gameMoves.size() <= 0)
			return;


		for (size_t i = 0; i < m_gameMoves.size(); i++)
		{
			QRegularExpressionMatch  match = m_move_re.match(m_gameMoves[i]);
			if (match.hasMatch())
			{
				tileClicked(&m_tiles[match.captured(4).toInt() - 1][match.captured(3)[0].toLower().unicode() - 'a'], true);
				tileClicked(&m_tiles[match.captured(6).toInt() - 1][match.captured(5)[0].toLower().unicode() - 'a'], true);
				qDebug() << match.captured(3)[0].toLower().unicode() - 'a' << " " << match.captured(4).toInt();
			}
		}
		repaint();
	}
	void Board::TileClicked(TileWidget *tile)
	{
		tileClicked(tile);
	}

	bool Board::CheckGameFinished()
	{
		if (m_turn == MoveTurn::Player1Move || m_turn == MoveTurn::Player1Block)
		{
			if (noMoveLeft(m_player1))
			{
				m_gameOver = true;
				return m_gameOver;
			}
		}
		else if (m_turn == MoveTurn::Player2Move || m_turn == MoveTurn::Player2Block)
		{
			if (noMoveLeft(m_player2))
			{
				m_gameOver = true;
				return m_gameOver;
			}
		}
	}

	void Board::TriggerAi()
	{
		if (CheckGameFinished())
			return;
		if (m_aiMove == AiMove::Player1 && m_turn == MoveTurn::Player1Move)
		{
			{
				Ai ai{ this };
				auto move = ai.FindMoves(m_turn);
				movePlayer(&m_tiles.at(move.row).at(move.col), m_turn, false);
			}
			{
				Ai ai{ this };
				auto move = ai.FindMoves(m_turn);
				blockTile(&m_tiles.at(move.row).at(move.col), m_turn, false);
			}
			if (m_tourMode == TournamentMode::AIasBlue)
			{
				SaveCurrentGame(m_tournamentFile);
			}
		}
		else if (m_aiMove == AiMove::Player2 && m_turn == MoveTurn::Player2Move)
		{
			{
				Ai ai{ this };
				auto move = ai.FindMoves(m_turn);
				movePlayer(&m_tiles.at(move.row).at(move.col), m_turn, false);
			}
			{
				Ai ai{ this };
				auto move = ai.FindMoves(m_turn);
				blockTile(&m_tiles.at(move.row).at(move.col), m_turn, false);
			}
			if (m_tourMode == TournamentMode::AIasRed)
			{
				SaveCurrentGame(m_tournamentFile);
			}
		}
		else if (m_aiMove == AiMove::Both && (m_turn == MoveTurn::Player1Move || m_turn == MoveTurn::Player2Move))
		{
			{
				Ai ai{ this };
				auto move = ai.FindMoves(m_turn);
				movePlayer(&m_tiles.at(move.row).at(move.col), m_turn, false);
			}
			{
				Ai ai{ this };
				auto move = ai.FindMoves(m_turn);
				blockTile(&m_tiles.at(move.row).at(move.col), m_turn, false);
			}
			if (!m_gameOver)
				TriggerAi();
		}
	}

	void Board::tileClicked(TileWidget *tile, bool isbackground)
	{
		CheckGameFinished();
		if (m_gameOver)
			return;

		if (m_tourMode == TournamentMode::None || isbackground)
		{
			switch (m_turn)
			{
			case MoveTurn::Player1Move:
			case MoveTurn::Player2Move:
				movePlayer(tile, m_turn, isbackground);
				break;
			case MoveTurn::Player1Block:
			case MoveTurn::Player2Block:
				blockTile(tile, m_turn, isbackground);
				if (!isbackground)
					TriggerAi();
				break;
			}
		}
		else if (m_tourMode == TournamentMode::PlayerAsBlue)
		{
			switch (m_turn)
			{
			case MoveTurn::Player1Move:
				movePlayer(tile, m_turn, isbackground);
				break;
			case MoveTurn::Player1Block:
				blockTile(tile, m_turn, isbackground);
				if (!isbackground)
					SaveCurrentGame(m_tournamentFile);
				break;
			}
		}
		else if (m_tourMode == TournamentMode::PlayerAsRed)
		{
			switch (m_turn)
			{
			case MoveTurn::Player2Move:
				movePlayer(tile, m_turn, isbackground);
				break;
			case MoveTurn::Player2Block:
				blockTile(tile, m_turn, isbackground);
				if (!isbackground)
					SaveCurrentGame(m_tournamentFile);
				break;
			}
		}
	}

	void Board::paintEvent(QPaintEvent *e)
	{
		QPainter painter{ this };
		QPen pen{ Qt::black };

		pen.setWidth(1);
		painter.setPen(pen);

		//painter.drawRect(this->rect());
		painter.drawRect(this->rect().topLeft().x(), this->rect().topLeft().y(),
			this->rect().width() - 1, this->rect().height() - 1);
	}
	void Board::NewPvPGame()
	{
		qDebug() << "New PvP setting up";
		resetBoard(false);
		m_aiMove = AiMove::None;
        m_tourMode = TournamentMode::None;
	}

	void Board::NewPvAiGame(AiMove aiPlayer)
	{
		qDebug() << "New PvAi setting up";
		resetBoard(false);
        m_tourMode = TournamentMode::None;
		m_aiMove = aiPlayer;
		TriggerAi();
	}


	void Board::NewAvAGame()
	{
		qDebug() << "New PvAi setting up";
		resetBoard(false);
        m_tourMode = TournamentMode::None;
		m_aiMove = AiMove::Both;
		TriggerAi();
	}

	void Board::SaveCurrentGame(const QString &path)
	{
		qDebug() << path;

		QFile fOut(path);
		if (fOut.open(QFile::WriteOnly | QFile::Text))
		{
			QTextStream s(&fOut);
			for (int i = 0; i < m_gameMoves.size(); ++i)
				s << m_gameMoves.at(i) << '\n';
		}
		else
		{
			return;
		}
		//fOut.close();
	}

	void Board::LoadGame(const QString &path)
	{
		qDebug() << path;
		QStringList stringList;
		QFile fIn(path);
		if (fIn.open(QFile::ReadOnly | QFile::Text))
		{
			QTextStream s(&fIn);
			while (true)
			{
				QString line = s.readLine();
				if (line.isNull())
					break;

				if (m_move_re.match(line).hasMatch())
					stringList.append(line);
				else
				{
					return;
				}
			}
		}
		else
		{
			return;
		}
		m_gameMoves = stringList;
		parseGameMoves();
		//fOut.close();
	}

	void Board::handleTournament(const QString &path)
	{
		LoadGame(path);
		TriggerAi();
		//SaveCurrentGame(path);
	}

	void Board::DisconnectFromTournament()
	{
		if (!m_tournamentFile.isNull() || !m_tournamentFile.isEmpty())
		{
			disconnect(&watcher, SIGNAL(fileChanged(const QString &)), this, SLOT(handleTournament(const QString &)));
			watcher.removePath(m_tournamentFile);
		}
		m_tourMode = TournamentMode::None;
	}

	void Board::NewTournamentPlayerAsBlue(const QString &path)
	{
		m_aiMove = AiMove::None;
		NewTournament(path, TournamentMode::PlayerAsBlue);
	}

	void Board::NewTournamentPlayerAsRed(const QString &path)
	{
		m_aiMove = AiMove::None;
		NewTournament(path, TournamentMode::PlayerAsRed);
	}

	void Board::NewTournamentAiAsBlue(const QString &path)
	{
		m_aiMove = AiMove::Player1;
		NewTournament(path, TournamentMode::AIasBlue);
	}

	void Board::NewTournamentAiAsRed(const QString &path)
	{
		NewTournament(path, TournamentMode::AIasRed);
		m_aiMove = AiMove::Player2;
	}

	void Board::NewTournament(const QString &path, TournamentMode tm)
	{
		DisconnectFromTournament();
		m_tourMode = tm;
		m_tournamentFile = path;
		watcher.addPath(m_tournamentFile);
		LoadGame(m_tournamentFile);
		connect(&watcher, SIGNAL(fileChanged(const QString &)), this, SLOT(handleTournament(const QString &)));
		TriggerAi();
	}
}
