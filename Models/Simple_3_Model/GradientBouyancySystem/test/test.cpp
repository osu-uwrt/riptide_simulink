#include <vector>
#include "../grad_buoy/GradientBouyanC.h"

int main() {
    
    
    {
        std::vector<double> bounding_box = {.3503, .3863, .4337, .4299, .1791, .1829};
        std::vector<Vertex> raw_vertices = generateRawVertices(bounding_box);

        int c = 0;
        for (auto i : raw_vertices) {
            std::cout << "Vertex " << c++ << "\n";
            std::cout << "\t* location: (" << i.location.x << ", " << i.location.y << ", " << i.location.z << ")\n";
            std::cout << "\t* c1_relat: (" << i.c1_relative.x << ", " << i.c1_relative.y << ", " << i.c1_relative.z << ")\n";
            std::cout << "\t* c2_relat: (" << i.c1_relative.x << ", " << i.c2_relative.y << ", " << i.c2_relative.z << ")\n";
            std::cout << "\t* c3_relat: (" << i.c3_relative.x << ", " << i.c3_relative.y << ", " << i.c3_relative.z << ")\n\n";
        }
    }
}

