#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

namespace loader
{

std::unordered_map<int, std::unordered_map<int, int>>
loadGraphFile(const std::string& path)
{
    std::unordered_map<int, std::unordered_map<int, int>> g;

    std::ifstream infile(path);
    std::string line;
    while(std::getline(infile, line))
    {
        if(line.front() != 'a') continue;

        std::istringstream iss(line);
        char _;
        int src, dst, w;
        if(!(iss >> _ >> src >> dst >> w)) break;

        g[src - 1][dst - 1] = w;
    }

    return g;
}

}
