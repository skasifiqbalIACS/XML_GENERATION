
#include "userOptions.hpp"

userOptions::userOptions() {
    // Initialization with default values
    rowSize = 5; // Default row size
    colSize = 4; // Default column size
    numBarriers = 2; // Default number of barriers
    numOilSpills = 2; // Default number of oil spill locations
    numChargingStations = 1; // Default number of charging stations
    numUnsatModels = 1; // Default number of unsat model files required
    numSatModels = 0;
}

// Getters and setters for each option
unsigned int userOptions::getRowSize() const {
    return rowSize;
}

void userOptions::setRowSize(unsigned int rs) {
    rowSize = rs;
}

unsigned int userOptions::getColSize() const {
    return colSize;
}

void userOptions::setColSize(unsigned int cs) {
    colSize = cs;
}

unsigned int userOptions::getNumBarriers() const {
    return numBarriers;
}

void userOptions::setNumBarriers(unsigned int nb) {
    numBarriers = nb;
}

unsigned int userOptions::getNumOilSpills() const {
    return numOilSpills;
}

void userOptions::setNumOilSpills(unsigned int nos) {
    numOilSpills = nos;
}

unsigned int userOptions::getNumChargingStations() const {
    return numChargingStations;
}

void userOptions::setNumChargingStations(unsigned int ncs) {
    numChargingStations = ncs;
}

unsigned int userOptions::getStartingCharge() const {
    return startingCharge;
}
void userOptions::setStartingCharge(unsigned int charge){
    startingCharge = charge;
}

unsigned int userOptions::getNumUnsatModels() const {
    return numUnsatModels;
}

void userOptions::setNumUnsatModels(unsigned int num) {
    numUnsatModels = num;
}

unsigned int userOptions::getNumSatModels() const {
    return numSatModels;
}

void userOptions::setNumSatModels(unsigned int num) {
    numSatModels = num;
}

unsigned int userOptions::getDepth() const {
    return depth;
}

void userOptions::setDepth(unsigned int dpt) {
    depth = dpt;
}

userOptions::~userOptions() {
    // Destructor
}
