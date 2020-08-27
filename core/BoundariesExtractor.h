#ifndef BOUNDARIESEXTRACTOR_H
#define BOUNDARIESEXTRACTOR_H

//ITK includes
#include <itkImage.h>
#include <itkConnectedThresholdImageFilter.h>
#include <itkBinaryContourImageFilter.h>
#include <itkBinaryThinningImageFilter.h>
#include <itkConnectedComponentImageFilter.h>
#include <itkLabelImageToShapeLabelMapFilter.h>
#include <itkRGBPixel.h>
#include <itkLabelMapToRGBImageFilter.h>
#include <itkRGBToLuminanceImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>

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

    //shape label map
    using LabelType = unsigned;
    using ShapeLabelObjectType = itk::ShapeLabelObject<LabelType, 2>;
    using LabelMapT = itk::LabelMap<ShapeLabelObjectType>;
    using LabelMapP = LabelMapT::Pointer;


public:
    BoundariesExtractor();

    void SetDatasetPath(const std::string& dataSetPath);
    void SetOutputPath (const std::string& outputPath);

    void SetThinBoundariesOn();
    void SetThinBoundariesOff();

    void Process(bool returnResults=false);

    GrayImageP ExtractBoundaries(GrayImageP binaryImage);
    GrayImageP ThinningBoundaries(GrayImageP boundaries);


    void ConnectBackground(GrayImageP& grayImage);



private:
    //attributes
    std::string DatasetPath = ".";
    std::string OutputPath  = DatasetPath;

    bool ThinBoundaries = false;

    unsigned ComponentsThreshold = 50;

    std::vector<GrayImageP> OutputImages;

    GrayImageP GrayToBinary(GrayImageP grayImage, bool show=false);
    GrayImageP DeleteSmallComponents(GrayImageP edgesImage);

    GrayImageP LabelMapToBinaryImage(const LabelMapP& labelMap);

};

#endif // BOUNDARIESEXTRACTOR_H
