#ifndef MINIMAXGAME_GRAPHICSSCENE_H
#define MINIMAXGAME_GRAPHICSSCENE_H

#include <QGraphicsScene>

namespace minimaxgame
{
	class GraphicsScene : public QGraphicsScene
	{
		Q_OBJECT
	public:
		explicit GraphicsScene(QObject *parent = nullptr);
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)override;
		virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)override;
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)override;
	signals:
		void MouseMoved(QPointF);
	signals:
		void MousePressed(QPointF);
	signals:
		void MouseReleased(QPointF);
	};
}
#endif
