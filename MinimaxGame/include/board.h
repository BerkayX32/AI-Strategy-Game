#ifndef MINIMAXGAME_BOARD_H
#define MINIMAXGAME_BOARD_H


#include <QWidget>
#include <QRegularExpression>
#include <array>
#include "tilewidget.h"
#include "graphicsscene.h"
#include <QFileSystemWatcher>


QT_BEGIN_NAMESPACE
namespace Ui { class Board; }
QT_END_NAMESPACE

namespace minimaxgame
{
	class Board : public QWidget
	{
		Q_OBJECT
	public:
		explicit Board(QWidget *parent = nullptr);
		~Board();
	protected:
		virtual void paintEvent(QPaintEvent *)override;

	public:
		void NewPvPGame();
		void NewPvAiGame(AiMove);
		void NewAvAGame();
		void NewTournamentPlayerAsBlue(const QString &path);
		void NewTournamentPlayerAsRed(const QString &path);
		void NewTournamentAiAsBlue(const QString &path);
		void NewTournamentAiAsRed(const QString &path);
		void SaveCurrentGame(const QString &);
		void LoadGame(const QString &);
		const std::array<std::array<TileWidget, TILE_SIZE>, TILE_SIZE> &GetTiles()const { return m_tiles; };
		const TileWidget *Player1()const { return m_player1; };
		const TileWidget *Player2()const { return m_player2; };
		TileWidget *GetTile(int row, int col) { return &m_tiles.at(row).at(col); };
		const MoveTurn &GetTurn() const { return m_turn; };
		void DisconnectFromTournament();

		void TriggerAi();
		bool CheckGameFinished();

	public slots:
		void TileClicked(TileWidget *);
	private slots:
		void handleTournament(const QString &);

	private:
		void initTiles();
		void resetBoard(bool isBackground = false);
		void resetPlayers();
		bool movePlayer(TileWidget *, MoveTurn, bool isBackground = false);
		bool blockTile(TileWidget *tile, MoveTurn, bool isBackground = false);
		bool noMoveLeft(TileWidget *);
		void parseGameMoves();
		void tileClicked(TileWidget *, bool isbackground = false);
		void NewTournament(const QString &, TournamentMode);

	private:
		Ui::Board *m_ui;
		std::array<std::array<TileWidget, TILE_SIZE>, TILE_SIZE> m_tiles;

		MoveTurn m_turn{ MoveTurn::Player1Move };
		TileWidget *m_player1 = nullptr;
		TileWidget *m_player2 = nullptr;
		AiMove m_aiMove{ AiMove::None };
		TournamentMode m_tourMode{ TournamentMode::None };

	private:
		int m_turnNumber = 0;
		bool m_gameOver = false;
		QString *m_gameOverMessage = nullptr;
		QFileSystemWatcher watcher{};
		QString m_tournamentFile{};
		QStringList m_gameMoves;
		const QRegularExpression m_move_re{ R"((\d);([12]);([abcdefg]);([1234567]);([abcdefg]);([1234567]);)", QRegularExpression::CaseInsensitiveOption };
	};
}
#endif