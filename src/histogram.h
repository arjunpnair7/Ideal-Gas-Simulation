//
// Created by Arjun Nair on 3/21/22.
//

#ifndef IDEAL_GAS_HISTOGRAM_H
#define IDEAL_GAS_HISTOGRAM_H
#include "cinder/gl/gl.h"

class histogram {
public:
    histogram(glm::vec2 topLeft, std::vector<particle> frequencyData, std::string graphTitle);
    histogram();
    std::vector<double> calculateSpeed(std::vector<particle> data);
    void drawHistogram() const;
private:
    std::vector<double> frequencyData;
    glm::vec2 topLeft;
    std::string graphTitle;
};
#endif //IDEAL_GAS_HISTOGRAM_H