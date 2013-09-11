#ifndef CAT_PIE_CHART_H
#define CAT_PIE_CHART_H

#include "common.h"

#include <charts/chart_base.h>
#include <charts/chart_widget.h>
#include <charts/chart_display_control.h>
#include <grid/grid_property.h>
#include <grid/grid_weight_property.h>
#include <grid/grid_filter.h>

#include <vtkQtTableView.h>
#include <vtkSmartPointer.h>
#include <vtkLookupTable.h>
#include <vtkChartPie.h>

#include <QModelIndex>
#include <QLabel>

#include <vector>

class PLUGIN_DEMOS_DECL Categorical_property_pie_chart : public Chart_base
{
    Q_OBJECT
public:
    explicit Categorical_property_pie_chart(Geostat_grid* grid,
                                    Grid_categorical_property* cprop,
                                    Grid_continuous_property* weight_prop,
                                    Grid_filter* filter, QWidget *parent = 0);
    
signals:
    
public slots:

 private :
  void build_value_table();
  void build_plot();


private:


  Chart_widget* chart_widget_;
  Chart_display_control* chart_control_;
  vtkSmartPointer<vtkChartPie> pie_chart_;

  Geostat_grid* grid_;
  Grid_categorical_property* cat_prop_;
  Grid_continuous_property* weight_prop_;
  Grid_filter* filter_;

  vtkSmartPointer<vtkTable> table_;
  vtkSmartPointer<vtkQtTableView> data_view_;

   
};

#endif // 
