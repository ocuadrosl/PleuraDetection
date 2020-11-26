#include <iostream>
#include <memory>
#include <cinttypes>

//local includes
#include "core/PreProcessor.h"
#include "core/BoundariesExtractor.h"
#include "core/FeatureExtractor.h"
#include "core/Trainer.h"
#include "core/Tester.h"
#include "core/ShowPrediction.h"




int main()
{


    //using GrayImageT = itk::Image<unsigned, 2>;
    //using FractalFilterType = itk::StochasticFractalDimensionImageFilter < GrayImageT >;
    //FractalFilterType::Pointer fractalFilter = FractalFilterType::New();
    //fractalFilter->SetInput()
    //fractalFilter->


    /*
    PreProcessor preProcessor;
    preProcessor.SetLThreshold(98);
    preProcessor.SetBThreshold(-1);
    preProcessor.SetAThreshold(1);
    preProcessor.SetInputDatasetPath("/home/oscar/data/biopsy/tiff/test/images/");
    preProcessor.SetOutputDatasetPath("/home/oscar/data/biopsy/tiff/test/clean_images/");
    preProcessor.Process();
    */

/*
    BoundariesExtractor boundaryExtractor;
    boundaryExtractor.SetInputDatasetPath("/home/oscar/data/biopsy/tiff/test/clean_images/test");
    boundaryExtractor.SetOutputDatasetPath("/home/oscar/data/biopsy/tiff/test/boundaries/");
    boundaryExtractor.SetOutputMaskPath("/home/oscar/data/biopsy/tiff/test/masks/");
    boundaryExtractor.SetGaussSigma(0.5);
    boundaryExtractor.SetSmallComponentsThreshold(500);
    boundaryExtractor.SetThinBoundariesOff();
    boundaryExtractor.Process();
*/



   /* unsigned kernelSize = 50;
    std::string dataset = "train";
    FeatureExtractor featureExtractor;
    featureExtractor.SetBoundariesPath("/home/oscar/data/biopsy/tiff/test/boundaries/");
    featureExtractor.SetImagesPath("/home/oscar/data/biopsy/tiff/test/clean_images/"+dataset);
    featureExtractor.SetLabelsPath("/home/oscar/data/biopsy/tiff/test/labels/");
    featureExtractor.SetMasksPath("/home/oscar/data/biopsy/tiff/test/masks/");
    featureExtractor.SetPleuraMasksPath("/home/oscar/data/biopsy/tiff/test/pleura_masks_20/");
    featureExtractor.SetKernelSize(kernelSize);
    featureExtractor.Process();
    featureExtractor.WriteFeaturesCSV("/home/oscar/data/biopsy/tiff/test/csv/"+dataset+"_"+std::to_string(kernelSize)+".csv", true);
*/



    unsigned kernelSize = 250;
    ShowPrediction showPredictions;
    showPredictions.SetPleuraMaskPath("/home/oscar/data/biopsy/tiff/test/pleura_masks_20/");
    showPredictions.SetImagesPath("/home/oscar/data/biopsy/tiff/test/images");
    showPredictions.SetOutputPath("/home/oscar/data/biopsy/tiff/test/classifications_"+std::to_string(kernelSize));
    showPredictions.ReadCSV("/home/oscar/data/biopsy/tiff/test/csv/test_"+std::to_string(kernelSize)+"_classification.csv", 2, 3,4, 71, 72);
    showPredictions.SetKernelSize(kernelSize);
    showPredictions.WritePredictions();









/*

    Trainer trainer;
    trainer.ReadFeaturesCSV("/home/oscar/data/biopsy/tiff/test/csv/fractal_lbp_50.csv", 4, 63, 64);
    trainer.ProcessSVMRadial();
    //trainer.ProcessSVM();
    //trainer.WriteLearnedFunction("/home/oscar/data/biopsy/tiff/test/learned_function.dat");
*/

    /*
    auto tester = std::make_unique<Tester>();
    tester->ReadFeaturesCSV("/home/oscar/data/biopsy/tiff/test/tranning_set.csv", 1,2,3,6,7);
    tester->ReadLearnedFunction("/home/oscar/data/biopsy/tiff/test/learned_function.dat");
    */


    std::cout << "END" << std::endl;


    return 0;
}
