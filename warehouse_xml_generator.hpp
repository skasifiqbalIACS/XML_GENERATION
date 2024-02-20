// warehouse_xml_generation.h

#ifndef WAREHOUSE_XML_GENERATION_H
#define WAREHOUSE_XML_GENERATION_H

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
    void GenerateXml(int rowSize, int colSize, const std::map<int, SpecialLocationType>& specialLocations, const std::string& xmlFilename);
    void generateCfgFile(const std::map<int, SpecialLocationType>& specialLocations, const std::string& filename, int rowSize, int colSize, int startingCharge);
    std::map<int, SpecialLocationType> GenerateSpecialLocations(int rowSize, int colSize, int numType1, int numType2, int numType3);
    void GenerateFilesForModel(const std::string& baseName, int rowSize, int colSize, int numType1, int numType2, int numType3, int depth, int startingCharge);


private:
    int rowSize, colSize;
    int numType1, numType2, numType3;
    int depth, startingCharge;
    std::map<int, SpecialLocationType> specialLocations;

    void AddParameters(tinyxml2::XMLNode* component, int numVariables, int numTransitions);
    void AddLocation(tinyxml2::XMLNode* component, int id, int x1Min, int x1Max, int x2Min, int x2Max, SpecialLocationType specialType = SpecialLocationType::None);
    void AddTransition(tinyxml2::XMLNode* component, int& transitionLabelCounter, int source, int target, int rowSize, int colSize, const std::map<int, SpecialLocationType>& specialLocations);
};

#endif // WAREHOUSE_XML_GENERATION_H
