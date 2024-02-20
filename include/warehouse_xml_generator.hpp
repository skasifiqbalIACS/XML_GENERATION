// warehouse_xml_generator.h

#ifndef WAREHOUSE_SIMULATION_HPP
#define WAREHOUSE_SIMULATION_HPP

#include "tinyxml2.h"
#include <map>
#include <string>

namespace tinyxml2 {
    class XMLNode;
}

enum class SpecialLocationType {
    None,  // Normal location
    Type1, // Barrier special location type
    Type2, // Oil Spill special location type
    Type3, // Charging special location type
    Type4  // Fourth special location type
};

class warehouse_xml_generator {
public:
    //WarehouseSimulation(int rowSize, int colSize, int numType1, int numType2, int numType3);
    void generateCfgFile(const std::map<int, SpecialLocationType>& specialLocations, const std::string& filename, int rowSize, int colSize, int startingCharge);
    // void run(int rowSize, int colSize, int numType1, int numType2, int numType3);
    std::map<int, SpecialLocationType> GenerateSpecialLocations(int rowSize, int colSize, int numType1, int numType2, int numType3);
    void GenerateFilesForModel(const std::string& baseName, int rowSize, int colSize, int numType1, int numType2, int numType3, int depth, int charge);
    void GenerateXml(int rowSize, int colSize, const std::map<int, SpecialLocationType>& specialLocations, const std::string& xmlFilename);


private:
    int rowSize, colSize;
    int numType1, numType2, numType3;
    int depth;
    int charge;
    std::map<int, SpecialLocationType> specialLocations;

    void AddParameters(tinyxml2::XMLNode* component, int numVariables, int numTransitions);
    void AddLocation(tinyxml2::XMLNode* component, int id, int x1Min, int x1Max, int x2Min, int x2Max, SpecialLocationType specialType = SpecialLocationType::None);
    void AddTransition(tinyxml2::XMLNode* component, int& transitionLabelCounter, int source, int target, int rowSize, int colSize, const std::map<int, SpecialLocationType>& specialLocations);
};

#endif // WAREHOUSE_SIMULATION_HPP
