#include <iostream>
#include <memory>

//local includes
#include "core/PreProcessor.h"
#include "core/BoundariesExtractor.h"
#include "core/FeatureExtractor.h"


int main()
{

/*
    auto preProcessor = std::make_unique<PreProcessor>();
    preProcessor->SetDatasetPath("/home/oscar/data/biopsy/tiff/test/images/");
    preProcessor->SetOutputPath("/home/oscar/data/biopsy/tiff/test/clean_images/");
    preProcessor->Process(true);
    auto processedImages = preProcessor->GetOutputs();
*/

/*    auto boundaryExtractor  = std::make_unique<BoundariesExtractor>();
    boundaryExtractor->SetDatasetPath("/home/oscar/data/biopsy/tiff/test/clean_images/");
    boundaryExtractor->SetOutputPath("/home/oscar/data/biopsy/tiff/test/boundaries/");
    boundaryExtractor->SetThinBoundariesOn();
    boundaryExtractor->Process();
*/


    auto featureExtractor = std::make_unique<FeatureExtractor>();
    featureExtractor->SetDatasetPath("/home/oscar/data/biopsy/tiff/test/boundaries/");




    std::cout << "END" << std::endl;
    return 0;
}
