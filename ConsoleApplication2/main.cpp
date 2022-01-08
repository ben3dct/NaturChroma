#pragma once
#include <iostream>
#include <algorithm>
#include "includes/httplib.h"
#include "includes/json.hpp"
#include "includes/RzCustomEvents.h"
#include "resource.h"

using json = nlohmann::json;
int WdataT;

VOID WINAPI SetConsoleColors(WORD attribs);

int fg()
{
    string YOUR_ACCESS_KEY;
    string ECity;
    
    SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);

    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hstdout, &csbi);

    csbi.dwSize.X = csbi.dwMaximumWindowSize.X;
    csbi.dwSize.Y = csbi.dwMaximumWindowSize.Y;
    SetConsoleScreenBufferSize(hstdout, csbi.dwSize);

    HWND x = GetConsoleWindow();
    ShowScrollBar(x, SB_BOTH, FALSE);
    
    ifstream readkey("key.txt");
    if (readkey.is_open())
    {
        getline(readkey, YOUR_ACCESS_KEY);
        readkey.close();
    }

    ifstream readcity("city.txt");
    if (readcity.is_open())
    {
        getline(readcity, ECity);
        readcity.close();      
    }
  
    std::string city = ECity;
    std::string key = YOUR_ACCESS_KEY;

    httplib::Client cli("http://api.weatherstack.com");
    auto res = cli.Get((std::string("/current?access_key=") + key + "&query=" + city).c_str());

    if (res->status == 200)
    {
        auto j = json::parse(res->body);
        try
        {
            int g = j.at("current").at("temperature");
            std::string city = j.at("location").at("name");
            std::string weather = j.at("current").at("weather_descriptions").at(0);
            int PPtemp = j.at("current").at("feelslike");
            std::string time = j.at("location").at("localtime");
            time.erase(0, 11);
            std::string wind_dir = j.at("current").at("wind_dir");
            double Pwind_speed = j.at("current").at("wind_speed");

            return g;
        }
        catch (const std::exception&)
        {
            SetConsoleColors(BACKGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            std::cout << "Failed to receive Weather data from https://api.weatherstack.com. \n" << "TROUBLESHOOTING:" << std::endl;
            std::cout << "Either the your access key is invalid or the entered city could not be found.\n";
            std::cout << "The API key can be retrieved from https://weatherstack.com/dashboard \n";
            std::cout << "Alternativley, the monthly usage limit has been reached. Usage can be viewed via the link above.";
            return EXIT_FAILURE;
        }
    }
    else
    {
        SetConsoleColors(BACKGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        std::cout << "Failed to receive Weather data from https://api.weatherstack.com. \n" << "TROUBLESHOOTING:" << std::endl;
        std::cout << "Either the access key is invalid or the entered city could not be found.\n";
        std::cout << "The API key can be retrieved from https://weatherstack.com/dashboard\n \n";
        std::cout << "Alternativley, the monthly usage limit has been reached. Usage can be viewed via the link above.";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    string APIkey;
    string city;

    SetConsoleColors(BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
    std::cout << "Enter API key: " << std::endl;
    cin >> APIkey;
    
    ofstream keyfile;
    keyfile.open("key.txt");
    keyfile << APIkey + "\n";
    keyfile.close();
    std::cout << APIkey + " saved locally! \n";
   
    std::cout << "Enter City: " << std::endl;
    cin >> city;

    ofstream cityfile;
    cityfile.open("city.txt");
    cityfile <<city + "\n";
    cityfile.close();

    std::cout << city + " saved locally! \n";
    std::cout << "Starting."; Sleep(50);
    std::cout << "."; Sleep(50);
    std::cout << ".\n";

    My_Chroma_Implementation impl_test; //Initialize ChromaSDK
    auto test_for_init = impl_test.Initialize(); // Initialize all Chroma devices
    if (test_for_init == TRUE) {
        std::cout << "Chroma Initialized.\n";
        WdataT = fg();
        while (true) { //Running all functions in a loop until you ctrl+c the programm
            auto Keyboard = impl_test.example_keyboard();
            auto Mouse = impl_test.example_mouse();
            auto Mousemat = impl_test.example_mousemat();
            Sleep(50); // The SDK doesn't like to get spammed 
             //If you want to, catch the BOOL Values and display a message if something has gone wrong
        }
    }
    else
    {
        std::cout << "Unable to initialize Chroma.\n";
        std::cin.ignore();
    }
    return 0;
}

VOID WINAPI SetConsoleColors(WORD attribs) {
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFOEX cbi;
    cbi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
    GetConsoleScreenBufferInfoEx(hOutput, &cbi);
    cbi.wAttributes = attribs;
    SetConsoleScreenBufferInfoEx(hOutput, &cbi);
}

