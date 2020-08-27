#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
//STL includes
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

//ITK includes
#include <itkImage.h>
#include <itkRGBPixel.h>
#include <itkMinimumMaximumImageCalculator.h>
#include <itkRGBToLuminanceImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkAdaptiveHistogramEqualizationImageFilter.h>

//local includes
#include "../util/InputOutput.h"
#include "../util/ColorConverterFilter.h"
#include "../util/VTKViewer.h"
#include "../util/ExtractChannelFilter.h"


class PreProcessor
{

private:
    //internal data types
    //suffix T=Type, P=Pointer

    using RGBPixelT = itk::RGBPixel<unsigned>;
    using RGBImageT = itk::Image<RGBPixelT,2>;
    using RGBImageP = RGBImageT::Pointer;

    using GrayImageT = itk::Image<unsigned, 2>;
    using GrayImageP = GrayImageT::Pointer;

    const unsigned Background = 0;
    const unsigned Foreground = 255;


public:

    PreProcessor();

    void SetDatasetPath(const std::string& dataSetPath);
    void SetOutputPath(const std::string& outputPath);

    void SetExtractForegroundArgs(float lThreshold, float aThreshold, float bThreshold);
    void SetHistogramEqualizationArgs(float alpha, float beta, unsigned radius);

    void Process(bool returnResults=false);

    std::vector<GrayImageP> GetOutputs();


private:

    //attributes
    std::string DatasetPath = ".";
    std::string OutputPath = DatasetPath;

    //background to white paramaters
    float LThreshold = 85;
    float AThreshold = 5;
    float BThreshold = 5;

    //Histogram equalization parameters
    float    Alpha  = 1.f;
    float    Beta   = 1.f;
    unsigned Radius = 5;


    std::vector<GrayImageP> OutputImages;



    //private methods
    RGBImageP  ExtractForeground(const RGBImageP& inputImage, bool show=false);
    GrayImageP HistogramEqualization(GrayImageP grayImage, bool show=false);



};

#endif // PREPROCESSOR_H
