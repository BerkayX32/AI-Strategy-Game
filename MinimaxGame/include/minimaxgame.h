#ifndef MINIMAXGAME_MINIMAXGAME_H
#define MINIMAXGAME_MINIMAXGAME_H

#include <math.h>

namespace minimaxgame
{

	constexpr size_t TILE_SIZE = 7;

	struct Move
	{
		int row;
		int col;
		double score;
	};

	enum class TileStatus
	{
		Idle,
		Player1,
		Player2,
		Blocked,
		VirtualOccupy,
		VirtualNoScoreOccupy,
	};

	enum class MoveTurn
	{
		Player1Move,
		Player1Block,
		Player2Move,
		Player2Block,
	};

	enum class AiMove
	{
		None,
		Player1,
		Player2,
		Both,
	};

	enum class TournamentMode
	{
		None,
		PlayerAsBlue,
		PlayerAsRed,
		AIasBlue,
		AIasRed,
	};

	inline int tileDistance(int row1, int col1, int row2, int col2)
	{
		int row_dist = abs(row1 - row2);
		int col_dist = abs(col1 - col2);

		if (row_dist > col_dist)
			return row_dist;
		else
			return col_dist;
	}
}

#endif