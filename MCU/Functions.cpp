#include "Celestrak.h"
#include "Constants.cpp"
#include "TLEPredictFunctions.cpp"
#include "GPIOFunctions.cpp"

std::atomic<bool> stopTracking(false);
std::atomic<bool> flipOver(false);

void collectCelestrakData()
{
    const char *command = "curl 'https://celestrak.org/NORAD/elements/gp.php?GROUP=active&FORMAT=tle' > Celestrak.txt"; // Replace with the command you want to run
    int result = std::system(command);

    if (result == 0)
    {
        std::cout << "Command executed successfully." << std::endl;
    }
    else
    {
        std::cerr << "Command failed with exit code " << result << std::endl;
    }
}

bool findSatelliteData(const std::string &satName)
{
    std::string line;
    std::string satLine1;
    std::string satLine2;
    std::string satLine3;

    std::ifstream myfile("Celestrak.txt");
    if (myfile.is_open())
    {
        while (std::getline(myfile, line))
        {
            if (line.find(satName) != std::string::npos)
            {
                satLine1 = line;
                if (std::getline(myfile, line))
                {
                    satLine2 = line;
                }
                if (std::getline(myfile, line))
                {
                    satLine3 = line;
                }
                myfile.close();
                // Convert std::string to const char* if needed
                const char *cSatLine1 = satLine1.c_str();
                const char *cSatLine2 = satLine2.c_str();
                const char *cSatLine3 = satLine3.c_str();
                std::cout << cSatLine1 << std::endl;
                std::cout << cSatLine2 << std::endl;
                std::cout << cSatLine3 << std::endl;
                return true;
            }
        }
        myfile.close();
    }
    else
    {
        std::cerr << "Unable to open file" << std::endl;
    }
    return false;
}

void exampleTLELookupCase()
{
    /*Example Code that would be written in a main function

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


    */

    std::string satName = "ODIN"; // Replace with the actual satellite name
    bool foundSatellite = findSatelliteData(satName);

    if (!foundSatellite)
    {
        std::cerr << "Satellite information not found for: " << satName << std::endl;
        return;
    }
}

// Function to prettify JSON
std::string prettifyJson(std::string json)
{
    std::string prettyJson = "";
    int indentLevel = 0;
    bool inString = false;

    for (int i = 0; i < json.length(); i++)
    {
        char c = json[i];

        if (c == '"' && json[i - 1] != '\\')
        {
            inString = !inString;
        }

        if (inString)
        {
            prettyJson += c;
            continue;
        }

        if (c == '{' || c == '[')
        {
            prettyJson += c;
            prettyJson += '\n';
            indentLevel++;
            for (int j = 0; j < indentLevel; j++)
            {
                prettyJson += '\t';
            }
        }
        else if (c == '}' || c == ']')
        {
            prettyJson += '\n';
            indentLevel--;
            for (int j = 0; j < indentLevel; j++)
            {
                prettyJson += '\t';
            }
            prettyJson += c;
        }
        else if (c == ',')
        {
            prettyJson += c;
            prettyJson += '\n';
            for (int j = 0; j < indentLevel; j++)
            {
                prettyJson += '\t';
            }
        }
        else
        {
            prettyJson += c;
        }
    }

    return prettyJson;
}

// Function to download and prettify JSON data
void downloadAndPrettifyJSON(const std::string &url, const std::string &outputPath, const std::string &prettyOutputPath)
{
    // Open the file for writing
    std::ofstream file(outputPath, std::ios::binary);

    // Open the URL
    FILE *fp = popen(("curl -s " + url + " > " + outputPath).c_str(), "r");

    // Check if the URL was opened successfully
    if (!fp)
    {
        std::cerr << "Error opening URL" << std::endl;
        return;
    }

    // Close the URL
    pclose(fp);

    // Read the JSON file
    std::ifstream jsonFile(outputPath);
    std::string json((std::istreambuf_iterator<char>(jsonFile)), std::istreambuf_iterator<char>());

    // Prettify the JSON
    std::string prettyJson = prettifyJson(json);

    // Write the prettified JSON to a file
    std::ofstream outputFile(prettyOutputPath);
    outputFile << prettyJson;
    outputFile.close();

    // Close the file
    file.close();
}

std::vector<std::string> compareFiles(const std::string &file1_path, const std::string &file2_path, bool debugging)
{
    std::ifstream file1(file1_path), file2(file2_path);
    std::string line1, line2;
    int line_num = 1;
    bool differencesFound = false;
    std::vector<std::string> differences;

    while (std::getline(file1, line1) && std::getline(file2, line2))
    {
        if (line1 != line2)
        {
            if (debugging)
            {
                std::cout << "Difference found on line " << line_num << ":\n";
                std::cout << "File 1: " << line1 << "\n";
                std::cout << "File 2: " << line2 << "\n";
            }
            // Remove empty characters
            differences.push_back(line2 + "\n");
            differencesFound = true;
        }
        line_num++;
    }

    while (std::getline(file1, line1))
    {
        if (debugging)
        {
            std::cout << "File 1 has additional lines:\n";
            std::cout << line1 << "\n";
        }
        differencesFound = true;
        differences.push_back("Additional line in file 1: " + line1 + "\n");
        line_num++;
    }

    while (std::getline(file2, line2))
    {
        if (debugging)
        {
            std::cout << "File 2 has additional lines:\n";
            std::cout << line2 << "\n";
        }
        differencesFound = true;
        differences.push_back("Additional line in file 2: " + line2 + "\n");
        line_num++;
    }

    if ((!differencesFound) && (debugging))
    {
        std::cout << "Files are identical.\n";
    }

    return differences;
}

// check first file to see if connect is set to 1
bool checkConnectInJSON(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "File not found: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        // Search for the specific string in each line
        if (line.find("		\"Connect\":1,") != std::string::npos)
        {
            return true;
        }
    }

    return false;
}

bool checkDisconnect(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "File not found: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        // Search for the specific string with flexible whitespace
        if (line.find("\"Disconnect\":1") != std::string::npos)
        {
            breakFunction();
            stopTracking = true;
            flipOver = false;
            return true;
        }
    }

    return false;
}

// Find the time in JSON File and return next 5 lines
std::vector<std::string> findLineInJson(const std::string &filename, const std::string &searchLine)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "File not found: " << filename << std::endl;
        return {};
    }
    // Remove the slashes and quotes from the searchLine
    std::string searchLineCleaned = searchLine;
    searchLineCleaned.erase(std::remove(searchLineCleaned.begin(), searchLineCleaned.end(), '\"'), searchLineCleaned.end());
    searchLineCleaned.erase(std::remove(searchLineCleaned.begin(), searchLineCleaned.end(), '/'), searchLineCleaned.end());
    searchLineCleaned.erase(std::remove(searchLineCleaned.begin(), searchLineCleaned.end(), '\"'), searchLineCleaned.end());
    std::vector<std::string> result;
    std::string line;
    bool found = false;
    while (std::getline(file, line))
    {
        if (line.find(searchLineCleaned) != std::string::npos)
        {
            found = true;
            result.push_back(line);
            for (int i = 0; i < 5 && std::getline(file, line); i++)
            {
                result.push_back(line);
            }
            break;
        }
    }

    if (!found)
    {
        std::cerr << "Line not found: " << searchLine << std::endl;
        return {};
    }

    return result;
}

double stringToDouble(std::string str)
{
    double result = 0.0;
    double factor = 1.0;
    bool decimal = false;
    int decimalPlaces = 0;

    for (char c : str)
    {
        if (c == '.')
        {
            decimal = true;
        }
        else if (c >= '0' && c <= '9')
        {
            if (decimal)
            {
                decimalPlaces++;
                factor /= 10.0;
                result += (c - '0') * factor;
            }
            else
            {
                result = result * 10.0 + (c - '0');
            }
        }
    }

    return result;
}

void pushFiletoFirebase(std::string fileName)
{
    const char *commandPart1 = "curl -X PUT -d @";
    const char *commandPart2 = " https://satellite-antenna-tracker-default-rtdb.firebaseio.com/SatelliteData/";

    std::string command = commandPart1 + fileName + commandPart2 + fileName;
    int result = std::system(command.c_str());
    std::cout << command << std::endl;
}

double jsonTimeAZLookup(const std::string &filename, const std::string &targetTime)
{
    std::ifstream file(filename);
    std::string line;
    bool foundTarget = false;
    std::string azimuth;

    while (std::getline(file, line))
    {
        if (line.find(targetTime) != std::string::npos)
        {
            foundTarget = true;
        }

        if (foundTarget)
        {
            for (int i = 0; i < 4; i++)
            {
                if (std::getline(file, line))
                {
                    if (line.find("azimuth") != std::string::npos)
                    {
                        std::cout << line << std::endl;
                        azimuth = line;
                    }
                }
                else
                {
                    break;
                }
            }
            break;
        }
    }

    // Remove everything but numbers and period from azimuth
    azimuth.erase(std::remove_if(azimuth.begin(), azimuth.end(), [](char c)
                                 { return !std::isdigit(c) && c != '.'; }),
                  azimuth.end());
    // std::cout << azimuth << std::endl;
    return std::stod(azimuth);
}
double jsonTimeELLookup(const std::string &filename, const std::string &targetTime)
{
    std::ifstream file(filename);
    std::string line;
    bool foundTarget = false;
    std::string elevation;

    while (std::getline(file, line))
    {
        if (line.find(targetTime) != std::string::npos)
        {
            foundTarget = true;
        }

        if (foundTarget)
        {
            for (int i = 0; i < 5; i++)
            {
                if (std::getline(file, line))
                {
                    if (line.find("elevation") != std::string::npos)
                    {
                        std::cout << line << std::endl;
                        elevation = line;
                    }
                }
                else
                {
                    break;
                }
            }
            break;
        }
    }

    // Remove everything but numbers, period and negative sign from elevation
    elevation.erase(std::remove_if(elevation.begin(), elevation.end(), [](char c)
                                   { return !std::isdigit(c) && c != '.' && c != '-'; }),
                    elevation.end());
    // std::cout << azimuth << std::endl;
    return std::stod(elevation);
}

void runTLELookupAndPredict(const std::string &satName)
{

    // Run TLE lookup and prediction
    std::string command = "./TLELookup \"" + satName + "\" | ./TLEPredict";
    std::cout << command << std::endl;
    system(command.c_str());
}

std::string findLineWithString(const std::string &searchString)
{
    std::string filename = "Celestrak.txt";
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        if (line.find(searchString) != std::string::npos)
        {
            std::cout << line << std::endl;
            return line;
        }
    }

    return ""; // Return an empty string if the search string is not found
}


