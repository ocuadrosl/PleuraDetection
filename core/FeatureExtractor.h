#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H
//SLT includes
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <cinttypes>


//local includes
#include "../util/InputOutput.h"
#include "../util/VTKViewer.h"
#include "../util/ImageOperations.h"
#include "../util/FractalDimensionCalculator.h"

//itk includes
#include <itkImage.h>
#include <itkRGBPixel.h>
#include <itkNeighborhoodIterator.h>
#include <itkScalarImageToCooccurrenceMatrixFilter.h>
#include <itkHistogramToTextureFeaturesFilter.h>
#include <itkImageDuplicator.h>
#include <itkConnectedComponentImageFilter.h>
#include <itkLabelImageToShapeLabelMapFilter.h>
#include <itkRelabelComponentImageFilter.h>


#include <itkLabelMapToRGBImageFilter.h>

//Dlib includes
#include <dlib/array2d.h>
#include <dlib/image_transforms.h>
#include <dlib/gui_widgets.h>

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

    const double Pleura    =  1;
    const double NonPleura = -1;

public:


    void SetBoundariesPath(const std::string& boundariesPath);
    void SetImagesPath(const std::string& imagesPath);
    void SetLabelsPath(const std::string& labelsPath);
    void SetKernelSize(const std::size_t& kernelSize);
    FeatureExtractor();

    void Process();

    void WriteFeaturesCSV(const std::string& fileName, bool writeHeader=false) const;

private:

    //for fast implementation, I assume that boundaries and input images have same names
    std::string BoundariesPath = ".";
    std::string ImagesPath = "."; //Original Images
    std::string LabelsPath = ".";
    std::size_t KernelSize{51};

    CentersT   CentersVector;
    FeaturesVectorT  FeaturesVector;
    std::vector<double> LabelsVector;
    std::vector<std::string> ImagesNames;
    std::vector<std::uint16_t> CentersNumberPerImage;

    void FindCenters(GrayImageP boundaries, CentersT& centers);

    void FindLabels(const RGBImageP& labelsImage, const CentersT& centersVector, std::vector<double>& labelsVector);

    void ComputeLBPFeatures(GrayImageP grayImage, const CentersT& centers, FeaturesVectorT& featuresVector);
    void ComputeFractalDimension(const GrayImageP &, const CentersT& centers, FeaturesVectorT& featuresVector);
    void ComputeCooccurrenceMatrixFeatures(GrayImageP grayImage, const CentersT& centers, FeaturesVectorT& featuresVector);
    void ComputeShapeFeatures(GrayImageP boundaries, const CentersT& centers, FeaturesVectorT& featuresVector);
    void ComputeShapeAndFractalFeatures(GrayImageP grayImage, GrayImageP boundaries, const CentersT& centers, FeaturesVectorT& featuresVector);






};

#endif // FEATUREEXTRACTOR_H
