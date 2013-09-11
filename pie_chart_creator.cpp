/* -----------------------------------------------------------------------------
** Copyright© 2012 Advanced Resources and Risk Technology, LLC
** All rights reserved.
**
** This file is part of Advanced Resources and Risk Technology, LLC (AR2TECH) 
** version of the open source software sgems.  It is a derivative work by 
** AR2TECH (THE LICENSOR) based on the x-free license granted in the original 
** version of the software (see notice below) and now sublicensed such that it 
** cannot be distributed or modified without the explicit and written permission 
** of AR2TECH.
**
** Only AR2TECH can modify, alter or revoke the licensing terms for this 
** file/software.
**
** This file cannot be modified or distributed without the explicit and written 
** consent of AR2TECH.
**
** Contact Dr. Alex Boucher (aboucher@ar2tech.com) for any questions regarding
** the licensing of this file/software
**
** The open-source version of sgems can be downloaded at 
** sourceforge.net/projects/sgems.
** ----------------------------------------------------------------------------*/

#include "pie_chart_creator.h"
#include "pie_chart.h"

#include <utils/manager_repository.h>

#include <QDialog>
#include <QTreeView>
#include <QToolBox>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QModelIndexList>
#include <QGroupBox>
#include <QLabel>
#include <QDockWidget>
#include <QToolBar>
#include <QAction>
#include <QMdiSubWindow>
#include <QButtonGroup>
#include <QTextEdit>
#include <QCheckBox>

Pie_chart_creator::Pie_chart_creator(Chart_mdi_area* mdi_area,QWidget *parent):Chart_creator(mdi_area,parent){

  QVBoxLayout* main_layout = new QVBoxLayout(this);
  main_layout->setContentsMargins(0,0,0,0);

  grid_selector_ = new GridSelector(this);
  grid_selector_->setToolTip("Grid containing the data");
  cat_prop_selector_ = new SingleCategoricalPropertySelector(this);

  QCheckBox* weight_option = new QCheckBox("Use weight",this);
  weight_option->setChecked(false);

  cat_prop_selector_->setToolTip("Categorical property");
  weight_prop_selector_ = new SinglePropertySelector(this);
  weight_prop_selector_->setToolTip("Weights");
  weight_prop_selector_->setEnabled(false);

  main_layout->addWidget(grid_selector_);
  main_layout->addWidget(new QLabel("Categorical Property",this));
  main_layout->addWidget(cat_prop_selector_);

  main_layout->addWidget(weight_option);
  main_layout->addWidget(weight_prop_selector_);
  main_layout->addStretch();

 
  QPushButton* show_button = new QPushButton("Display",this);
  main_layout->addWidget(show_button);
  this->setLayout(main_layout);

  bool ok = connect( grid_selector_, SIGNAL(activated( const QString&)),weight_prop_selector_, SLOT(show_properties( const QString&))   );
  ok = connect( grid_selector_, SIGNAL(activated( const QString&)),cat_prop_selector_, SLOT(show_properties( const QString&))   );
  ok = connect( weight_option, SIGNAL(toggled( bool)),weight_prop_selector_, SLOT(setEnabled( bool))   );

  ok = connect( show_button, SIGNAL(clicked()),this, SLOT(show_chart ())   );

}

void Pie_chart_creator::show_chart(){

  

  QString grid_name = grid_selector_->selectedGrid();
  QString region_name = grid_selector_->selectedRegion();
  QString cat_prop_name = cat_prop_selector_->currentText();

  if(grid_name.isEmpty() || cat_prop_name.isEmpty() ) return;

  SmartPtr<Named_interface> grid_ni =
    Root::instance()->interface( gridModels_manager + "/" + grid_name.toStdString() );
  Geostat_grid* grid = dynamic_cast<Geostat_grid*>( grid_ni.raw_ptr() );
  
  if(grid == 0) return;


  Grid_categorical_property* cprop = grid->categorical_property(cat_prop_name.toStdString());
  Grid_region* region = grid->region(region_name.toStdString());
  
  if(cprop == 0 ) return;

  Grid_continuous_property* weight_prop = 0;
  if( weight_prop_selector_->isEnabled() ) {
    weight_prop = grid->property( weight_prop_selector_->currentText().toStdString()  );
  }

  Grid_filter* filter = 0;
  if(region) {
    filter = new Grid_filter_region(region );
  }

  Categorical_property_pie_chart* chart = new Categorical_property_pie_chart(grid,cprop,weight_prop,filter,this );
  QMdiSubWindow* sub_window = mdi_area_->addSubWindow(chart);
  sub_window->setAttribute( Qt::WA_DeleteOnClose );
  sub_window->show();

}

