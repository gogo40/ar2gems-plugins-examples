#include "pie_chart.h"

#include <utils/manager.h>
#include <utils/manager_repository.h>
#include <utils/error_messages_handler.h>
#include <appli/project.h>

#include <QPixmap>
#include <QIcon>

#include <QSplitter>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QTableView>
#include <QSortFilterProxyModel>

#include <vtkAxis.h>
#include <vtkPlotLine.h>
#include <vtkPlotPie.h>
#include <vtkPlotBar.h>
#include <vtkIntArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkColorSeries.h>
#include <vtkTextProperty.h>

Categorical_property_pie_chart::Categorical_property_pie_chart(Geostat_grid* grid,
                                    Grid_categorical_property* cat_prop,
                                    Grid_continuous_property* weight_prop,
                                    Grid_filter* filter, QWidget *parent ) :
Chart_base(parent), grid_(grid), cat_prop_(cat_prop), weight_prop_(weight_prop), filter_(filter)
{


  QString title = QString("Pie chart for %1").arg(cat_prop_->name().c_str());

  this->setWindowTitle(title);
  this->setWindowIcon(QPixmap(":/icons/appli/Pixmaps/ar2gems-icon-256x256.png"));

  QSplitter* main_splitter = new QSplitter(Qt::Horizontal, this);

  QTabWidget* chart_tab = new QTabWidget(main_splitter);

  QSplitter* chart_splitter = new QSplitter(Qt::Vertical, chart_tab);

  chart_widget_ = new Chart_widget(chart_splitter);
  chart_control_ = new Chart_display_control( chart_splitter );
  chart_widget_->set_controler(chart_control_);

  chart_splitter->addWidget( chart_widget_ );
  chart_splitter->addWidget( chart_control_ );

  QVTKWidget* pie_widget = new QVTKWidget(chart_tab);
	vtkSmartPointer<vtkContextView> pie_context_view = vtkSmartPointer<vtkContextView>::New();
	pie_context_view->SetInteractor(pie_widget->GetInteractor());
	pie_widget->SetRenderWindow(pie_context_view->GetRenderWindow());
	pie_chart_ = vtkSmartPointer<vtkChartPie>::New();
	pie_context_view->GetScene()->AddItem(pie_chart_);  
  
  chart_tab->addTab(pie_widget, "Pie" );
  chart_tab->addTab(chart_splitter, "Bars" );

  data_view_ = vtkSmartPointer<vtkQtTableView>::New();
  data_view_->GetWidget()->setParent(main_splitter);

  main_splitter->addWidget( chart_tab );
  main_splitter->addWidget( data_view_->GetWidget() );

  main_splitter->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));

  QFrame* report_frame = new QFrame(this);
  QPushButton* view_report_button = new QPushButton("View Report",report_frame);
  QPushButton* save_report_button = new QPushButton("Save Report",report_frame);
  QPushButton* save_figure_button = new QPushButton("Save Figure",report_frame);
  QHBoxLayout* report_layout = new QHBoxLayout(report_frame);
  report_layout->addStretch();
  report_layout->addWidget(save_figure_button);
  report_layout->addWidget(save_report_button);
  report_layout->addWidget(view_report_button);
  report_frame->setLayout(report_layout);


  report_frame->setSizePolicy(QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum));

  QVBoxLayout* main_layout = new QVBoxLayout(this);
  
  main_layout->addWidget(main_splitter);
  main_layout->addWidget(report_frame);
  
  this->setLayout(main_layout);


  this->build_value_table();
  this->build_plot();
  data_view_->SetRepresentationFromInput(table_);
  data_view_->Update();

  QTableView* table = dynamic_cast<QTableView*>(data_view_->GetWidget());
  bool ok = connect(table, SIGNAL(clicked(const QModelIndex &)), this, SLOT(cutoff_selected(const QModelIndex &)) );
  ok = connect( save_figure_button, SIGNAL(clicked()), chart_widget_, SLOT(save_figure()) );

}


void Categorical_property_pie_chart::build_value_table(){

  CategoricalPropertyDefinition* cdef = cat_prop_->get_category_definition();
  std::vector<double> proportions(cat_prop_->get_number_of_category(), 0.0);

  float sum_weight = 0.0;
  for(int i=0; i<cat_prop_->size(); ++i) {

    if( !cat_prop_->is_informed(i) ) continue;
    if(filter_ && !filter_->is_valid_nodeid(i)) continue;
    float weight = weight_prop_?weight_prop_->get_value(i):1;
    sum_weight += weight;

    int cat_id = cat_prop_->get_value(i);
    int cat_index = cdef->index_from_category_id( cat_id );

    proportions[cat_index]+=weight;

  }


  vtkSmartPointer<vtkFloatArray> proportion_array = vtkSmartPointer<vtkFloatArray>::New();
  proportion_array->SetName("Proportion");
  proportion_array->SetNumberOfValues(cat_prop_->get_number_of_category());

  vtkSmartPointer<vtkDoubleArray> index_array = vtkSmartPointer<vtkDoubleArray>::New();
  index_array->SetName("Category");
  index_array->SetNumberOfValues(cat_prop_->get_number_of_category());

  vtkSmartPointer<vtkStringArray> cat_name_array = vtkSmartPointer<vtkStringArray>::New();
  cat_name_array->SetName("Category Name");
  cat_name_array->SetNumberOfValues(cat_prop_->get_number_of_category());

  for(int i=0; i<proportion_array->GetNumberOfTuples(); ++i) {
    proportion_array->SetValue(i,proportions[i]/sum_weight);
    index_array->SetValue(i,i);
    cat_name_array->SetValue(i,cdef->get_category_name_from_index(i));
  }

  table_ = vtkSmartPointer<vtkTable>::New();
  table_->AddColumn(index_array);
  table_->AddColumn(cat_name_array);
  table_->AddColumn(proportion_array);
  

}


void Categorical_property_pie_chart::build_plot()
{

  vtkPlotBar* plot_bar = vtkPlotBar::SafeDownCast(chart_widget_->chart()->AddPlot(vtkChart::BAR ));
  plot_bar->SetInputData(table_,0,2);
  plot_bar->SetColor(0.2,0.2,0.8);

  chart_widget_->chart()->GetAxis(vtkAxis::BOTTOM)->SetMinimumLimit( -0.5 );
  chart_widget_->chart()->GetAxis(vtkAxis::BOTTOM)->SetMaximumLimit( table_->GetNumberOfRows() + 0.5 );
  //chart_widget_->chart()->GetAxis(vtkAxis::BOTTOM)->SetBehavior(1);
  chart_widget_->chart()->GetAxis(vtkAxis::BOTTOM)->SetTitle("Category");

  chart_widget_->chart()->GetAxis(vtkAxis::BOTTOM)->SetCustomTickPositions(vtkDoubleArray::SafeDownCast(table_->GetColumn(0)),
                                                                           vtkStringArray::SafeDownCast( table_->GetColumn(1) ));
  chart_widget_->chart()->GetAxis(vtkAxis::BOTTOM)->GetLabelProperties()->SetOrientation(90);
  chart_widget_->chart()->GetAxis(vtkAxis::BOTTOM)->GetLabelProperties()->SetVerticalJustification(VTK_TEXT_CENTERED);
  chart_widget_->chart()->GetAxis(vtkAxis::BOTTOM)->GetLabelProperties()->SetJustification(VTK_TEXT_RIGHT);

  chart_widget_->chart()->GetAxis(vtkAxis::LEFT)->SetMinimumLimit( 0 );
  chart_widget_->chart()->GetAxis(vtkAxis::LEFT)->SetTitle(table_->GetColumnName(0));

  chart_widget_->chart()->GetAxis(vtkAxis::RIGHT)->SetTitle(table_->GetColumnName(1));
  chart_widget_->chart()->GetAxis(vtkAxis::RIGHT)->SetMaximumLimit( 1 );



  
  vtkPlotPie* plot_pie = vtkPlotPie::SafeDownCast(pie_chart_->AddPlot(0));
  plot_pie->SetInputData(table_);
  plot_pie->SetInputArray(0,"Proportion");
  plot_pie->SetLabels( vtkStringArray::SafeDownCast( table_->GetColumn(1) ) );
  pie_chart_->SetShowLegend(true);
  pie_chart_->SetTitle(cat_prop_->name().c_str());

   vtkSmartPointer<vtkColorSeries> color_series = vtkSmartPointer<vtkColorSeries>::New();
   color_series->ClearColors();
   int ncat = cat_prop_->get_number_of_category();
   const CategoricalPropertyDefinition* cdef = cat_prop_->get_category_definition();
   for(int i=0; i<ncat; ++i) {
     QColor color = cdef->color_from_index(i);
     vtkColor3ub vtk_color( color.red(), color.green(), color.blue() );
     color_series->AddColor(vtk_color);
   }

   plot_pie->SetColorSeries(color_series);

}
