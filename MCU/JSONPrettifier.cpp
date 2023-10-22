#include <iostream>
#include <fstream>
#include <string>
#include "Functions.cpp"

using namespace std;


int main() {
    // Read the JSON file
    ifstream inputFile("ugly.json");
    string jsonData((istreambuf_iterator<char>(inputFile)), (istreambuf_iterator<char>()));
    inputFile.close();

    // Prettify the JSON
    string prettyJson = prettifyJson(jsonData);

    // Write the prettified JSON to a new file
    ofstream outputFile("output.json");
    outputFile << prettyJson;
    outputFile.close();

    return 0;
}
