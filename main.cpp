#include "Student.h"
#include "StudentRegistry.h"
#include "CSVReader.h"
#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <iomanip>
#include <limits>

using namespace std;

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
    
    auto iitdStudents = CSVReader::readStudentsStringString("students.csv");
    
    if (iitdStudents.empty()) {
        cout << "Error: Couldn't read students from CSV\n";
        return;
    }
    
    cout << "IIIT-D students (using string course codes like 'OOPD', 'DSA'):\n\n";
    
    int count = 0;
    for (const auto& student : iitdStudents) {
        if (count < 2) {
            cout << "Student " << (count+1) << ":\n";
            cout << *student << endl << endl;
            count++;
        } else {
            break;
        }
    }
    
    cout << "\n--- Creating IIT-D student records (integer course codes) ---\n";
    
    auto iitdStudent1 = make_shared<Student<string, int>>(
        "Rajesh Kumar", "2020CS1001", "CSE", 2020);
    iitdStudent1->addPreviousCourse(101, 9.5);
    iitdStudent1->addPreviousCourse(202, 8.8);
    iitdStudent1->addCurrentCourse(303, 0.0);
    
    cout << "IIT-D Student (integer course codes):\n";
    cout << *iitdStudent1 << endl << endl;
    
    cout << "Note: The generic Student class supports both:\n";
    cout << "  - Student<string, string> for IIIT-D courses (e.g., 'OOPD', 'DSA')\n";
    cout << "  - Student<string, int> for IIT-D courses (e.g., 101, 202, 303)\n";
    cout << "This demonstrates that IIIT-D students can take IIT-D courses\n";
    cout << "by using the appropriate template instantiation.\n";
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
    vector<chrono::microseconds> threadTimes;
    
    cout << "Sorting with " << numThreads << " threads...\n";
    auto startTime = chrono::high_resolution_clock::now();
    StudentRegistry<string, string>::parallelSort(studentsToSort, numThreads, threadTimes);
    auto endTime = chrono::high_resolution_clock::now();
    
    auto totalTime = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
    
    cout << "\nSorting results:\n";
    for (size_t i = 0; i < threadTimes.size(); ++i) {
        double ms = threadTimes[i].count() / 1000.0;
        cout << "Thread " << i << ": " << threadTimes[i].count() << " μs (" 
             << fixed << setprecision(3) << ms << " ms)\n";
    }
    double totalMs = totalTime.count() / 1000.0;
    cout << "Total time: " << totalTime.count() << " μs (" 
         << fixed << setprecision(3) << totalMs << " ms)\n";
    
    bool isSorted = true;
    for (size_t i = 1; i < studentsToSort.size(); ++i) {
        if (*studentsToSort[i] < *studentsToSort[i-1]) {
            isSorted = false;
            break;
        }
    }
    cout << "Verification: " << (isSorted ? "PASSED" : "FAILED") << endl;
    
    cout << "\nAll students (sorted by roll number):\n";
    for (size_t i = 0; i < studentsToSort.size(); ++i) {
        cout << "  " << studentsToSort[i]->getRollNumber() 
             << " - " << studentsToSort[i]->getName() << endl;
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
    
    int orderChoice;
    bool validChoice = false;
    
    while (!validChoice) {
        cout << "\nSelect order to display:\n";
        cout << "1. Original order (as entered)\n";
        cout << "2. Sorted order (by roll number)\n";
        cout << "Enter your choice (1 or 2): ";
        
        string input;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, input);
        
        if (cin.fail()) {
            cout << "Error: Input stream error. Please try again.\n";
            cin.clear();
            continue;
        }
        
        try {
            orderChoice = stoi(input);
            if (orderChoice == 1 || orderChoice == 2) {
                validChoice = true;
            } else {
                cout << "Error: Please enter 1 or 2.\n";
            }
        } catch (const std::invalid_argument&) {
            cout << "Error: Invalid input. Please enter 1 or 2.\n";
        } catch (const std::out_of_range&) {
            cout << "Error: Input out of range. Please enter 1 or 2.\n";
        }
    }
    
    if (orderChoice == 1) {
        cout << "\nOriginal order (as entered):\n";
        for (auto it = registry.originalBegin(); it != registry.originalEnd(); ++it) {
            cout << "  " << (*it)->getRollNumber() << " - " << (*it)->getName() << endl;
        }
    } else {
        cout << "\nSorted order (by roll number):\n";
        for (auto it = registry.sortedBegin(); it != registry.sortedEnd(); ++it) {
            cout << "  " << (*it)->getRollNumber() << " - " << (*it)->getName() << endl;
        }
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
    
    string courseCode;
    double minGrade;
    
    cout << "\nEnter course code (e.g., OOPD, DSA): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, courseCode);
    
    courseCode.erase(0, courseCode.find_first_not_of(" \t\n\r"));
    courseCode.erase(courseCode.find_last_not_of(" \t\n\r") + 1);
    
    if (courseCode.empty()) {
        cout << "Error: Course code cannot be empty\n";
        return;
    }
    
    bool validGrade = false;
    while (!validGrade) {
        cout << "Enter minimum grade threshold (e.g., 9.0): ";
        
        string gradeInput;
        getline(cin, gradeInput);
        
        if (cin.fail()) {
            cout << "Error: Input stream error. Please try again.\n";
            cin.clear();
            continue;
        }
        
        try {
            size_t pos;
            minGrade = stod(gradeInput, &pos);
            
            string remaining = gradeInput.substr(pos);
            remaining.erase(0, remaining.find_first_not_of(" \t\n\r"));
            
            if (!remaining.empty()) {
                cout << "Error: Invalid grade input. Please enter a valid number (e.g., 9.0).\n";
                continue;
            }
            
            if (minGrade < 0.0 || minGrade > 10.0) {
                cout << "Warning: Grade is outside typical range (0-10). Continuing anyway...\n";
            }
            
            validGrade = true;
        } catch (const std::invalid_argument&) {
            cout << "Error: Invalid grade input. Please enter a valid number (e.g., 9.0).\n";
        } catch (const std::out_of_range&) {
            cout << "Error: Grade value is out of range. Please enter a valid number.\n";
        }
    }
    
    cout << "\nFinding students with grade >= " << fixed << setprecision(1) << minGrade 
         << " in " << courseCode << ":\n";
    auto qualifiedStudents = registry.getStudentsWithGrade(courseCode, minGrade);
    
    cout << "Found " << qualifiedStudents.size() << " students:\n";
    if (qualifiedStudents.empty()) {
        cout << "  No students found with the specified criteria.\n";
    } else {
        for (const auto& student : qualifiedStudents) {
            double grade = student->getGrade(courseCode);
            cout << "  " << student->getName() << " (" << student->getRollNumber() 
                 << ") - Grade: " << fixed << setprecision(1) << grade << endl;
        }
    }
}

int main() {
    cout << "University ERP System\n";

    int choice;
    while (true) {
        cout << "\n========================================\n";
        cout << "Select an option:\n";
        cout << "1. Generic Student Class\n";
        cout << "2. IIIT-D with IIT-D Course Support\n";
        cout << "3. Parallel Sorting from CSV\n";
        cout << "4. Iterators (Original & Sorted Order)\n";
        cout << "5. Efficient Grade-Based Queries\n";
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
