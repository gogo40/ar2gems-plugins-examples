#include "common.h"
#include "add_property_action.h"
#include "multiply_property_action.h"
#include "local_high_values_algo.h"
#include "pie_chart_creator.h"
#include <GsTL/utils/smartptr.h>
#include <utils/gstl_messages.h>
#include <utils/manager_repository.h>

//extern "C" __declspec(dllexport) int plugin_init() {
extern "C" PLUGIN_DEMOS_DECL int plugin_init() {

  GsTLlog << "\n\n registering actions add_properties" << "\n";
  SmartPtr<Named_interface> ni = 
    Root::instance()->interface( actions_manager );
  Manager* dir = dynamic_cast<Manager*>( ni.raw_ptr() );
  if( !dir ) {
    GsTLlog << "Directory " << actions_manager << " does not exist \n";
    return 1;
  }
  
  dir->factory( "AddProperties", Add_property::create_new_interface );
  dir->factory( "MultiplyProperties", Multiply_property::create_new_interface );

  // Register algo
  ni = Root::instance()->interface( geostatAlgo_manager );
  dir = dynamic_cast<Manager*>( ni.raw_ptr() );
  if( !dir ) {
    GsTLlog << "Directory " << geostatAlgo_manager << " does not exist \n";
    return 1;
  }
  dir->factory( Local_high_values().name(), Local_high_values::create_new_interface );


  // Add the new chart
  ni = 	Root::instance()->interface( eda_charts_manager );
  dir = dynamic_cast<Manager*>( ni.raw_ptr() );
  if( !dir ) {
	  GsTLlog << "Directory " << eda_charts_manager << " does not exist \n";
	  return 1;
  }
  dir->factory( Pie_chart_creator_factory().name(), 
                Pie_chart_creator_factory::create_new_interface);

  return 0;
}
