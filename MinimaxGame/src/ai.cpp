#include "virtualboard.h"
#include "tilewidget.h"
#include "board.h"
#include "mainwindow.h"
#include "ai.h"


namespace minimaxgame
{

	Ai::Ai(const Board *board)
		:m_vb{ new VirtualBoard{board} }
	{
	}

	Ai::Ai(const VirtualBoard *board)
		:m_vb{ new VirtualBoard{board} }
	{
	}

	Ai::~Ai()
	{
		delete m_vb;
	}

	Move Ai::FindBestPlayer1Move() const
	{
		VirtualBoard vb{ m_vb };
		Ai ai{ &vb };
		ai.moveMax(MoveTurn::Player1Move);
		ai.sortMoves();
		Move best_move = ai.GetMoves().at(0);
		return best_move;
	}

	Move Ai::FindBestPlayer2Move() const
	{
		VirtualBoard vb{ m_vb };
		Ai ai{ &vb };
		ai.moveMax(MoveTurn::Player2Move);
		ai.sortMoves();
		Move best_move = ai.GetMoves().at(0);
		return best_move;
	}

	Move Ai::FindMoves(MoveTurn turn)
	{
		switch (turn)
		{
		case MoveTurn::Player1Move:
		case MoveTurn::Player2Move:
			moveMax(turn);
			sortMoves();
			if (m_moves.size() <= 0)
				return GetRandomFreeTile();
			return m_moves.at(0);
			break;
		case MoveTurn::Player1Block:
			moveMax(MoveTurn::Player2Move, true);
			sortMoves();
			if (m_moves.size() <= 0)
				return GetRandomFreeTile();
			return m_moves.at(0);
			break;
		case MoveTurn::Player2Block:
			moveMax(MoveTurn::Player1Move, true);
			sortMoves();
			if (m_moves.size() <= 0)
				return GetRandomFreeTile();
			return m_moves.at(0);
			break;
		}
	}

	Move Ai::GetRandomFreeTile()
	{
		for (int i = 0; i < TILE_SIZE; i++)
		{
			for (int k = 0; k < TILE_SIZE; k++)
			{
				if (m_vb->GetTilesV().at(i).at(k).IsIdle())
					return Move{i, k, 0};
			}
		}
		return Move{ 0, 0, 0 };
	}

	void Ai::sortMoves()
	{
		std::sort(m_moves.begin(), m_moves.end(), [](Move a, Move b) {return a.score > b.score; });
	}

	Move Ai::blockMin(MoveTurn turn, bool isBlock)
	{
		return Move{ 0,0 };
	}

	bool Ai::moveMax(MoveTurn turn, bool isBlock)
	{
		VirtualBoard v{ m_vb };
		int pRow;
		int pCol;
		if (turn == MoveTurn::Player1Move || turn == MoveTurn::Player2Block)
		{
			pRow = v.Player1()->GetRow();
			pCol = v.Player1()->GetCol();
		}
		else if (turn == MoveTurn::Player2Move || turn == MoveTurn::Player1Block)
		{
			pRow = v.Player2()->GetRow();
			pCol = v.Player2()->GetCol();
		}
		else
			throw std::exception{};

		auto firstStatus = v.GetStatus(pRow, pCol);

		double score = std::numeric_limits<int>::min();
		int mR = -1;
		int mC = -1;
		for (int i = -1; i <= 1; i++)
		{
			int row = pRow + i;
			if (!(row >= 0 && row < 7))
				continue;
			for (int k = -1; k <= 1; k++)
			{
				int col = pCol + k;
				if (!(col >= 0 && col < 7))
					continue;
				if (!v.GetTilesV().at(row).at(col).IsIdle())
					continue;

				v.SetTileStatus(pRow, pCol, TileStatus::Idle);
				v.SetTileStatus(row, col, firstStatus);

				VirtualBoard vb{ v };
				auto s1 = vb.EvaluatePlayer(firstStatus);
				auto s2 = vb.EvaluatePlayer(SwitchStatus(firstStatus));

				m_moves.append(Move{ row, col, s1 - s2 });
				if (s1 - s2 > score)
				{
					score = s1 - s2;
					mR = row;
					mC = col;
				}

				v.SetTileStatus(pRow, pCol, firstStatus);
				v.SetTileStatus(row, col, TileStatus::Idle);
			}
		}
		if (mR < 0 || mC < 0)
			return false;
		return true;
	}
}

