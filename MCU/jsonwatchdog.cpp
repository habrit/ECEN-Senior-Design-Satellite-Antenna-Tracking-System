#include "Functions.cpp"

int main()
{
    // Set the URL of the JSON file
    std::string url = "https://satellite-antenna-tracker-default-rtdb.firebaseio.com/Commands/Commands.json";
    int runCount = 0;
    bool differencesFound = false;

    downloadAndPrettifyJSON(url, "ugly.json", "pretty.json");
    std::string cleanedSatelliteName;

    int delay = 10000;
    bool connected = false;
    bool disconnect = true;

    while (true)
    {

        std::string file_path = "ugly.json";
        if (!differencesFound)
        {
            // No Differences found, so we can just overwrite the file
        }
        else
        {
            downloadAndPrettifyJSON(url, file_path, "pretty.json");
            // std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
        disconnect = checkDisconnect("pretty.json");
        connected = checkConnectInJSON("pretty.json");
        if ((connected) & (!disconnect))
        {
            delay = 10;
            // std::cout << "CONNECTED" << std::endl;
        }
        else
        {
            delay = 10000;
        }

        // Repeat for the second file
        std::string file_path2 = "ugly2.json";
        downloadAndPrettifyJSON(url, file_path2, "pretty2.json");
        // std::this_thread::sleep_for(std::chrono::milliseconds(delay));

        // Compare the two files
        std::vector<std::string> results = compareFiles("pretty.json", "pretty2.json", false);
        differencesFound = !results.empty();
        // std::this_thread::sleep_for(std::chrono::milliseconds(delay));

        // std::cout << "Done" << std::endl;
        for (const auto &result : results)
        {
            std::string cleanedResult = result;
            // remove empty characters including tabs
            cleanedResult.erase(std::remove(cleanedResult.begin(), cleanedResult.end(), '\t'), cleanedResult.end());
            std::cout << cleanedResult << std::endl;
            // Check to see if cleanedResult contains a command
            if (cleanedResult.find(CONNECT) != std::string::npos)
            {
                std::cout << "CONNECT" << std::endl;
            }
            if (cleanedResult.find(DISCONNECT) != std::string::npos)
            {
                std::cout << "DISCONNECT" << std::endl;
            }
            if (cleanedResult.find("SatelliteName") != std::string::npos)
            {
                // Remove the SatelliteName: part
                cleanedSatelliteName = cleanedResult.erase(0, 16);
                // Remove the \n
                cleanedSatelliteName.erase(std::remove(cleanedSatelliteName.begin(), cleanedSatelliteName.end(), '\n'), cleanedSatelliteName.end());

                std::cout << cleanedSatelliteName << std::endl;
                // Run TLELookup and Predict with cleanedSatelliteName
                // Run TLELookup and Predict with cleanedSatelliteName in a thread so watchdog can continue
                std::thread tleLookupAndPredict(runTLELookupAndPredict, cleanedSatelliteName);
                // std::string command = "./TLELookup \"" + cleanedSatelliteName + "\" | ./TLEPredict";
                // std::cout << command << std::endl;
                // system(command.c_str());
                tleLookupAndPredict.detach();
            }
            if (cleanedResult.find("SatNameAuto") != std::string::npos)
            {
                // Remove the SatNameAuto: part
                std::string cleanedResult2 = cleanedResult.erase(0, 14);
                // Remove the \n
                cleanedResult2.erase(std::remove(cleanedResult2.begin(), cleanedResult2.end(), '\n'), cleanedResult2.end());
                // remove the ,
                cleanedResult2.erase(std::remove(cleanedResult2.begin(), cleanedResult2.end(), ','), cleanedResult2.end());
                // std::cout << "Cleaned Result is: " << cleanedResult2 << std::endl;
                // Remove the slashes
                cleanedResult2.erase(std::remove(cleanedResult2.begin(), cleanedResult2.end(), '/'), cleanedResult2.end());

                std::thread trackSatAuto(trackSatellite, cleanedResult2);
                trackSatAuto.detach();
            }
            if (cleanedResult.find("SetPosAz") != std::string::npos)
            {
                std::cout << "setPositionAzimuth" << std::endl;
                // Remove the setPosition: part
                std::string cleanedResult2 = cleanedResult.erase(0, 11);
                // Remove the \n
                cleanedResult2.erase(std::remove(cleanedResult2.begin(), cleanedResult2.end(), '\n'), cleanedResult2.end());
                // remove the ,
                cleanedResult2.erase(std::remove(cleanedResult2.begin(), cleanedResult2.end(), ','), cleanedResult2.end());
                std::cout << cleanedResult2 << std::endl;

                // Find line that contains SetPosEl in pretty2.json
                std::string line;
                std::ifstream myfile("pretty2.json");
                if (myfile.is_open())
                {
                    while (getline(myfile, line))
                    {
                        if (line.find("SetPosEl") != std::string::npos)
                        {
                            std::cout << line << '\n';
                            // Remove the SetPosEl: part
                            std::string cleanedResult3 = line.erase(0, 15);
                            // Remove the \n
                            cleanedResult3.erase(std::remove(cleanedResult3.begin(), cleanedResult3.end(), '\n'), cleanedResult3.end());
                            // remove the ,
                            cleanedResult3.erase(std::remove(cleanedResult3.begin(), cleanedResult3.end(), ','), cleanedResult3.end());
                            std::cout << cleanedResult3 << std::endl;
                            // Run moveToPosition with cleanedResult2 and cleanedResult3
                            // Thread move to position so watchdog can continue
                            std::thread moveToAz(moveToPosition, stringToDouble(cleanedResult2), stringToDouble(cleanedResult3));
                            moveToAz.detach();
                            // moveToPosition(stringToDouble(cleanedResult2), stringToDouble(cleanedResult3));
                        }
                    }
                    myfile.close();
                }
            }

            if (cleanedResult.find("SetPosEl") != std::string::npos)
            {
                std::cout << "setPositionElevation" << std::endl;
                // Remove the setPosition: part
                std::string cleanedResult2 = cleanedResult.erase(0, 11);
                // Remove the \n
                cleanedResult2.erase(std::remove(cleanedResult2.begin(), cleanedResult2.end(), '\n'), cleanedResult2.end());
                // remove the ,
                cleanedResult2.erase(std::remove(cleanedResult2.begin(), cleanedResult2.end(), ','), cleanedResult2.end());
                std::cout << cleanedResult2 << std::endl;

                // Find line that contains SetPosAz in pretty2.json
                std::string line;
                std::ifstream myfile("pretty2.json");
                if (myfile.is_open())
                {
                    while (getline(myfile, line))
                    {
                        if (line.find("SetPosAz") != std::string::npos)
                        {
                            std::cout << line << '\n';
                            // Remove the SetPosAz: part
                            std::string cleanedResult3 = line.erase(0, 15);
                            // Remove the \n
                            cleanedResult3.erase(std::remove(cleanedResult3.begin(), cleanedResult3.end(), '\n'), cleanedResult3.end());
                            // remove the ,
                            cleanedResult3.erase(std::remove(cleanedResult3.begin(), cleanedResult3.end(), ','), cleanedResult3.end());
                            std::cout << cleanedResult3 << std::endl;
                            // Run moveToPosition with cleanedResult2 and cleanedResult3
                            // Thread move to position so watchdog can continue
                            std::thread moveToEl(moveToPosition, stringToDouble(cleanedResult3), stringToDouble(cleanedResult2));
                            moveToEl.detach();
                            // moveToPosition(stringToDouble(cleanedResult3), stringToDouble(cleanedResult2));
                        }
                    }
                    myfile.close();
                }
            }
            if (cleanedResult.find("Park Satellite") != std::string::npos)
            {
                std::cout << "Park Satellite" << std::endl;
                // set all GPIO pins to low and write current Az El to file
                // Read Az and El from file
                std::string azPath = "AzimuthEx.txt";
                std::ifstream azFile(azPath);
                std::string contents;

                if (azFile.is_open())
                {
                    std::string line;
                    while (getline(azFile, line))
                    {
                        contents += line + "\n";
                        // std::cout << contents << std::endl;
                    }
                    azFile.close();
                }

                std::string elPath = "ElevationEx.txt";
                std::ifstream elFile(elPath);
                std::string elContents;

                if (elFile.is_open())
                {
                    std::string line;
                    while (getline(elFile, line))
                    {
                        elContents += line + "\n";
                        // std::cout << contents << std::endl;
                    }
                    elFile.close();
                }
                // Run break function
                breakFunction;
                // Write Az and El to file
                std::ofstream azFileOut(azPath);
                azFileOut << contents;
                azFileOut.close();

                std::ofstream elFileOut(elPath);
                elFileOut << elContents;
                elFileOut.close();
            }
        }
        // std::cout << "Differences found: " << differencesFound << std::endl;
        //  std::cout << "Done" << std::endl;

        // std::cout << "Run: " << runCount << std::endl;
        runCount++;
    }

    return 0;
}