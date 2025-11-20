#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <map>
#include <iostream>

template<typename R, typename C>
class Student {
private:
    std::string name;
    R rollNumber;
    std::string branch;
    int startingYear;
    std::map<C, double> currentCourses;
    std::map<C, double> previousCourses;

public:
    Student(const std::string& name, const R& rollNumber, 
            const std::string& branch, int startingYear)
        : name(name), rollNumber(rollNumber), branch(branch), 
          startingYear(startingYear) {}

    std::string getName() const { return name; }
    R getRollNumber() const { return rollNumber; }
    std::string getBranch() const { return branch; }
    int getStartingYear() const { return startingYear; }
    const std::map<C, double>& getCurrentCourses() const { return currentCourses; }
    const std::map<C, double>& getPreviousCourses() const { return previousCourses; }

    void setName(const std::string& name) { this->name = name; }
    void setRollNumber(const R& rollNumber) { this->rollNumber = rollNumber; }
    void setBranch(const std::string& branch) { this->branch = branch; }
    void setStartingYear(int year) { startingYear = year; }

    void addCurrentCourse(const C& courseCode, double grade) {
        currentCourses[courseCode] = grade;
    }

    void addPreviousCourse(const C& courseCode, double grade) {
        previousCourses[courseCode] = grade;
    }

    void completeCourse(const C& courseCode) {
        auto it = currentCourses.find(courseCode);
        if (it != currentCourses.end()) {
            double grade = it->second;
            currentCourses.erase(it);
            previousCourses[courseCode] = grade;
        }
    }

    double getGrade(const C& courseCode) const {
        auto it = previousCourses.find(courseCode);
        if (it != previousCourses.end()) {
            return it->second;
        }
        it = currentCourses.find(courseCode);
        if (it != currentCourses.end()) {
            return it->second;
        }
        return -1.0;
    }

    bool operator==(const Student& other) const {
        return rollNumber == other.rollNumber;
    }

    bool operator<(const Student& other) const {
        return rollNumber < other.rollNumber;
    }

    template<typename R1, typename C1>
    friend std::ostream& operator<<(std::ostream& os, const Student<R1, C1>& student);
};
template<typename R, typename C>
std::ostream& operator<<(std::ostream& os, const Student<R, C>& student) {
    os << "Student{name='" << student.name 
       << "', rollNumber=" << student.rollNumber
       << ", branch='" << student.branch
       << "', startingYear=" << student.startingYear
       << ", currentCourses={";
    
    bool first = true;
    for (const auto& pair : student.currentCourses) {
        if (!first) os << ", ";
        os << pair.first << ":" << pair.second;
        first = false;
    }
    os << "}, previousCourses={";
    
    first = true;
    for (const auto& pair : student.previousCourses) {
        if (!first) os << ", ";
        os << pair.first << ":" << pair.second;
        first = false;
    }
    os << "}}";
    return os;
}

#endif
