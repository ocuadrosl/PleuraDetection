#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H
//SLT includes
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>


//local includes
#include "../util/InputOutput.h"
#include "../util/VTKViewer.h"


class FeatureExtractor
{

private:
    using GrayImageT = itk::Image<unsigned, 2>;
    using GrayImageP = GrayImageT::Pointer;

    const unsigned Background = 0;
    const unsigned Foreground = 255;

public:



    void SetDatasetPath(const std::string& dataSetPath);
    FeatureExtractor();

    void Process();

private:

    std::string DatasetPath;


};

#endif // FEATUREEXTRACTOR_H
