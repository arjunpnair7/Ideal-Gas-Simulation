
#include <iostream>
using std::string;
/**
 * A class that contains constants to manipulate the simulation
 */

namespace SimulationConstants {

    //Constants to manipulate the creation of the container
    const int number_of_particles_in_simulation = 50;
    const int container_start_x_position = 100;
    const int container_start_y_position = 100;
    const int container_horizontal_dimension = 1000;
    const int container_vertical_dimension = 650;
    const char *const container_color = "purple";
    const char *const particle_color_1 = "red";
    const char *const particle_color_2 = "blue";
    const char *const particle_color_3 = "green";
    const char *const particle_color_4 = "purple";
    const char *const particle_color_5 = "white";

    //Constants to modify the attribute of the particles
    const int min_start_x_position = 100;
    const int min_start_y_position = 100;
    const int min_start_position = 0;
    const int max_start_position = 650;
    const int min_velocity = 1;
    const int max_velocity = 2;
    const int min_radius = 15;
    const int max_radius = 15;

    //Constants to use for test cases
    const int number_of_particles_in_test = 10;
    const float tolerance = .001;

}
