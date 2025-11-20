# University ERP System

A comprehensive ERP system for managing university students with support for different data types and efficient querying.

## Features

1. **Generic Student Class**: Supports different data types for roll numbers and course codes
2. **IIIT-D and IIT-D Integration**: Handles both string and integer course codes
3. **Parallel CSV Sorting**: Multi-threaded sorting with performance logging
4. **Multiple Iterators**: Original order and sorted order iteration
5. **Efficient Queries**: Fast grade-based lookups for placement queries

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

## Project Structure

- `Student.h`: Generic template class for students
- `StudentRegistry.h`: Registry with iterators and efficient querying
- `CSVReader.h`: Utility for reading student data from CSV
- `main.cpp`: Demonstration of all features
- `Makefile`: Build configuration

## Requirements

- C++17 or later
- pthread library (for multi-threading)

## CSV Format

The CSV file should have the following format:
```
Name,RollNumber,Branch,StartingYear,Courses
Student1,2020CS1001,CSE,2020,OOPD:9.5;DSA:8.5
```

Where Courses are in the format: `course1:grade1;course2:grade2`

