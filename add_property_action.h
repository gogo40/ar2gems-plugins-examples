// DEMO

#ifndef ADD_PROPERTY_ACTIONS_H_
#define ADD_PROPERTY_ACTIONS_H_

#include "common.h"

#include <appli/action.h> 
#include <grid/grid_property.h>
#include <grid/geostat_grid.h>
#include <utils/named_interface.h>


class PLUGIN_DEMOS_DECL Add_property :  public Action {
 
public: 

  static Named_interface* create_new_interface( std::string& );

  virtual ~Add_property() {} 
  virtual bool init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors = 0, Progress_notifier* notifier = 0 ); 
  virtual bool exec(Progress_notifier* notifier = 0); 

private :
  Geostat_grid* grid_;
  std::vector<Grid_continuous_property*> props_;
  Grid_continuous_property* sum_prop_;

};



#endif