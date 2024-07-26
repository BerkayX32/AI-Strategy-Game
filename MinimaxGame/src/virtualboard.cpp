
#include "virtualboard.h"
#include "board.h"
#include <qdebug.h>



namespace minimaxgame
{
	VirtualBoard::VirtualBoard(const Board *board)
	{
		for (size_t i = 0; i < TILE_SIZE; i++)
		{
			for (size_t k = 0; k < TILE_SIZE; k++)
			{
				m_tiles[i][k].SetRowCol(i, k);
				if (board != nullptr)
				{
					m_tiles[i][k].SetStatus(board->GetTiles()[i][k].GetStatus());
					if (board->GetTiles()[i][k].GetStatus() == TileStatus::Player1)
						m_player1 = &m_tiles[i][k];
					else if (board->GetTiles()[i][k].GetStatus() == TileStatus::Player2)
						m_player2 = &m_tiles[i][k];
				}
			}
		}
		m_turn = board->GetTurn();
	}


	VirtualBoard::VirtualBoard(const VirtualBoard *board)
	{
		for (size_t i = 0; i < TILE_SIZE; i++)
		{
			for (size_t k = 0; k < TILE_SIZE; k++)
			{
				m_tiles.at(i).at(k).SetRowCol(i, k);
				if (board != nullptr)
				{
					m_tiles[i][k].SetStatus(board->GetStatus(i, k));
					if (board->GetStatus(i, k) == TileStatus::Player1)
						m_player1 = &m_tiles.at(i).at(k);
					else if (board->GetStatus(i, k) == TileStatus::Player2)
						m_player2 = &m_tiles.at(i).at(k);
				}
			}
		}
		m_turn = board->m_turn;
	}

	void VirtualBoard::Player1Move(int row, int col)
	{
		m_player1->SetStatus(TileStatus::Idle);
		*m_player1 = m_tiles.at(row).at(col);
		m_player1->SetStatus(TileStatus::Player1);
		m_turn = MoveTurn::Player1Block;
	}

	void VirtualBoard::Player2Move(int row, int col)
	{
		m_player2->SetStatus(TileStatus::Idle);
		*m_player2 = m_tiles.at(row).at(col);
		m_player2->SetStatus(TileStatus::Player2);
		m_turn = MoveTurn::Player2Block;
	}

	void VirtualBoard::Player1Block(int row, int col)
	{
		auto &tile = m_tiles.at(row).at(col);
		if (!tile.IsIdle())
			return;

		tile.SetStatus(TileStatus::Blocked);
		m_turn = MoveTurn::Player2Move;
	}

	void VirtualBoard::Player2Block(int row, int col)
	{
		auto &tile = m_tiles.at(row).at(col);
		if (!tile.IsIdle())
			return;

		tile.SetStatus(TileStatus::Blocked);
		m_turn = MoveTurn::Player1Move;
	}
	double VirtualBoard::evaluatePlayer(TileStatus p) const
	{
		VirtualBoard vb{ this };
		int pRow;
		int pCol;
		if (p == TileStatus::Player1)
		{
			pRow = vb.Player1()->GetRow();
			pCol = vb.Player1()->GetCol();
			vb.m_player2->SetStatus(TileStatus::Idle);
		}
		else if (p == TileStatus::Player2)
		{
			pRow = vb.Player2()->GetRow();
			pCol = vb.Player2()->GetCol();
			vb.m_player1->SetStatus(TileStatus::Idle);
		}
		else
			throw std::exception{};

		double score = std::numeric_limits<int>::max();
		double s0 = vb.SetOccupations(pRow, pCol, 1);
		for (int i = -2; i <= 2; i++)
		{
			int row = pRow + i;
			if (!(row >= 0 && row < 7))
				continue;
			for (int k = -2; k <= 2; k++)
			{
				int col = pCol + k;
				if (!(col >= 0 && col < 7))
					continue;
				if (vb.GetStatus(row, col) != TileStatus::Idle)
					continue;

				vb.SetTileStatus(row, col, TileStatus::Blocked);
				//-------------------------------------------------------------------//
				double s1 = vb.SetOccupations(pRow, pCol, 2);

				for (int i = -2; i <= 2; i++)
				{
					int row = pRow + i;
					if (!(row >= 0 && row < 7))
						continue;
					for (int k = -2; k <= 2; k++)
					{
						int col = pCol + k;
						if (!(col >= 0 && col < 7))
							continue;
						if (vb.GetStatus(row, col) != TileStatus::Idle)
							continue;

						vb.SetTileStatus(row, col, TileStatus::Blocked);
						//-------------------------------------------------------------------//
						double s2 = vb.SetOccupations(pRow, pCol, 3);

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
								if (vb.GetStatus(row, col) != TileStatus::Idle)
									continue;

								vb.SetTileStatus(row, col, TileStatus::Blocked);
								//-------------------------------------------------------------------//
								double s3 = vb.SetOccupations(pRow, pCol, 3);

								if (s0 + s1 + s2 + s3 < score)
									score = s0 + s1 + s2 + s3;

								vb.SetTileStatus(row, col, TileStatus::Idle);
							}
						}
						vb.SetTileStatus(row, col, TileStatus::Idle);
					}
				}
				vb.SetTileStatus(row, col, TileStatus::Idle);
			}
		}
		if (score == std::numeric_limits<int>::max())
			return std::numeric_limits<int>::min();
		return score;
	}

	double VirtualBoard::EvaluatePlayer(TileStatus s) const
	{
		return evaluatePlayer(s);
	}

	double VirtualBoard::EvaluatePlayer1() const
	{
		return evaluatePlayer(TileStatus::Player1);
	}

	double VirtualBoard::EvaluatePlayer2() const
	{
		return evaluatePlayer(TileStatus::Player2);
	}

	void VirtualBoard::setTiles(const Board *board)
	{
		for (size_t i = 0; i < TILE_SIZE; i++)
		{
			for (size_t k = 0; k < TILE_SIZE; k++)
			{
				m_tiles[i][k].SetRowCol(i, k);
				if (board != nullptr)
				{
					m_tiles[i][k].SetStatus(board->GetTiles()[i][k].GetStatus());
				}
			}
		}
	}

	void VirtualBoard::resetVirtualTiles()
	{
		for (size_t i = 0; i < TILE_SIZE; i++)
		{
			for (size_t k = 0; k < TILE_SIZE; k++)
			{
				if (m_tiles[i][k].GetStatus() == TileStatus::VirtualOccupy)
					m_tiles[i][k].SetStatus(TileStatus::Idle);
			}
		}

		if (m_player1 != nullptr)
			m_player1->SetStatus(TileStatus::Player1);

		if (m_player2 != nullptr)
			m_player2->SetStatus(TileStatus::Player2);
	}


	void VirtualBoard::setVirtualOccupy(VirtualTile *tile)
	{
		if (!tile->IsVirtuallyBlocked())
			tile->SetStatus(TileStatus::VirtualOccupy);
	}

	double VirtualBoard::SetOccupations(int row, int col, int maxDepth) const
	{
		VirtualBoard vb{ this };
		vb.setOccupations(&vb.m_tiles.at(row).at(col), 0, maxDepth);

		return vb.getOccupationScore(&vb.m_tiles.at(row).at(col), maxDepth);
	}

	void VirtualBoard::setOccupations(VirtualTile *tile, int depth, int maxDepth)
	{
		depth++;
		if (depth > maxDepth)
			return;

		for (int i = -1; i <= 1; i++)
		{
			int row = tile->GetRow() + i;
			if (!(row >= 0 && row < 7))
				continue;
			for (int k = -1; k <= 1; k++)
			{
				int col = tile->GetCol() + k;
				if (!(col >= 0 && col < 7))
					continue;

				if (m_tiles[row][col].GetStatus() != TileStatus::Blocked)
				{
					setVirtualOccupy(&m_tiles[row][col]);
					setOccupations(&m_tiles[row][col], depth, maxDepth);
				}
			}
		}
	}

	double VirtualBoard::getOccupationScore(VirtualTile *tile, int maxDepth)
	{
		double score = 0;

		for (size_t i = 0; i < TILE_SIZE; i++)
			for (size_t k = 0; k < TILE_SIZE; k++)
				if (m_tiles[i][k].GetStatus() == TileStatus::VirtualOccupy)
					score += pow(4.4, maxDepth - tileDistance(tile->GetRow(), tile->GetCol(), i, k));

		return score;
	}

	int VirtualBoard::getOccupationCount()
	{
		int count = 0;

		for (size_t i = 0; i < TILE_SIZE; i++)
			for (size_t k = 0; k < TILE_SIZE; k++)
				if (m_tiles[i][k].GetStatus() == TileStatus::VirtualOccupy)
					count++;

		return count;
	}
}
