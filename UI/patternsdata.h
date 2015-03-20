#ifndef PATTERNSDATA_H
#define PATTERNSDATA_H
#include <vector>
#include <QString>

class PatternsData
{
    public:
        PatternsData();
        static std::vector<QString> names_patterns;
        static std::vector<QString> descriptions;
        static std::vector<QString> images;
        static std::vector<QString> fillNamesPatterns();
        static std::vector<QString> fillNamesImages();
        static std::vector<QString> fillDescriptionPatterns();
};

#endif // PATTERNSDATA_H
