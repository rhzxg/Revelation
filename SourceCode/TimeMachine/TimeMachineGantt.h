#pragma once

#include <QWidget>
#include "ui_TimeMachineGantt.h"

class TimeMachineGantt : public QWidget
{
	Q_OBJECT

public:
	TimeMachineGantt(QWidget* parent = nullptr);
	~TimeMachineGantt();

private:
	Ui::TimeMachineGanttClass ui;
};
