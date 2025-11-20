#ifndef CSV_READER_H
#define CSV_READER_H

#include "Student.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <iostream>

class CSVReader {
private:
    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, (last - first + 1));
    }
    
public:
    static std::vector<std::shared_ptr<Student<std::string, std::string>>> 
    readStudentsStringString(const std::string& filename) {
        std::vector<std::shared_ptr<Student<std::string, std::string>>> students;
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return students;
        }
        
        std::string line;
        if (std::getline(file, line)) {
            if (line.find("name") == std::string::npos && 
                line.find("roll") == std::string::npos) {
                file.seekg(0);
            }
        } else {
            file.seekg(0);
        }
        
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            
            std::istringstream ss(line);
            std::string token;
            std::vector<std::string> tokens;
            
            while (std::getline(ss, token, ',')) {
                tokens.push_back(trim(token));
            }
            
            if (tokens.size() >= 4) {
                try {
                    std::string name = tokens[0];
                    std::string rollNumber = tokens[1];
                    std::string branch = tokens[2];
                    int startingYear = std::stoi(tokens[3]);
                    
                    auto student = std::make_shared<Student<std::string, std::string>>(
                        name, rollNumber, branch, startingYear);
                    
                    if (tokens.size() > 4 && !tokens[4].empty()) {
                        std::istringstream coursesStream(tokens[4]);
                        std::string courseEntry;
                        while (std::getline(coursesStream, courseEntry, ';')) {
                            courseEntry = trim(courseEntry);
                            if (courseEntry.empty()) continue;
                            
                            size_t colonPos = courseEntry.find(':');
                            if (colonPos != std::string::npos) {
                                std::string courseCode = trim(courseEntry.substr(0, colonPos));
                                std::string gradeStr = trim(courseEntry.substr(colonPos + 1));
                                if (!courseCode.empty() && !gradeStr.empty()) {
                                    try {
                                        double grade = std::stod(gradeStr);
                                        student->addPreviousCourse(courseCode, grade);
                                    } catch (...) {
                                    }
                                }
                            }
                        }
                    }
                    
                    students.push_back(student);
                } catch (...) {
                    continue;
                }
            }
        }
        
        file.close();
        return students;
    }
    
    static std::vector<std::shared_ptr<Student<std::string, int>>> 
    readStudentsStringInt(const std::string& filename) {
        std::vector<std::shared_ptr<Student<std::string, int>>> students;
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return students;
        }
        
        std::string line;
        if (std::getline(file, line)) {
            if (line.find("name") == std::string::npos && 
                line.find("roll") == std::string::npos) {
                file.seekg(0);
            }
        } else {
            file.seekg(0);
        }
        
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            
            std::istringstream ss(line);
            std::string token;
            std::vector<std::string> tokens;
            
            while (std::getline(ss, token, ',')) {
                tokens.push_back(trim(token));
            }
            
            if (tokens.size() >= 4) {
                try {
                    std::string name = tokens[0];
                    std::string rollNumber = tokens[1];
                    std::string branch = tokens[2];
                    int startingYear = std::stoi(tokens[3]);
                    
                    auto student = std::make_shared<Student<std::string, int>>(
                        name, rollNumber, branch, startingYear);
                    
                    if (tokens.size() > 4 && !tokens[4].empty()) {
                        std::istringstream coursesStream(tokens[4]);
                        std::string courseEntry;
                        while (std::getline(coursesStream, courseEntry, ';')) {
                            courseEntry = trim(courseEntry);
                            if (courseEntry.empty()) continue;
                            
                            size_t colonPos = courseEntry.find(':');
                            if (colonPos != std::string::npos) {
                                std::string courseCodeStr = trim(courseEntry.substr(0, colonPos));
                                std::string gradeStr = trim(courseEntry.substr(colonPos + 1));
                                if (!courseCodeStr.empty() && !gradeStr.empty()) {
                                    try {
                                        int courseCode = std::stoi(courseCodeStr);
                                        double grade = std::stod(gradeStr);
                                        student->addPreviousCourse(courseCode, grade);
                                    } catch (...) {
                                    }
                                }
                            }
                        }
                    }
                    
                    students.push_back(student);
                } catch (...) {
                    continue;
                }
            }
        }
        
        file.close();
        return students;
    }
};

#endif
