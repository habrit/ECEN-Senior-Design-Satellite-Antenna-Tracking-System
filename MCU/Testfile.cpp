#include "Functions.cpp"
#include <thread>

// Function for the first thread
void threadFunction1()
{
    for (int i = 0; i < 500; ++i)
    {
        std::cout << "Thread 1: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

// Function for the second thread
void threadFunction2()
{
    for (int i = 0; i < 500; ++i)
    {
        std::cout << "Thread 2: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void threadFunction3()
{
    for (int i = 0; i < 500; ++i)
    {
        // Write data to file named thread3.txt
        std::ofstream myfile;
        myfile.open("thread3.txt");
        myfile << "Thread 3: " << i << std::endl;
        myfile.close();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void testThread()
{
    std::thread t1(threadFunction1);
    std::thread t2(threadFunction2);
    std::thread t3(threadFunction3);

    // Wait for both threads to finish
    t1.join();
    t2.join();
    t3.join();
}

int main()
{
    std::string tleName = "TEST";
    std::string url = "https://satellite-antenna-tracker-default-rtdb.firebaseio.com/Commands/Commands.json";
    // Create two threads and run the functions concurrently
    // testThread();
    // moveToPosition(36,125);
    // trackSatellite("ODIN");
    // std::vector <std::string> test = findLineInJson("ODIN.json", "Satellite Info");
    // std::thread AzIncrementThread(AzIncrement, 60, 1);
    // std::thread ElIncrementThread(ElIncrement, 60, 1);
    // AzIncrementThread.join();
    // ElIncrementThread.join();
    // gpioOutmicro(19,100,true);
    //downloadAndPrettifyJSON(url, "ugly.json", "pretty.json");
    //trackSatellite(tleName);
    //tleName = findTLEName(tleName);
    //std::cout << tleName << std::endl;
    std::vector<std::string> timeJSON = findLineInJson("Celestrak.txt", "ISS");
    std::cout << timeJSON[0] << std::endl;



    return 0;
}