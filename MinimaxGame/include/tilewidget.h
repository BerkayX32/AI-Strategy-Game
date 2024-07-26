#ifndef MINIMAXGAME_TILEWIDGET_H
#define MINIMAXGAME_TILEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPainter>

#include "minimaxgame.h"

namespace minimaxgame
{
	class TileWidget : public QWidget
	{
		Q_OBJECT
	public:
		explicit TileWidget(int row = -1, int col = -1, QWidget *parent = nullptr);
		~TileWidget() = default;

	protected:
		virtual void paintEvent(QPaintEvent *)override;
		virtual void mouseReleaseEvent(QMouseEvent *)override;
	signals:
		void TileClicked(TileWidget *);

	public:
		void SetRowCol(int r, int c) { m_row = r; m_col = c; };
		void SetStatus(TileStatus status) { m_status = status; };

		TileStatus GetStatus()const { return m_status; };
		int GetRow()const { return m_row; };
		int GetCol()const { return m_col; };
		bool IsIdle()const { return m_status == TileStatus::Idle; };
	private:
		int m_row{ -1 };
		int m_col{ -1 };
		TileStatus m_status{ TileStatus::Idle };
	};

	bool IsValidMove(TileWidget *player, TileWidget *tile);
}


#endif