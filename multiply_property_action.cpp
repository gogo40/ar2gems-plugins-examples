#include "multiply_property_action.h"

#include <utils/string_manipulation.h>
#include <grid/utilities.h>

Named_interface* Multiply_property::create_new_interface( std::string& ){
  return new Multiply_property;
}


// format gridName::NewNameSum::prop1::prop2[::prop3::propN]
bool Multiply_property::init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors )
{

    std::vector< std::string > params = 
    String_Op::decompose_string( parameters, Actions::separator,
                      				   Actions::unique );

  if( params.size() < 4 ) {
    errors->report( "Parameters are missing" );  
    return false;
  }

	std::string grid_name = params[0];
  if(grid_name.empty()) {
    errors->report( "No grid selected" );
    return false;
  }

  grid_ = get_grid_from_manager(grid_name);
  if(grid_ == 0) {
    errors->report( "The grid "+grid_name+" does not exist" );
    return false;
  }

	std::string product_prop_name = params[1];
  if(product_prop_name.empty()) errors->report( "No new property name was defined" );

  std::vector<Grid_continuous_property*> props;

  for(int i=2; i< params.size(); ++i) {
    Grid_continuous_property* prop = grid_->property(params[i]);
    if(prop == 0) {
      errors->report( "The property "+params[i]+" does not exist" );
      return false;
    }

    props_.push_back(prop);
  }

  product_prop_ = grid_->add_property(product_prop_name);
  if(product_prop_ == 0) {
    errors->report( "Could not create the property "+product_prop_name );
    return false;
  }

  return true;
}


bool Multiply_property::exec(){

  for(int i=0; i<props_[0]->size(); ++i) {
    float prod = 1.0;
    bool ok = true;
    for(int p=0; p>props_.size(); ++p) {
      
      if( !props_[p]->is_informed(i) ) {
        ok = false;
        break;
      }
      prod *= props_[p]->get_value(i);
    }

    if(ok) product_prop_->set_value(prod,i);
  }

  return true;
}