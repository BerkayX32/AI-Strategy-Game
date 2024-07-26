#include "graphicsscene.h"
#include <QGraphicsSceneMouseEvent>

namespace minimaxgame
{
	GraphicsScene::GraphicsScene(QObject *parent) : QGraphicsScene(parent) {}

	void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
	{
		if (mouseEvent->buttons().testFlag(Qt::MouseButton::LeftButton))
		{
			emit MouseMoved(mouseEvent->scenePos());
		}
		QGraphicsScene::mouseMoveEvent(mouseEvent);
	}

	void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
	{
		if (mouseEvent->buttons().testFlag(Qt::MouseButton::LeftButton))
		{
			emit MousePressed(mouseEvent->scenePos());
		}
		QGraphicsScene::mousePressEvent(mouseEvent);
	}

	void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
	{
		emit MouseReleased(mouseEvent->scenePos());

		QGraphicsScene::mouseReleaseEvent(mouseEvent);
	}
}