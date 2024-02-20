#include "warehouse_xml_generator.hpp"
#include "filegenerator.hpp"
#include "userOptions.hpp"
#include "readCommandLine.hpp"
#include <iostream>

int main(int argc, char** argv) {
    // Create an instance of userOptions
    userOptions options;

    // Parse command line arguments
    readCommandLine(argc, argv, options);

    // Extract configuration parameters from options
    int rowSize = options.getRowSize();
    int colSize = options.getColSize();
    int numType1 = options.getNumBarriers();
    int numType2 = options.getNumOilSpills();
    int numType3 = options.getNumChargingStations();
    int numUnsatFiles = options.getNumUnsatModels();
    int numSatFiles = options.getNumSatModels();
    int depth = options.getDepth(); // Get depth
    int startingCharge = options.getStartingCharge(); // Get starting charge

    // Create an instance of UnsatFileGenerator with the desired configuration
    UnsatFileGenerator fileGenerator(numUnsatFiles, numSatFiles, rowSize, colSize, numType1, numType2, numType3, depth, startingCharge);

    // Generate the files
    fileGenerator.generateFiles();

    std::cout << "Requested UNSAT XML and CFG files have been generated successfully." << std::endl;

    return 0;
}

