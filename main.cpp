#include <iostream>
#include <memory>

//local includes
#include "core/PreProcessor.h"
#include "core/BoundariesExtractor.h"


int main()
{


  /*  auto preProcessor = std::make_unique<PreProcessor>();
    preProcessor->SetDatasetPath("/home/oscar/data/biopsy/tiff/test/images/");
    preProcessor->SetOutputPath("/home/oscar/data/biopsy/tiff/test/clean_images/");
    preProcessor->Process(true);
    auto processedImages = preProcessor->GetOutputs();
*/

    auto boundaryExtractor  = std::make_unique<BoundariesExtractor>();
    boundaryExtractor->SetDatasetPath("/home/oscar/data/biopsy/tiff/test/clean_images/");
    boundaryExtractor->Process();






    std::cout << "END" << std::endl;
    return 0;
}
