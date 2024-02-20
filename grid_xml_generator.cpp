#include <iostream>
#include <fstream>
#include <string>

std::string createCell(int x, int y, int n) {
    // Generate the XML for a single cell
    std::string cellXml;
    int cellId = x * n + y + 1;
    cellXml += "    <location id=\"" + std::to_string(cellId) + "\" name=\"loc" + std::to_string(cellId) + "\">\n";
    cellXml += "      <invariant>x1>=" + std::to_string(x) + " & x1<=" + std::to_string(x + 1) + " & x2>=" + std::to_string(y) + " & x2<=" + std::to_string(y + 1) + " & c>=0.1</invariant>\n";
    cellXml += "      <flow>x1'==z & x2'==z & c'==-1</flow>\n";
    cellXml += "    </location>\n";

    // Add transitions (simple pattern for demonstration)
    if (x < n - 1) {
        cellXml += "    <transition source=\"" + std::to_string(cellId) + "\" target=\"" + std::to_string((x + 1) * n + y + 1) + "\">\n";
        cellXml += "      <label>e" + std::to_string(cellId * 2 + 1) + "</label>\n";
        cellXml += "      <guard>x1==" + std::to_string(x + 1) + " & x2>=" + std::to_string(y) + " & x2<=" + std::to_string(y + 1) + "</guard>\n";
        cellXml += "    </transition>\n";
    }
    if (y < n - 1) {
        cellXml += "    <transition source=\"" + std::to_string(cellId) + "\" target=\"" + std::to_string(x * n + y + 2) + "\">\n";
        cellXml += "      <label>e" + std::to_string(cellId * 2 + 2) + "</label>\n";
        cellXml += "      <guard>x1>=" + std::to_string(x) + " & x1<=" + std::to_string(x + 1) + " & x2==" + std::to_string(y + 1) + "</guard>\n";
        cellXml += "    </transition>\n";
    }

    return cellXml;
}

void generateXml(int n) {
    // Open file for writing
    std::ofstream xmlFile("output.xml");
    if (!xmlFile.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }

    // Write the beginning of the XML
    xmlFile << "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n";
    xmlFile << "<sspaceex xmlns=\"http://www-verimag.imag.fr/xml-namespaces/sspaceex\" version=\"0.2\" math=\"SpaceEx\">\n";
    xmlFile << "  <component id=\"warehouse_automation_agent\">\n";

    // Generate XML for each cell
    for (int x = 0; x < n; ++x) {
        for (int y = 0; y < n; ++y) {
            xmlFile << createCell(x, y, n);
        }
    }

    // Close the XML structure
    xmlFile << "  </component>\n";
    xmlFile << "</sspaceex>\n";

    // Close the file
    xmlFile.close();
}

int main() {
    int n = 3; // Example size
    generateXml(n);
    std::cout << "XML file generated for " << n << "x" << n << " grid." << std::endl;
    return 0;
}

