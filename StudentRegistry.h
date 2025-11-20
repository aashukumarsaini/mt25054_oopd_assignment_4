#ifndef STUDENT_REGISTRY_H
#define STUDENT_REGISTRY_H

#include "Student.h"
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <algorithm>
#include <chrono>
#include <thread>
#include <mutex>
#include <iterator>

template<typename R, typename C>
class StudentRegistry {
private:
    std::vector<std::shared_ptr<Student<R, C>>> originalOrder;
    std::vector<std::shared_ptr<Student<R, C>>> sortedOrder;
    std::map<C, std::map<double, std::set<std::shared_ptr<Student<R, C>>>>> courseGradeIndex;
    mutable std::mutex registryMutex;

public:
    void addStudent(std::shared_ptr<Student<R, C>> student) {
        std::lock_guard<std::mutex> lock(registryMutex);
        
        originalOrder.push_back(student);
        sortedOrder.push_back(student);
        std::sort(sortedOrder.begin(), sortedOrder.end(),
                  [](const std::shared_ptr<Student<R, C>>& a,
                     const std::shared_ptr<Student<R, C>>& b) {
                      return *a < *b;
                  });
        
        for (const auto& coursePair : student->getPreviousCourses()) {
            courseGradeIndex[coursePair.first][coursePair.second].insert(student);
        }
        for (const auto& coursePair : student->getCurrentCourses()) {
            courseGradeIndex[coursePair.first][coursePair.second].insert(student);
        }
    }

    std::vector<std::shared_ptr<Student<R, C>>> getStudentsWithGrade(
            const C& courseCode, double minGrade) const {
        std::lock_guard<std::mutex> lock(registryMutex);
        
        std::set<std::shared_ptr<Student<R, C>>> resultSet;
        
        auto courseIt = courseGradeIndex.find(courseCode);
        if (courseIt == courseGradeIndex.end()) {
            return std::vector<std::shared_ptr<Student<R, C>>>();
        }
        
        for (auto gradeIt = courseIt->second.lower_bound(minGrade);
             gradeIt != courseIt->second.end(); ++gradeIt) {
            for (const auto& student : gradeIt->second) {
                resultSet.insert(student);
            }
        }
        
        std::vector<std::shared_ptr<Student<R, C>>> result(resultSet.begin(), resultSet.end());
        return result;
    }

    class OriginalOrderIterator {
        typename std::vector<std::shared_ptr<Student<R, C>>>::const_iterator it;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::shared_ptr<Student<R, C>>;
        using difference_type = std::ptrdiff_t;
        using pointer = std::shared_ptr<Student<R, C>>*;
        using reference = std::shared_ptr<Student<R, C>>;

        OriginalOrderIterator(typename std::vector<std::shared_ptr<Student<R, C>>>::const_iterator it)
            : it(it) {}

        OriginalOrderIterator& operator++() {
            ++it;
            return *this;
        }

        OriginalOrderIterator operator++(int) {
            OriginalOrderIterator tmp = *this;
            ++it;
            return tmp;
        }

        bool operator==(const OriginalOrderIterator& other) const {
            return it == other.it;
        }

        bool operator!=(const OriginalOrderIterator& other) const {
            return it != other.it;
        }

        std::shared_ptr<Student<R, C>> operator*() const {
            return *it;
        }

        std::shared_ptr<Student<R, C>> operator->() const {
            return *it;
        }
    };

    OriginalOrderIterator originalBegin() const {
        return OriginalOrderIterator(originalOrder.begin());
    }

    OriginalOrderIterator originalEnd() const {
        return OriginalOrderIterator(originalOrder.end());
    }

    class SortedOrderIterator {
        typename std::vector<std::shared_ptr<Student<R, C>>>::const_iterator it;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::shared_ptr<Student<R, C>>;
        using difference_type = std::ptrdiff_t;
        using pointer = std::shared_ptr<Student<R, C>>*;
        using reference = std::shared_ptr<Student<R, C>>;

        SortedOrderIterator(typename std::vector<std::shared_ptr<Student<R, C>>>::const_iterator it)
            : it(it) {}

        SortedOrderIterator& operator++() {
            ++it;
            return *this;
        }

        SortedOrderIterator operator++(int) {
            SortedOrderIterator tmp = *this;
            ++it;
            return tmp;
        }

        bool operator==(const SortedOrderIterator& other) const {
            return it == other.it;
        }

        bool operator!=(const SortedOrderIterator& other) const {
            return it != other.it;
        }

        std::shared_ptr<Student<R, C>> operator*() const {
            return *it;
        }

        std::shared_ptr<Student<R, C>> operator->() const {
            return *it;
        }
    };

    SortedOrderIterator sortedBegin() const {
        return SortedOrderIterator(sortedOrder.begin());
    }

    SortedOrderIterator sortedEnd() const {
        return SortedOrderIterator(sortedOrder.end());
    }

    size_t size() const {
        return originalOrder.size();
    }

    static void parallelSort(std::vector<std::shared_ptr<Student<R, C>>>& students,
                            int numThreads, 
                            std::vector<std::chrono::microseconds>& threadTimes) {
        if (students.empty()) return;
        
        size_t chunkSize = students.size() / numThreads;
        if (chunkSize == 0) chunkSize = 1;
        
        std::vector<std::thread> threads;
        std::vector<std::vector<std::shared_ptr<Student<R, C>>>> chunks(numThreads);
        threadTimes.resize(numThreads);
        
        for (int i = 0; i < numThreads; ++i) {
            size_t start = i * chunkSize;
            size_t end = (i == numThreads - 1) ? students.size() : (i + 1) * chunkSize;
            chunks[i].assign(students.begin() + start, students.begin() + end);
        }
        
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([&chunks, &threadTimes, i]() {
                auto startTime = std::chrono::high_resolution_clock::now();
                
                std::sort(chunks[i].begin(), chunks[i].end(),
                         [](const std::shared_ptr<Student<R, C>>& a,
                            const std::shared_ptr<Student<R, C>>& b) {
                             return *a < *b;
                         });
                
                auto endTime = std::chrono::high_resolution_clock::now();
                threadTimes[i] = std::chrono::duration_cast<std::chrono::microseconds>(
                    endTime - startTime);
            });
        }
        
        for (auto& thread : threads) {
            thread.join();
        }
        
        std::vector<std::shared_ptr<Student<R, C>>> result;
        std::vector<size_t> indices(numThreads, 0);
        
        while (result.size() < students.size()) {
            int bestChunk = -1;
            std::shared_ptr<Student<R, C>> bestStudent = nullptr;
            
            for (int i = 0; i < numThreads; ++i) {
                if (indices[i] < chunks[i].size()) {
                    if (bestChunk == -1 || *chunks[i][indices[i]] < *bestStudent) {
                        bestChunk = i;
                        bestStudent = chunks[i][indices[i]];
                    }
                }
            }
            
            if (bestChunk != -1) {
                result.push_back(bestStudent);
                indices[bestChunk]++;
            }
        }
        
        students = result;
    }
};

#endif
