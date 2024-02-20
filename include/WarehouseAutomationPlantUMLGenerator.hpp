// WarehouseAutomationPlantUMLGenerator.hpp
#ifndef WAREHOUSE_AUTOMATION_PLANT_UML_GENERATOR_HPP
#define WAREHOUSE_AUTOMATION_PLANT_UML_GENERATOR_HPP

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include "warehouse_xml_generator.hpp"

using namespace std;

class WarehouseAutomationPlantUMLGenerator {
public:
    // Generates a PlantUML diagram and saves it to a file.
    // @param specialLocations A map of location IDs to their special types.
    // @param transitions A vector of pairs representing transitions between locations.
    // @param filename The name of the file to save the PlantUML diagram to.
    static void generatePlantUML(const std::map<int, SpecialLocationType>& specialLocations,
                                 int rowSize, int colSize,
                                 const std::string& filename);
};

#endif // WAREHOUSE_AUTOMATION_PLANT_UML_GENERATOR_HPP
