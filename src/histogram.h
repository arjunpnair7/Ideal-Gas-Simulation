//
// Created by Arjun Nair on 3/21/22.
//

#ifndef IDEAL_GAS_HISTOGRAM_H
#define IDEAL_GAS_HISTOGRAM_H
#include "cinder/gl/gl.h"

class histogram {
public:
    histogram(glm::vec2 topLeft, std::vector<particle> frequencyData);
    std::vector<double> calculateSpeed(std::vector<particle> data);
private:
    std::vector<double> frequencyData;
};
#endif //IDEAL_GAS_HISTOGRAM_H