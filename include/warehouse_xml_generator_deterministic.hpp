#ifndef MODIFIED_WAREHOUSE_XML_GENERATOR_HPP
#define MODIFIED_WAREHOUSE_XML_GENERATOR_HPP

#include "warehouse_xml_generator.hpp"
#include "tinyxml2.h"
#include <map>
#include <string>
#include <random>

// Since the enum SpecialLocationType is already defined in the base class header,
// it doesn't need to be redefined here.

class ModifiedWarehouseXmlGenerator : public warehouse_xml_generator {
public:
    // Constructor inheriting the constructor of warehouse_xml_generator or defining its own
    // using warehouse_xml_generator::warehouse_xml_generator;
    // ModifiedWarehouseXmlGenerator(int rowSize, int colSize, int numType1, int numType2, int numType3, int depth, int charge);

    
    // Overriding or adding new methods
    void GenerateXml(int rowSize, int colSize, const std::map<int, SpecialLocationType>& specialLocations, const std::string& xmlFilename) override;
    void generateCfgFile(const std::tuple<double, double, int, int>& initialSetup, const std::string& filename, int startingCharge) override;
    void GenerateFilesForModel(const std::map<int, SpecialLocationType>& specialLocations,
                                                    const std::string& baseName, int rowSize, int colSize, int depth, int startingCharge, 
                                                    const std::tuple<double, double, int, int> initialSetup) override;

    // virtual ~ModifiedWarehouseXmlGenerator();

protected:
    // Additional member variables or methods unique to the derived class
    int depth;
    int charge;

    // Example of an overridden method
    void AddLocation(tinyxml2::XMLNode* component, int id, int x1Min, int x1Max, int x2Min, int x2Max, SpecialLocationType specialType = SpecialLocationType::None) override;

    // Additional methods unique to the modified generator
    void AddParameters(tinyxml2::XMLNode* component, int numVariables, int numTransitions) override;

    // void AddTransition(tinyxml2::XMLNode* component, int& transitionLabelCounter, int source, int target, int rowSize, int colSize, const std::map<int, SpecialLocationType>& specialLocations);
};

#endif // MODIFIED_WAREHOUSE_XML_GENERATOR_HPP
