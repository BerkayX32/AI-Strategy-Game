#ifndef MINIMAXGAME_VIRTUALTILE_H
#define MINIMAXGAME_VIRTUALTILE_H


#include "minimaxgame.h"
#include <qdebug.h>
namespace minimaxgame
{

	class VirtualTile
	{

	public:
		VirtualTile() = default;

	public:
		inline void SetRowCol(int r, int c) { m_row = r; m_col = c; };
		inline void SetStatus(TileStatus status) { m_status = status; };

		inline TileStatus GetStatus()const { return m_status; };
		inline int GetRow()const { return m_row; };
		inline int GetCol()const { return m_col; };
		inline bool IsIdle()const 
		{ 
			//qDebug() << (m_status == TileStatus::Idle);
			return m_status == TileStatus::Idle; 
		};
		inline bool IsVirtuallyBlocked()const
		{
			return
				m_status == TileStatus::VirtualOccupy
				|| m_status == TileStatus::Blocked
				|| m_status == TileStatus::VirtualNoScoreOccupy;
		};

	private:
		int m_row{ -1 };
		int m_col{ -1 };
		TileStatus m_status{ TileStatus::Idle };

	};

}


#endif