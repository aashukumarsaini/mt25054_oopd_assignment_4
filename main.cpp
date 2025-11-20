#include "Student.h"
#include "StudentRegistry.h"
#include "CSVReader.h"
#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <iomanip>
#include <fstream>

using namespace std;

using IIITDStudent = Student<string, string>;
using IITDStudent = Student<string, int>;
void demonstratePart1() {
    cout << "\n--- Part 1: Generic Student Class (from CSV) ---\n";
    
    auto students = CSVReader::readStudentsStringString("students.csv");
    
    if (students.empty()) {
        cout << "Error: Couldn't read students from CSV\n";
        return;
    }
    
    cout << "Showing first 3 students from CSV:\n\n";
    
    for (int i = 0; i < min(3, (int)students.size()); ++i) {
        cout << "Student " << (i+1) << ":\n";
        cout << *students[i] << endl << endl;
    }
    
    cout << "Total students in CSV: " << students.size() << endl;
    cout << "\nNote: This demonstrates the generic Student class working with\n";
    cout << "string roll numbers and string course codes from CSV data.\n";
}

void demonstratePart2() {
    cout << "\n--- Part 2: IIIT-D students with IIT-D course support ---\n";
    
    auto students = CSVReader::readStudentsStringString("students.csv");
    
    if (students.empty()) {
        cout << "Error: Couldn't read students from CSV\n";
        return;
    }
    
    cout << "Showing IIIT-D students (string course codes) from CSV:\n\n";
    
    int count = 0;
    for (const auto& student : students) {
        if (count < 3) {
            cout << "Student " << (count+1) << ":\n";
            cout << *student << endl << endl;
            count++;
        } else {
            break;
        }
    }
    
    cout << "Note: IIIT-D students use string course codes (like 'OOPD', 'DSA').\n";
    cout << "These students can also take IIT-D courses which use integer course codes.\n";
    cout << "To demonstrate IIT-D courses, you would use Student<string, int> type.\n";
}

void demonstratePart3() {
    cout << "\n--- Part 3: Parallel sorting from CSV ---\n";
    
    cout << "Reading students from CSV file...\n";
    auto students = CSVReader::readStudentsStringString("students.csv");
    
    if (students.empty()) {
        cout << "Error: Couldn't read students from CSV\n";
        return;
    }
    
    cout << "Read " << students.size() << " students\n";
    
    auto studentsToSort = students;
    int numThreads = 2;
    vector<chrono::milliseconds> threadTimes;
    
    cout << "Sorting with " << numThreads << " threads...\n";
    auto startTime = chrono::high_resolution_clock::now();
    StudentRegistry<string, string>::parallelSort(studentsToSort, numThreads, threadTimes);
    auto endTime = chrono::high_resolution_clock::now();
    
    auto totalTime = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
    
    cout << "\nSorting results:\n";
    for (size_t i = 0; i < threadTimes.size(); ++i) {
        cout << "Thread " << i << ": " << threadTimes[i].count() << " ms\n";
    }
    cout << "Total time: " << totalTime.count() << " ms\n";
    
    bool isSorted = true;
    for (size_t i = 1; i < studentsToSort.size(); ++i) {
        if (*studentsToSort[i] < *studentsToSort[i-1]) {
            isSorted = false;
            break;
        }
    }
    cout << "Verification: " << (isSorted ? "PASSED" : "FAILED") << endl;
    
    cout << "\nFirst 5 (sorted):\n";
    for (size_t i = 0; i < min(static_cast<size_t>(5), studentsToSort.size()); ++i) {
        cout << "  " << studentsToSort[i]->getRollNumber() 
             << " - " << studentsToSort[i]->getName() << endl;
    }
    if (studentsToSort.size() > 5) {
        cout << "...\n";
        cout << "Last 5 (sorted):\n";
        for (size_t i = studentsToSort.size() - 5; i < studentsToSort.size(); ++i) {
            cout << "  " << studentsToSort[i]->getRollNumber() 
                 << " - " << studentsToSort[i]->getName() << endl;
        }
    }
}

void demonstratePart4() {
    cout << "\n--- Part 4: Iterators for original and sorted order ---\n";
    
    auto students = CSVReader::readStudentsStringString("students.csv");
    
    if (students.empty()) {
        cout << "Error: Couldn't read students\n";
        return;
    }
    
    StudentRegistry<string, string> registry;
    
    cout << "Adding " << students.size() << " students to registry...\n";
    for (const auto& student : students) {
        registry.addStudent(student);
    }
    
    cout << "\nOriginal order (first 10):\n";
    int count = 0;
    for (auto it = registry.originalBegin(); it != registry.originalEnd() && count < 10; ++it, ++count) {
        cout << "  " << (*it)->getRollNumber() << " - " << (*it)->getName() << endl;
    }
    if (registry.size() > 10) {
        cout << "  ... (showing first 10 of " << registry.size() << ")\n";
    }
    
    cout << "\nSorted order (first 10):\n";
    count = 0;
    for (auto it = registry.sortedBegin(); it != registry.sortedEnd() && count < 10; ++it, ++count) {
        cout << "  " << (*it)->getRollNumber() << " - " << (*it)->getName() << endl;
    }
    if (registry.size() > 10) {
        cout << "  ... (showing first 10 of " << registry.size() << ")\n";
    }
}

void demonstratePart5() {
    cout << "\n--- Part 5: Finding students with high grades ---\n";
    
    auto students = CSVReader::readStudentsStringString("students.csv");
    
    if (students.empty()) {
        cout << "Error: Couldn't read students\n";
        return;
    }
    
    StudentRegistry<string, string> registry;
    
    cout << "Adding " << students.size() << " students to registry...\n";
    for (const auto& student : students) {
        registry.addStudent(student);
    }
    
    cout << "\nFinding students with grade >= 9.0 in OOPD:\n";
    auto qualifiedStudents = registry.getStudentsWithGrade("OOPD", 9.0);
    
    cout << "Found " << qualifiedStudents.size() << " students:\n";
    int count = 0;
    for (const auto& student : qualifiedStudents) {
        if (count < 10) {
            double grade = student->getGrade("OOPD");
            cout << "  " << student->getName() << " (" << student->getRollNumber() 
                 << ") - Grade: " << fixed << setprecision(1) << grade << endl;
            count++;
        } else {
            break;
        }
    }
    if (qualifiedStudents.size() > 10) {
        cout << "  ... (showing first 10 of " << qualifiedStudents.size() << ")\n";
    }
    
    cout << "\nFinding students with grade >= 9.0 in DSA:\n";
    qualifiedStudents = registry.getStudentsWithGrade("DSA", 9.0);
    
    cout << "Found " << qualifiedStudents.size() << " students:\n";
    count = 0;
    for (const auto& student : qualifiedStudents) {
        if (count < 10) {
            double grade = student->getGrade("DSA");
            cout << "  " << student->getName() << " (" << student->getRollNumber() 
                 << ") - Grade: " << fixed << setprecision(1) << grade << endl;
            count++;
        } else {
            break;
        }
    }
    if (qualifiedStudents.size() > 10) {
        cout << "  ... (showing first 10 of " << qualifiedStudents.size() << ")\n";
    }
}

void createSampleCSV() {
    ofstream file("students.csv");
    if (!file.is_open()) {
        cerr << "Error creating CSV file\n";
        return;
    }
    
    file << "Name,RollNumber,Branch,StartingYear,Courses\n";
    
    for (int i = 0; i < 3000; ++i) {
        string name = "Student" + to_string(i);
        string roll = "2020CS" + to_string(1000 + i);
        string branch = (i % 3 == 0) ? "CSE" : (i % 3 == 1) ? "ECE" : "EEE";
        int year = 2020 + (i % 4);
        double grade1 = 7.0 + (i % 31) * 0.1;
        double grade2 = 7.0 + ((i * 3) % 31) * 0.1;
        
        file << name << "," << roll << "," << branch << "," << year 
             << ",OOPD:" << fixed << setprecision(1) << grade1 
             << ";DSA:" << grade2 << "\n";
    }
    
    file.close();
    cout << "Created students.csv with 3000 records\n";
}

int main() {
    cout << "University ERP System\n";
    cout << "====================\n";
    
    cout << "\nCreating sample CSV file...\n";
    createSampleCSV();
    
    int choice;
    while (true) {
        cout << "\n========================================\n";
        cout << "Select an option:\n";
        cout << "1. Part 1: Generic Student Class\n";
        cout << "2. Part 2: IIIT-D with IIT-D Course Support\n";
        cout << "3. Part 3: Parallel Sorting from CSV\n";
        cout << "4. Part 4: Iterators (Original & Sorted Order)\n";
        cout << "5. Part 5: Efficient Grade-Based Queries\n";
        cout << "6. Run All Parts\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        
        cin >> choice;
        
        if (choice == 0) {
            cout << "Exiting...\n";
            break;
        } else if (choice == 1) {
            demonstratePart1();
        } else if (choice == 2) {
            demonstratePart2();
        } else if (choice == 3) {
            demonstratePart3();
        } else if (choice == 4) {
            demonstratePart4();
        } else if (choice == 5) {
            demonstratePart5();
        } else if (choice == 6) {
            demonstratePart1();
            demonstratePart2();
            demonstratePart3();
            demonstratePart4();
            demonstratePart5();
        } else {
            cout << "Invalid choice! Please try again.\n";
        }
    }
    
    return 0;
}
