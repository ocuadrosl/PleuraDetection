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



    FeatureExtractor featureExtractor;
    featureExtractor.SetBoundariesPath("/home/oscar/data/biopsy/tiff/test/boundaries/");
    featureExtractor.SetImagesPath("/home/oscar/data/biopsy/tiff/test/clean_images");
    featureExtractor.SetLabelsPath("/home/oscar/data/biopsy/tiff/test/labels/");
    featureExtractor.SetMasksPath("/home/oscar/data/biopsy/tiff/test/masks/");
    featureExtractor.SetPleuraMasksPath("/home/oscar/data/biopsy/tiff/test/pleura_masks_150/");
    featureExtractor.SetKernelSize(150);
    featureExtractor.Process();
    featureExtractor.WriteFeaturesCSV("/home/oscar/data/biopsy/tiff/test/csv/fractal_lbp_coo_mome_150.csv", true);




/*
    ShowPrediction showPredictions;
    showPredictions.SetPleuraMaskPath("/home/oscar/data/biopsy/tiff/test/pleura_masks_200/");
    showPredictions.SetImagesPath("/home/oscar/data/biopsy/tiff/test/images");
    showPredictions.SetOutputPath("/home/oscar/data/biopsy/tiff/test/classifications_200");
    showPredictions.ReadCSV("/home/oscar/data/biopsy/tiff/test/csv/fractal_lbp_coo_mome_200_prediction.csv",
                          2, 3,4, 71, 72);
    showPredictions.SetKernelSize(200);
    showPredictions.WritePredictions();

*/







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
