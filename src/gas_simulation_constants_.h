//
// Created by Arjun Nair on 3/21/22.
//

#ifndef IDEAL_GAS_GAS_SIMULATION_CONSTANTS__H
#define IDEAL_GAS_GAS_SIMULATION_CONSTANTS__H

#include <iostream>

using std::string;
/**
 * A header file that contains constants to manipulate the simulation as well as the test cases
 */

namespace SimulationConstants {
    //Constants to manipulate the creation of the container
    const int number_of_particles_in_simulation = 50;
    const int container_start_x_position = 100;
    const int container_start_y_position = 100;
    const int container_horizontal_dimension = 600;
    const int container_vertical_dimension = 600;
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
    const int max_start_position = 500;
    const int min_velocity = 0;
    const int max_velocity = 2;
    const int min_radius = 15;
    const int max_radius = 15;
    const float particle_type_1_mass = 5;
    const float particle_type_2_mass = 20;
    const float particle_type_3_mass = 40;

    //Constants to use for test cases
    const int number_of_particles_in_test = 10;
    const float tolerance = .001;

    //Constants to control the drawing of the histogram
    const int bar_width = 25;
    const int label_y_offset = 15;
    const double x_axis_cutoff_1 = 1.0;
    const double x_axis_cutoff_2 = 2.0;
    const double x_axis_cutoff_3 = 3.0;
    const int x_label_padding = 7;
    const int histogram_height = 150;
    const int histogram_width = 250;
    const int histogram_y_label_midpoint = histogram_height/2;
    const int histogram_y_labels_padding = 20;



}


#endif //IDEAL_GAS_GAS_SIMULATION_CONSTANTS__H
