#ifndef BOUNDARIESEXTRACTOR_H
#define BOUNDARIESEXTRACTOR_H
//ITK includes
#include <itkImage.h>
#include <itkConnectedThresholdImageFilter.h>
#include <itkBinaryContourImageFilter.h>
#include <itkBinaryThinningImageFilter.h>

//SLT includes
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>


//local includes
#include "../util/InputOutput.h"
#include "../util/VTKViewer.h"


class BoundariesExtractor
{

private:
    using GrayImageT = itk::Image<unsigned, 2>;
    using GrayImageP = GrayImageT::Pointer;

    const unsigned Background = 0;
    const unsigned Foreground = 255;


public:
    BoundariesExtractor();

    void SetDatasetPath(const std::string& dataSetPath);
    void SetOutputPath (const std::string& outputPath);

    void Process(bool returnResults=false);

    GrayImageP ExtractBoundaries(GrayImageP binaryImage);
    GrayImageP ThinningBoundaries(GrayImageP boundaries);
    GrayImageP GrayToBinary(GrayImageP grayImage, bool show=false);

    void ConnectBackground(GrayImageP& grayImage);



private:
    //attributes
    std::string DatasetPath = ".";
    std::string OutputPath  = DatasetPath;

    bool ThinBoundaries = true;

    std::vector<GrayImageP> OutputImages;






};

#endif // BOUNDARIESEXTRACTOR_H
