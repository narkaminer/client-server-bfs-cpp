#include <iostream>
#include <fstream>

int main() {
    std::ofstream file("graph.csv");
    if (!file) {
        std::cerr << "Failed to create file!" << std::endl;
        return 1;
    }

    const int numVertices = 1000;

    for (int i = 0; i < numVertices - 1; ++i) {
        file << i << " " << i + 1 << std::endl; 
    }

    file.close();
    std::cout << "Graph file created successfully." << std::endl;
    return 0;
}
