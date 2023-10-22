#include "Functions.cpp"

int main(int argc, char *argv[]) {
    
    
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <satellite_name>" << std::endl;
        return 1;
    }

    std::string satName = argv[1];
    
    if (!findSatelliteData(satName)) {
        std::cerr << "Satellite information not found for: " << satName << std::endl;
        return 1;
    }


    return 0;
}

