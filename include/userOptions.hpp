#ifndef USEROPTIONS_HPP
#define USEROPTIONS_HPP

#include <cstring>
#include <iostream>

/**
 * @brief Class to store and manage user options for XML generation.
*/
class userOptions {
public:
    userOptions(); // Constructor
    ~userOptions(); // Destructor

    unsigned int getRowSize() const;
    unsigned int getColSize() const;
    unsigned int getNumBarriers() const;
    unsigned int getNumOilSpills() const;
    unsigned int getNumChargingStations() const;
    unsigned int getStartingCharge() const;
    unsigned int getNumUnsatModels() const;
    unsigned int getNumSatModels() const;
    unsigned int getDepth() const;

    void setRowSize(unsigned int rs);
    void setColSize(unsigned int cs);
    void setNumBarriers(unsigned int nb);
    void setNumOilSpills(unsigned int nos);
    void setNumChargingStations(unsigned int ncs);
    void setStartingCharge(unsigned int charge);
    void setNumUnsatModels(unsigned int num);
    void setNumSatModels(unsigned int num);
    void setDepth(unsigned int depth);

private:
    unsigned int rowSize;
    unsigned int colSize;
    unsigned int numBarriers;
    unsigned int numOilSpills;
    unsigned int numChargingStations;
    unsigned int numUnsatModels;
    unsigned int numSatModels;
    unsigned int depth;
    unsigned int startingCharge;

};
#endif // USEROPTIONS_HPP
