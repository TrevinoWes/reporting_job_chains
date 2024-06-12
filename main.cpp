#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <chrono>
#include <format> // !!! not support by clang?

/*
Assumptions:
    If any line is malformed entire input is malformed
    starting job is not associated to any other chain
*/

// !!! switch from int to size_t or unint32_t?


class Job {
public:
    int id;
    std::chrono::seconds runtime;
    int nextId;
    
    Job(): id(0), runtime(0), nextId(0){};
    Job(const int& i, const std::chrono::seconds& r, const int& n): id(i), runtime(r), nextId(n){};
};

class Report {
    int startId;
    int endId;
    int jobCount;
    std::chrono::seconds runtime;
    std::chrono::seconds average;

    static std::string formatTime(const std::chrono::seconds& time) {
        return std::format("{:%T}", time);
    }

public:
    Report(const int& sId, const int& eId, const int& jCount,
           const std::chrono::seconds& rtime): startId(sId), endId(eId),
                                               jobCount(jCount), runtime(rtime) {
        average = rtime / jobCount;                                         
    }
    
    friend std::ostream& operator<<(std::ostream& stream, const Report& report);
};

std::ostream& operator<<(std::ostream& stream, const Report& report){
        stream << "-\n"; // make const text;
        stream << "start_job: " << report.startId << "\n";
        stream << "last_job: " << report.endId << "\n";
        stream << "number_of_jobs: " << report.jobCount << "\n";
        stream << "job_chain_runtime: " << Report::formatTime(report.runtime) << "\n";
        stream << "average_job_time: " << Report::formatTime(report.average) << std::endl;
}

enum class Value {
    job_id,
    runtime_in_seconds,
    next_job_id
};

void parseNextVal(std::string::const_iterator& iter, const std::string::const_iterator& end,
                  const char& delimiter, std::string& val) {
    while(iter != end && *iter != delimiter) {
        val.push_back(*iter);
        ++iter;
    }
    
    ++iter;
}

bool parseHeader(const std::map<std::string, Value>& valueMap, const std::string& header) {
    auto iter = header.begin();
    std::string val;
    
    for(int i = 0; i < valueMap.size(); ++i) {
        parseNextVal(iter, header.end(), ',', val);
        
        auto valueIter = valueMap.find(val);
        if(valueIter == valueMap.end()) {
            return false;
        }
    }
    
    // map header to position
}

bool validateJob(const Job& job) {
    
    if(job.id == 0) {
        return false;
    }
    
    if(job.runtime == std::chrono::seconds::zero()) {
        return false;
    }
    
    return true;
}

Job parseInput(const std::string& jobStr) {
    Job job;
    
    // using mapped positions parse then create job object
    
    if(!validateJob(job)) {
        // report error
    }
    
    return job;
}

using namespace std;
int main() {
    std::map<int, Job> jobInfo;
    std::set<int> processedIDs;
    std::set<Report> reportSet;
    std::queue<int> jobQueue;
    std::map<std::string, Value> valueMap{{"job_id", Value::job_id},
                                          {"runtime_in_seconds", Value::runtime_in_seconds},
                                          {"next_job_id", Value::next_job_id}};
    
    std::string request;
    Job job;
    jobInfo[job.id] = job; // Dummy job
    
    std::getline(std::cin, request);
    if(parseHeader(valueMap, request)) {
        std::cout << "Malformed Input" << std::endl; // create constant text for errors
        return 1;
    }
    
    
    while(std::getline(std::cin, request)) {
        job = parseInput(request);
        jobQueue.push(job.id);
        jobInfo[job.id] = job;
    }
    
    std::chrono::seconds runtime;
    int jobCount, startId, endId;
    while(!jobQueue.empty()) {
        job = jobInfo[jobQueue.front()];
        jobQueue.pop();
        if(processedIDs.find(job.id) != processedIDs.end()) {
            continue;
        }
        
        startId = job.id;
        runtime.zero();
        jobCount = 0;
        while(job.id != 0) {
            runtime += job.runtime;
            ++jobCount;
            processedIDs.insert(job.id);
            job = jobInfo[job.nextId];
            if(job.nextId == 0) {
                endId = job.id;
            }
        }
        reportSet.insert(Report(startId, endId, runtime, jobCount));
    }
    
    for(auto reportIter = reportSet.rbegin(); reportIter != reportSet.rend(); ++reportIter) {
        std::cout << *reportIter;
    }
    
    std::cout << "-\n";
    
    return 0;
}