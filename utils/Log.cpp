//
// Created by Воробьев Егор on 25.01.2023
//

#include "Log.h"
#include "Settings.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include <iomanip>


namespace Log {
    void log(const std::string& msg) {
        if (Settings::USE_LOG_FILE) {
            std::time_t const now = std::time(nullptr);
            auto dt = std::put_time(std::localtime(&now), "%F %T");

            std::fstream file;
            file.open("log.txt", std::ios::app | std::ios::out);
            file << dt << "\t" << msg << std::endl;
            if (Settings::PRINT_LOG)
                std::cout << dt << "\t" << msg << std::endl;
            file.close();
        }
    }    
};
