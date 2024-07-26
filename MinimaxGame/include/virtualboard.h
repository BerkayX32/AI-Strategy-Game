#ifndef MINIMAXGAME_VIRTUALBOARD_H
#define MINIMAXGAME_VIRTUALBOARD_H

#include <array>
#include "virtualtile.h"

namespace minimaxgame
{
	constexpr int MAX_OCC_DEPTH = 3;
	class Board;
	class VirtualBoard
	{
	public:
		explicit VirtualBoard(const Board *board = nullptr);
		explicit VirtualBoard(const VirtualBoard *board = nullptr);
		void setTiles(const Board *);
		double SetOccupations(int row, int col, int maxDepth) const;
		double getOccupationScore(int row, int col);
		inline const std::array<std::array<VirtualTile, TILE_SIZE>, TILE_SIZE> &GetTilesV()const { return m_tiles; };
		inline void SetTileStatus(int row, int col, TileStatus status) { m_tiles[row][col].SetStatus(status); };
		inline const VirtualTile *Player1()const { return m_player1; };
		inline const VirtualTile *Player2()const { return m_player2; };
		void Player1Move(int row, int col);
		void Player2Move(int row, int col);
		void Player1Block(int row, int col);
		void Player2Block(int row, int col);

		inline TileStatus GetStatus(int r, int c) const { return m_tiles.at(r).at(c).GetStatus(); };
		double EvaluatePlayer(TileStatus) const;
		double EvaluatePlayer1() const;
		double EvaluatePlayer2() const;

	private:
		void resetVirtualTiles();
		void setVirtualOccupy(VirtualTile *);
		void setOccupations(VirtualTile *, int depth = 0, int maxDepth = MAX_OCC_DEPTH);
		double getOccupationScore(VirtualTile *, int maxDepth);
		int getOccupationCount();
		double evaluatePlayer(TileStatus p) const;

	private:
		std::array<std::array<VirtualTile, TILE_SIZE>, TILE_SIZE> m_tiles{};
		MoveTurn m_turn{ MoveTurn::Player1Move };
		VirtualTile *m_player1 = nullptr;
		VirtualTile *m_player2 = nullptr;
	};
}


#endif