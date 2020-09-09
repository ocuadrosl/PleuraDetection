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
#include "../util/ImageOperations.h"

//itk includes
#include <itkImage.h>
#include <itkRGBPixel.h>
#include <itkNeighborhoodIterator.h>
#include <itkScalarImageToCooccurrenceMatrixFilter.h>
#include <itkHistogramToTextureFeaturesFilter.h>

//Dlib includes
#include <dlib/array2d.h>

class FeatureExtractor
{

private:

    using RGBImageT = itk::Image<itk::RGBPixel<unsigned char>, 2>;
    using RGBImageP = RGBImageT::Pointer;

    using GrayImageT = itk::Image<unsigned, 2>;
    using GrayImageP = GrayImageT::Pointer;

    const unsigned Background = 0;
    const unsigned Foreground = 255;

    using FeaturesT  = dlib::matrix<double>;
    using FeaturesVectorT = std::vector<FeaturesT>;
    using CentersT  = std::vector<GrayImageT::IndexType>;

    const unsigned Pleura    = 1;
    const unsigned NonPleura = 2;



public:


    void SetBoundariesPath(const std::string& boundariesPath);
    void SetImagesPath(const std::string& imagesPath);
    void SetLabelsPath(const std::string& labelsPath);
    FeatureExtractor();

    void ProcessForTrainning();

private:

    //for fast implementation, I assume that boundaries and input images have same names
    std::string BoundariesPath = ".";
    std::string ImagesPath = "."; //Original Images
    std::string LabelsPath = ".";
    unsigned KernelSize = 51;

    CentersT   CentersVector;
    FeaturesVectorT  FeaturesVector;
    std::vector<double> LabelsVector;

    void FindCenters(GrayImageP boundaries, CentersT& centers);
    void ComputeCooccurrenceMatrixFeatures(GrayImageP grayImage, const CentersT& centers, FeaturesVectorT& featuresVector);
    void FindLabels(const RGBImageP& labelsImage, const CentersT& centersVector, std::vector<double>& labelsVector);

    void WriteFeaturesCSV(const std::string& fileName, bool writeHeader=false) const;

};

#endif // FEATUREEXTRACTOR_H
