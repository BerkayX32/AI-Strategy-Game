#ifndef MINIMAXGAME_AI_H
#define MINIMAXGAME_AI_H

#include <qlist.h>

namespace minimaxgame
{
	class Board;
	class TileWidget;
	class VirtualBoard;
	class Ai
	{

	public:
		Ai(const Board *);
		Ai(const VirtualBoard *);
		~Ai();
		Move FindBestPlayer1Move() const;
		Move FindBestPlayer2Move() const;
		Move FindMoves(MoveTurn);
		const QList<Move> &GetMoves() const { return m_moves; };
		Move GetRandomFreeTile();

	private:
		bool moveMax(MoveTurn, bool isBlock = false);
		Move blockMin(MoveTurn, bool isBlock = false);
		const VirtualBoard *m_vb;
		QList<Move> m_moves{};
		void sortMoves();
	};

	inline TileStatus SwitchStatus(TileStatus status)
	{
		if (status == TileStatus::Player1)
			return TileStatus::Player2;

		if (status == TileStatus::Player2)
			return TileStatus::Player1;

		throw std::exception{};
	}
}


#endif