#include "ccissWidget.h"

ccissWidget::ccissWidget(QWidget *parent) : QWidget(parent)
{
    this->resize(800,600);
    this->constructIHM();
}

ccissWidget::~ccissWidget()
{

}

//!---------------------------------------------------------------------------------
//!
//! \brief ccissWidget::constructIHM
//!
void ccissWidget::constructIHM(){

    this->titleBarWidget = new QWidget();
    this->titleBarLayout = new QHBoxLayout(titleBarWidget);

    this->toolBarWidget = new QWidget();
    this->toolBarLayout = new QHBoxLayout(toolBarWidget);

    this->operationWindowWidget = new QWidget();
    this->operationWindowLayout = new QHBoxLayout(operationWindowWidget);


    this->stateBarWidget = new QWidget();
    this->stateBarLayout = new QHBoxLayout(stateBarWidget);

    this->ccissMainWindowLayout = new QVBoxLayout(this);
    this->ccissMainWindowLayout->addWidget(titleBarWidget);
    this->ccissMainWindowLayout->addWidget(toolBarWidget);
    this->ccissMainWindowLayout->addWidget(operationWindowWidget);
    this->ccissMainWindowLayout->addWidget(stateBarWidget);
}
