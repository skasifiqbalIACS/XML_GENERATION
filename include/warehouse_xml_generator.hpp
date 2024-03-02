// warehouse_xml_generator.h

#ifndef WAREHOUSE_SIMULATION_HPP
#define WAREHOUSE_SIMULATION_HPP

#include "tinyxml2.h"
#include <map>
#include <string>
#include <random>

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
    std::tuple<double, double, int, int> generateInitialSetup(const std::map<int, SpecialLocationType>& specialLocations, int rowSize, int colSize, int startingCharge);
    
    void virtual generateCfgFile(const std::tuple<double, double, int, int>& initialSetup, const std::string& filename, int startingCharge);

    // virtual void generateCfgFile(const std::map<int, SpecialLocationType>& specialLocations, const std::string& filename, int rowSize, int colSize, int startingCharge);
    // void run(int rowSize, int colSize, int numType1, int numType2, int numType3);
    std::map<int, SpecialLocationType> GenerateSpecialLocations(int rowSize, int colSize, int numType1, int numType2, int numType3);
    virtual void GenerateFilesForModel(const std::map<int, SpecialLocationType>& specialLocations,
                                                    const std::string& baseName, int rowSize, int colSize, int depth, int startingCharge, 
                                                    const std::tuple<double, double, int, int> initialSetup); 
    virtual void GenerateXml(int rowSize, int colSize, const std::map<int, SpecialLocationType>& specialLocations, const std::string& xmlFilename);


    virtual ~warehouse_xml_generator();
protected:
    int rowSize, colSize;
    int numType1, numType2, numType3;
    int depth;
    int charge;
    /* std::map<int, SpecialLocationType> specialLocations;
    std::mt19937 rng; // Mersenne Twister pseudo-random generator
    std::uniform_int_distribution<std::mt19937::result_type> dist; */

    virtual void AddParameters(tinyxml2::XMLNode* component, int numVariables, int numTransitions);
    virtual void AddLocation(tinyxml2::XMLNode* component, int id, int x1Min, int x1Max, int x2Min, int x2Max, SpecialLocationType specialType = SpecialLocationType::None);
    void AddTransition(tinyxml2::XMLNode* component, int& transitionLabelCounter, int source, int target, int rowSize, int colSize, const std::map<int, SpecialLocationType>& specialLocations);
};

#endif // WAREHOUSE_SIMULATION_HPP
