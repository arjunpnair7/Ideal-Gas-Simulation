//
// Created by Arjun Nair on 3/21/22.
//

#ifndef IDEAL_GAS_HISTOGRAM_H
#define IDEAL_GAS_HISTOGRAM_H
#include "cinder/gl/gl.h"

class histogram {
public:
    histogram(glm::vec2 topLeft, std::vector<particle> frequencyData, std::string graphTitle);
    histogram(std::vector<particle> gravity_data, std::string graphTitle, glm::vec2 topLeft);
    histogram(std::string graphTitle, std::vector<particle> pressure_data, glm::vec2 topLeft);
    histogram();
    std::vector<double> calculateSpeed(std::vector<particle> data);
    void DrawSpeedHistogram() const;
    void DrawGravitationalEnergyHistogram() const;
    void DrawPressureHistogram() const;

private:
    std::vector<double> frequencyData;
    std::vector<particle> containers_particle_data;
    glm::vec2 topLeft;
    std::string graphTitle;

};
#endif //IDEAL_GAS_HISTOGRAM_H