#pragma once

#include <string>
#include <list>

class BlackList {
public:
    BlackList();
    void load();
    bool isInclude(std::string& entry);
    void append(std::string& entry);
private:
    std::list<std::string> m_blackList;
    void loadFile(std::string file);
};
