# University ERP System

A comprehensive ERP system for managing university students with support for different data types, parallel processing, and efficient querying.

## Features

1. **Generic Student Class**: Template-based class that supports different data types for roll numbers (string, unsigned int, etc.) and course codes (string, int, etc.)

2. **IIIT-D and IIT-D Integration**: Handles both IIIT-D students (string course codes like 'OOPD', 'DSA') and IIT-D students (integer course codes like 101, 202)

3. **Current and Previous Courses**: Tracks both courses students are currently taking and courses they have completed with grades

4. **Parallel CSV Sorting**: Multi-threaded sorting of student records with performance timing for each thread

5. **Multiple Iterators**: Custom iterators for accessing students in original insertion order and sorted order (by roll number)

6. **Efficient Grade-Based Queries**: Fast lookup system using indexed data structures to find students with grades >= threshold in specific courses (O(log n) complexity)

## Building

```bash
make
```

## Running

```bash
make run
# or
./erp_system
```

The program provides an interactive menu to test different features:
- Option 1: Generic Student Class demonstration
- Option 2: IIIT-D with IIT-D Course Support
- Option 3: Parallel Sorting from CSV
- Option 4: Iterators (Original & Sorted Order)
- Option 5: Efficient Grade-Based Queries
- Option 6: Run All Parts

## Project Structure

- `Student.h`: Generic template class for students with support for different roll number and course code types
- `StudentRegistry.h`: Registry class with iterators, thread-safe operations, and efficient grade-based queries
- `CSVReader.h`: Utility for reading student data from CSV files (supports both string and integer course codes)
- `main.cpp`: Interactive demonstration program
- `Makefile`: Build configuration
- `students.csv`: Input CSV file (must exist before running)

## Requirements

- C++17 or later
- pthread library (for multi-threading)
- A CSV file named `students.csv` in the project directory

## CSV Format

The CSV file supports two formats:

### Format 1: Single Column (Backward Compatible)
```
Name,RollNumber,Branch,StartingYear,Courses
Student1,2020CS1001,CSE,2020,OOPD:9.5;DSA:8.5;AI:7.8
```
- All courses with grades > 0 are treated as previous courses
- Courses with grade 0.0 are treated as current courses

### Format 2: Two Columns (Recommended)
```
Name,RollNumber,Branch,StartingYear,CurrentCourses,PreviousCourses
Student1,2020CS1001,CSE,2020,OOPD:0;DSA:0,OOPD:9.5;DSA:8.5;AI:7.8
```
- **CurrentCourses**: Courses currently being taken (grade 0.0 or no grade)
- **PreviousCourses**: Completed courses with their grades

### Course Format
- Courses are separated by semicolons (`;`)
- Each course entry: `courseCode:grade`
- Example: `OOPD:9.5;DSA:8.5;AI:7.8`

### Supported Course Code Types
- **String course codes**: `OOPD`, `DSA`, `AI`, `ML`, etc. (for IIIT-D)
- **Integer course codes**: `101`, `202`, `303`, etc. (for IIT-D)

## Key Implementation Details

- **Thread Safety**: Uses mutex locks for thread-safe operations
- **Smart Pointers**: Uses `shared_ptr` to avoid data copying
- **Natural Sorting**: Roll numbers are sorted naturally (numeric strings sorted numerically, alphanumeric sorted lexicographically)
- **Efficient Indexing**: Uses nested map structure for O(log n) grade-based queries
- **Parallel Sorting**: Divides data into chunks, sorts in parallel, then merges results

## Example Usage

1. Ensure `students.csv` exists in the project directory
2. Build the project: `make`
3. Run: `./erp_system`
4. Select options from the menu to test different features
5. For grade queries (Option 5), enter course code and minimum grade when prompted
