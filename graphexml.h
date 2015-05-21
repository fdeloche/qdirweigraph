#ifndef GRAPHEXML_H
#define GRAPHEXML_H

#include "tinyxml.h"
#include "graphe.h"
#include <QString>


namespace GrapheXml{

template <typename T>
std::string num2str(const T& num, unsigned int prec = 12) {
    std::string ret;
    std::stringstream ss;
    std::ios_base::fmtflags ff = ss.flags();
    ff |= std::ios_base::floatfield;
    ff |= std::ios_base::fixed;
    ss.flags(ff);
    ss.precision(prec);
    ss << num;
    ret = ss.str();
    return ret;
};

template std::string num2str<float>(const float &num, unsigned int prec = 12);
template std::string num2str<double>(const double &num, unsigned int prec = 12);
template std::string num2str<int>(const int &num, unsigned int prec);



}

#endif // GRAPHEXML_H
