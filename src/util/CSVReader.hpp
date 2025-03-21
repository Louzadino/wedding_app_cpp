#ifndef CSV_READER_H
#define CSV_READER_H

#include <string>
#include <vector>

using namespace std;

namespace util {

class CSVReader {
public:
    static vector<vector<string>> lerCSV(const string& filePath);
};

}

#endif
