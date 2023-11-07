#include "Functions.cpp"
#include <cstdlib>

int main() {
    // Set the URL of the JSON file
    std::string url = "https://satellite-antenna-tracker-default-rtdb.firebaseio.com/Commands/Commands.json";

    bool differencesFound = false;

    downloadAndPrettifyJSON(url, "ugly.json", "pretty.json");
    std::string cleanedSatelliteName;

int delay = 10000;
bool connected = false;
bool disconnect = true;

    while (true) {


        //set time for current run
        auto now = std::chrono::system_clock::now();
        std::string file_path = "ugly.json";
        if (!differencesFound) {
            //No Differences found, so we can just overwrite the file
        } else {
            downloadAndPrettifyJSON(url, file_path, "pretty.json");
            //std::this_thread::sleep_for(std::chrono::milliseconds(delay));

        }
        disconnect = checkDisconnect("pretty.json");
        connected = checkConnectInJSON("pretty.json");
        if ((connected) & (!disconnect)){
            delay = 10;
            //std::cout << "CONNECTED" << std::endl;
        } else {
            delay = 10000;
        }

        // Repeat for the second file
        std::string file_path2 = "ugly2.json";
        downloadAndPrettifyJSON(url, file_path2, "pretty2.json");
        //std::this_thread::sleep_for(std::chrono::milliseconds(delay));

        // Compare the two files
        std::vector<std::string> results = compareFiles("pretty.json", "pretty2.json", false);
        differencesFound = !results.empty();
        //std::this_thread::sleep_for(std::chrono::milliseconds(delay));

        //std::cout << "Done" << std::endl;
        for (const auto& result : results) {
            std::string cleanedResult = result;
            //remove empty characters including tabs
            cleanedResult.erase(std::remove(cleanedResult.begin(), cleanedResult.end(), '\t'), cleanedResult.end());
            std::cout << cleanedResult << std::endl;
            //Check to see if cleanedResult contains a command
            if (cleanedResult.find(CONNECT) != std::string::npos) {
                std::cout << "CONNECT" << std::endl;
            }
            if (cleanedResult.find("SatelliteName") != std::string::npos) {
                //Remove the SatelliteName: part
                cleanedSatelliteName = cleanedResult.erase(0, 16);
                //Remove the \n
                cleanedSatelliteName.erase(std::remove(cleanedSatelliteName.begin(), cleanedSatelliteName.end(), '\n'), cleanedSatelliteName.end());
                
                std::cout << cleanedSatelliteName << std::endl;
                //Run TLELookup and Predict with cleanedSatelliteName
                std::string command = "./TLELookup \"" + cleanedSatelliteName + "\" | ./TLEPredict";
                std::cout << command << std::endl;
                system(command.c_str());
            } 
        }
        //std::cout << "Differences found: " << differencesFound << std::endl;
        //std::cout << "Done" << std::endl;
        //set time for end of run
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - now;
        //std::cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";
        //Append time to file
        std::ofstream myfile;
        myfile.open ("time.txt", std::ios_base::app);
        myfile << elapsed_seconds.count() << "\n";
        myfile.close();



        
    }

    return 0;
}
