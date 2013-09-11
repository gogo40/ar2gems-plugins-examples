#ifndef __HIGH_VALUES_H__
#define __HIGH_VALUES_H__


#include "common.h"
#include <geostat/geostat_algo.h>
#include <grid/geostat_grid.h>
#include <grid/grid_property.h>
#include <grid/grid_region.h>
#include <grid/neighborhood.h>
#include <utils/error_messages_handler.h>
#include <appli/project.h>
#include <geostat/parameters_handler.h>

class Local_high_values : public Geostat_algo {
public:
	Local_high_values();
	virtual ~Local_high_values();

	  virtual bool initialize( const Parameters_handler* parameters,
				   Error_messages_handler* errors );
	  virtual int execute( GsTL_project* proj=0 );
	  virtual std::string name() const { return "local_high_values"; }

	 public:
	  static Named_interface* create_new_interface( std::string& );

private :
    
  float threshold_;
  Grid_continuous_property* prop_;
  Geostat_grid* grid_;
  Grid_region* region_;

  std::string out_name_;

  Neighborhood* neigh_;

};

#endif /* GEOBODY_H_ */
