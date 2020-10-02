#include <iostream>
#include <memory>
#include <cinttypes>

//local includes
#include "core/PreProcessor.h"
#include "core/BoundariesExtractor.h"
#include "core/FeatureExtractor.h"
#include "core/Trainer.h"
#include "core/Tester.h"


int main()
{

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
    boundaryExtractor.SetInputDatasetPath("/home/oscar/data/biopsy/tiff/test/clean_images/");
    boundaryExtractor.SetOutputDatasetPath("/home/oscar/data/biopsy/tiff/test/boundaries/");
    boundaryExtractor.SetGaussSigma(0.5);
    boundaryExtractor.SetSmallComponentsThreshold(500);
    boundaryExtractor.SetThinBoundariesOff();
    boundaryExtractor.Process();
*/


/*
    FeatureExtractor featureExtractor;
    featureExtractor.SetBoundariesPath("/home/oscar/data/biopsy/tiff/test/boundaries/");
    featureExtractor.SetImagesPath("/home/oscar/data/biopsy/tiff/test/clean_images/train");
    featureExtractor.SetLabelsPath("/home/oscar/data/biopsy/tiff/test/labels/");
    featureExtractor.SetKernelSize(100);
    featureExtractor.Process();
    featureExtractor.WriteFeaturesCSV("/home/oscar/data/biopsy/tiff/test/csv/fractal_shape_100.csv", true);
*/


    Trainer trainer;
    trainer.ReadFeaturesCSV("/home/oscar/data/biopsy/tiff/test/csv/fractal_shape_100.csv", 4, 7, 8);
    trainer.ProcessSVMRadial();
    //trainer.WriteLearnedFunction("/home/oscar/data/biopsy/tiff/test/learned_function.dat");



    /*
    auto tester = std::make_unique<Tester>();
    tester->ReadFeaturesCSV("/home/oscar/data/biopsy/tiff/test/tranning_set.csv", 1,2,3,6,7);
    tester->ReadLearnedFunction("/home/oscar/data/biopsy/tiff/test/learned_function.dat");
*/


    std::cout << "END" << std::endl;


    return 0;
}
