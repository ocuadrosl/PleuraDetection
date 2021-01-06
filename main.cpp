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

/*



*/


int main()
{

    std::string dataset = "dataset_1";

  /*
    //input folder images
    //output folder required "images_cleaned"
    PreProcessor preProcessor;
    preProcessor.SetLThreshold(98);
    preProcessor.SetBThreshold(-1);
    preProcessor.SetAThreshold(1);
    preProcessor.SetInputDatasetPath("/home/oscar/data/biopsy/tiff/"+dataset+"/images/");
    preProcessor.SetOutputDatasetPath("/home/oscar/data/biopsy/tiff/"+dataset+"/images_cleaned/");
    preProcessor.Process();
*/

/*
    //input folder image_cleaned
    //output folders required "boundaries" and "masks"
    BoundariesExtractor boundaryExtractor;
    boundaryExtractor.SetInputDatasetPath("/home/oscar/data/biopsy/tiff/"+dataset+"/images_cleaned");
    boundaryExtractor.SetOutputDatasetPath("/home/oscar/data/biopsy/tiff/"+dataset+"/boundaries/");
    boundaryExtractor.SetOutputMaskPath("/home/oscar/data/biopsy/tiff/"+dataset+"/masks/");
    boundaryExtractor.SetGaussSigma(0.5);
    boundaryExtractor.SetSmallComponentsThreshold(500);
    boundaryExtractor.SetThinBoundariesOff();
    boundaryExtractor.Process();
*/




    //input folders boundaries
    //images_cleaned
    //labels
    //masks
    //output folder boundary_masks/<erode radius size>
    //csv
    unsigned kernelSize = 50;
    unsigned erodeRadius = 20;
    std::string setType = "test";
    FeatureExtractor featureExtractor;
    featureExtractor.SetBoundariesPath("/home/oscar/data/biopsy/tiff/"+dataset+"/boundaries/");
    featureExtractor.SetImagesPath("/home/oscar/data/biopsy/tiff/"+dataset+"/images_cleaned/"+setType);
    featureExtractor.SetLabelsPath("/home/oscar/data/biopsy/tiff/"+dataset+"/labels/"); //use exiftool -all *.tiff to remove all metadata...
    featureExtractor.SetMasksPath("/home/oscar/data/biopsy/tiff/"+dataset+"/masks/");
    featureExtractor.SetPleuraMasksPath("/home/oscar/data/biopsy/tiff/"+dataset+"/boundary_masks/erode_radius_"+std::to_string(erodeRadius)+"/");
    featureExtractor.SetKernelSize(kernelSize);
    featureExtractor.SetErodeRadius(erodeRadius);
    featureExtractor.Process();
    featureExtractor.WriteFeaturesCSV("/home/oscar/data/biopsy/tiff/"+dataset+"/csv/kernel_size_"+std::to_string(kernelSize)+"_"+setType+".csv", true);



/*

    //input folders images,
    unsigned kernelSize = 250;
    ShowPrediction showPredictions;
    showPredictions.SetPleuraMaskPath("/home/oscar/data/biopsy/tiff/test/pleura_masks_20/");
    showPredictions.SetImagesPath("/home/oscar/data/biopsy/tiff/test/images");
    showPredictions.SetOutputPath("/home/oscar/data/biopsy/tiff/test/classifications_"+std::to_string(kernelSize));
    showPredictions.ReadCSV("/home/oscar/data/biopsy/tiff/test/csv/test_"+std::to_string(kernelSize)+"_classification.csv", 2, 3,4, 71, 72);
    showPredictions.SetKernelSize(kernelSize);
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
