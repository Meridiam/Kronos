#pragma once

#include <QWidget>
#include "Stage.h"

class StageController : public QWidget
{
	Q_OBJECT

public:
	StageController(QWidget *parent);
	~StageController();
};
