#include "WarehouseAutomationPlantUMLGenerator.hpp"

void WarehouseAutomationPlantUMLGenerator::generatePlantUML(const std::map<int, SpecialLocationType> &specialLocations, int rowSize, int colSize, const std::string &filename)
{
    std::string outputDir = "generated_models";
    std::string fullPumlPath = outputDir + "/" + filename;
    
    std::ofstream out(fullPumlPath);
    if (!out.is_open()) {
        std::cerr << "Failed to open " << filename << " for writing." << std::endl;
        return;
    }

    out << "@startuml " + filename + "\n";
    out << "skinparam state {\n";
    out << "  BackgroundColor<<normal>> White\n";
    out << "  BackgroundColor<<barrier>> Red\n";
    out << "  BackgroundColor<<oil_spill>> Yellow\n";
    out << "  BackgroundColor<<charging>> Green\n";
    out << "}\n\n";

    // Define states
    for (int row = 0; row < rowSize; ++row) {
        for (int col = 0; col < colSize; ++col) {
            int id = row * colSize + col + 1;
            auto it = specialLocations.find(id);
            SpecialLocationType specialType = (it != specialLocations.end()) ? it->second : SpecialLocationType::None;
            std::string typeName;
            std::string invariantText;
            std::string flowText;

            switch (specialType) {
                case SpecialLocationType::Type1:
                    typeName = " <<blocked>>";
                    invariantText = "x1==0 \\n x2==0, c>=0.1";
                    flowText = "x1'==0 \\n x2'==0, c'==-1";
                    break;
                case SpecialLocationType::Type2:
                    typeName = " <<oil_spill>";
                    invariantText = to_string(col) + "<=x1<=" + to_string(col + 1) + "\\n " + to_string(row) + "<=x2<=" + to_string(row + 1) + ", c>=0.1";
                    flowText = "-1<=x1'<=1 \\n -1<=x2'<=1, c'==-2";
                    break;
                case SpecialLocationType::Type3:
                    typeName = " <<charging>>";
                    invariantText = to_string(col) + "<=x1<=" + to_string(col + 1) + "\\n " + to_string(row) + "<=x2<=" + to_string(row + 1) + ", c>=0.1";
                    flowText = "-1<=x1'<=1 \\n -1<=x2'<=1, c'==0";
                    break;
                default:
                    typeName = " <<normal>>";
                    invariantText = to_string(col) + "<=x1<=" + to_string(col + 1) + "\\n " + to_string(row) + "<=x2<=" + to_string(row + 1) + ", c>=0.1";
                    flowText = "-1<=x1'<=1 \\n -1<=x2'<=1, c'==-1";
                    break;
            }

            // State with flow and invariant as a note
            out << "state \"l" << id << typeName << "\" as loc" << id << ":Inv: " << invariantText << "\\nFlow: " << flowText <<"\n";
        }
    }

    out << "\n";

    // Generate transitions based on adjacency, considering blocked locations
    int transition_counter = 1;
    int sourceRow, sourceCol, targetRow, targetCol;
    for (int row = 0; row < rowSize; row++) {
        for (int col = 0; col < colSize; col++) {
            int id = row * colSize + col + 1;
            auto currentTypeIt = specialLocations.find(id);
            sourceRow = (id - 1)/colSize;
            sourceCol = (id - 1)%colSize;
            targetRow = (id + 1 - 1)/colSize;
            targetCol = (id + 1 - 1)%colSize;

            // Transition to the right neighbor
            if (col < colSize - 1) {
                int rightNeighborId = id + 1;
                auto rightNeighborTypeIt = specialLocations.find(rightNeighborId);

                // Add transition if neither current nor neighbor is a barrier
                if (currentTypeIt->second != SpecialLocationType::Type1 && (rightNeighborTypeIt == specialLocations.end() || rightNeighborTypeIt->second != SpecialLocationType::Type1)) {
                    if (currentTypeIt->second == SpecialLocationType::Type3){
                        out << "loc" << id << " -right-> " << "loc" << rightNeighborId << ": e" + to_string(transition_counter++) + "\\n x1 ==" + to_string(sourceCol + 1) + "," + to_string(sourceRow) + "<=x2<" + to_string(sourceRow + 1) + "\\n c := 10" "\n";
                        out << "loc" << rightNeighborId << " -left-> " << "loc" << id << ": e" + to_string(transition_counter++) + "\\n x1 ==" + to_string(sourceCol + 1) + "," + to_string(sourceRow) + "<=x2<" + to_string(sourceRow + 1) + "\n";
                    }
                    else {
                        out << "loc" << id << " -right-> " << "loc" << rightNeighborId << ": e" + to_string(transition_counter++) + "\\n x1 ==" + to_string(sourceCol + 1) + "," + to_string(sourceRow) + "<=x2<" + to_string(sourceRow + 1) + "\n";
                        out << "loc" << rightNeighborId << " -left-> " << "loc" << id << ": e" + to_string(transition_counter++) + "\\n x1 ==" + to_string(sourceCol + 1) + "," + to_string(sourceRow) + "<=x2<" + to_string(sourceRow +1) + "\n";
                    }
/*                     out << "loc" << id << " -right-> " << "loc" << rightNeighborId << ": e" + to_string(transition_counter++) + "\\n x ==" + to_string(sourceRow) +  "\n";
                    out << "loc" << rightNeighborId << " -left-> " << "loc" << id << ": e" + to_string(transition_counter++) + "\\n x ==" + to_string(sourceRow) +  "\n";
 */                    
                }
                else {
                    out << "loc" << id << "-right[#white]->" << "loc" << rightNeighborId << "\n";
                }
            }

            // Transition to the top neighbor
            if (row < rowSize - 1) {
                int topNeighborId = id + colSize;
                auto topNeighborTypeIt = specialLocations.find(topNeighborId);

                // Add transition if neither current nor neighbor is a barrier
                if (currentTypeIt->second != SpecialLocationType::Type1 && (topNeighborTypeIt == specialLocations.end() || topNeighborTypeIt->second != SpecialLocationType::Type1)) {
                    if ( currentTypeIt->second == SpecialLocationType::Type3){
                        out << "loc" << id << " -up-> " << "loc" << topNeighborId << ": e" + to_string(transition_counter++) + "\\n" + to_string(sourceCol) + "<=x1<" + to_string(sourceCol + 1) + ", x2==" + to_string(sourceRow + 1) + "\\n c := 10" "\n";
                        out << "loc" << topNeighborId << " -down-> " << "loc" << id << ": e" + to_string(transition_counter++) + "\\n" + to_string(sourceCol) + "<=x1<" + to_string(sourceCol + 1) + ", x2==" + to_string(sourceRow + 1) + "\n";
                    }
                    else {
                        out << "loc" << id << " -up-> " << "loc" << topNeighborId << ": e" + to_string(transition_counter++) + "\\n" + to_string(sourceCol) + "<=x1<" + to_string(sourceCol + 1) + ", x2==" + to_string(sourceRow + 1) + "\n";
                        out << "loc" << topNeighborId << " -down-> " << "loc" << id << ": e" + to_string(transition_counter++) + "\\n" + to_string(sourceCol) + "<=x1<" + to_string(sourceCol + 1) + ", x2==" + to_string(sourceRow + 1) + "\n";
                    }

/*                     out << "loc" << id << " -up-> " << "loc" << topNeighborId << ": e" + to_string(transition_counter++) + "\\n x ==" + to_string(sourceRow) +  "\n";
                    out << "loc" << topNeighborId << " -down-> " << "loc" << id << ": e" + to_string(transition_counter++) + "\\n x ==" + to_string(sourceRow) +  "\n";
 */
                }
                else {
                    out << "loc" << id << "-up[#white]->" << "loc" << topNeighborId << "\n";
                }
            }
        }
    }

    out << "@enduml\n";
    out.close();
    std::cout << "PlantUML diagram generated successfully: " << filename << std::endl;
}
