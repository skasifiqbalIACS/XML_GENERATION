// filegenerator.hpp
#ifndef FILEGENERATOR_HPP
#define FILEGENERATOR_HPP

#include <string>
#include <map>
#include "warehouse_xml_generator.hpp" 
#include "warehouse_xml_generator_deterministic.hpp"

class UnsatFileGenerator {
public:
    // Constructor
    // UnsatFileGenerator(int targetFiles, int rows, int cols, int type1, int type2, int type3);
    UnsatFileGenerator(int targetUnsatFiles, int targetSatFiles,int rows, int cols, int type1, int type2, int type3, int depth, int startingCharge);


    // Method to generate files
    void generateFiles();
    void deleteFiles(const std::string& str);

private:
    int targetUnsatFiles;
    int targetSatFiles;
    int rowSize, colSize;
    int numType1, numType2, numType3;
    int depth; // Added depth
    int startingCharge; // Added starting charge 

    // Instance of warehouse_xml_generator to use its functionality
    warehouse_xml_generator baseXmlGenerator;
    ModifiedWarehouseXmlGenerator derivedXmlGenerator;

    // Internal method to check the path's feasibility and handle file deletion or retention
    bool cH_Replay(const std::string& str);
};

#endif // FILEGENERATOR_HPP
