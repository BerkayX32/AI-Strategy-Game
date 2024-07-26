
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include "tilewidget.h"

namespace minimaxgame
{
	TileWidget::TileWidget(int row, int col, QWidget *parent)
		: QWidget{ parent }
	{
		m_row = row;
		m_col = col;
	}

	void TileWidget::paintEvent(QPaintEvent *e)
	{
		QPainter painter{ this };
		QPen pen{ Qt::black };

		pen.setWidth(1);
		painter.setPen(pen);

		painter.drawRect(this->rect());
		//painter.fillRect(this->rect(), Qt::green);

		switch (m_status)
		{
		case minimaxgame::TileStatus::Player1:
			painter.fillRect(this->rect().topLeft().x() + 1, this->rect().topLeft().y() + 1,
				this->rect().width() - 1, this->rect().height() - 1, Qt::blue);
			break;
		case minimaxgame::TileStatus::Player2:
			painter.fillRect(this->rect().topLeft().x() + 1, this->rect().topLeft().y() + 1,
				this->rect().width() - 1, this->rect().height() - 1, Qt::red);
			break;
		case minimaxgame::TileStatus::Blocked:
			painter.fillRect(this->rect().topLeft().x() + 1, this->rect().topLeft().y() + 1,
				this->rect().width() - 1, this->rect().height() - 1, Qt::gray);
			break;
		case minimaxgame::TileStatus::Idle:
			break;
		default:
			break;
		}

	}

	void TileWidget::mouseReleaseEvent(QMouseEvent *e)
	{
		if (e->button() == Qt::LeftButton)
			emit TileClicked(this);
	}


	bool IsValidMove(TileWidget *player, TileWidget *tile)
	{
		if (!tile->IsIdle())
			return false;

		auto coldiff = abs(player->GetCol() - tile->GetCol());
		auto rowdiff = abs(player->GetRow() - tile->GetRow());

		if (coldiff == 0 && rowdiff == 0)
			return false;

		if (coldiff < 2 &&
			rowdiff < 2)
			return true;
		return false;
	}
}