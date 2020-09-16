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
    auto preProcessor = std::make_unique<PreProcessor>();
    preProcessor->SetInputDatasetPath("/home/oscar/data/biopsy/tiff/test/images/");
    preProcessor->SetOutputDatasetPath("/home/oscar/data/biopsy/tiff/test/clean_images/");
    preProcessor->Process();
*/

/*
    auto boundaryExtractor  = std::make_unique<BoundariesExtractor>();
    boundaryExtractor->SetInputDatasetPath("/home/oscar/data/biopsy/tiff/test/clean_images/");
    boundaryExtractor->SetOutputDatasetPath("/home/oscar/data/biopsy/tiff/test/boundaries/");
    boundaryExtractor->SetThinBoundariesOn();
    boundaryExtractor->Process();

*/

/*
    auto featureExtractor = std::make_unique<FeatureExtractor>();
    featureExtractor->SetBoundariesPath("/home/oscar/data/biopsy/tiff/test/boundaries/");
    featureExtractor->SetImagesPath("/home/oscar/data/biopsy/tiff/test/clean_images/");
    featureExtractor->SetLabelsPath("/home/oscar/data/biopsy/tiff/test/labels/");
    featureExtractor->SetKernelSize(201);
    featureExtractor->ProcessForTrainning();
    featureExtractor->WriteFeaturesCSV("/home/oscar/data/biopsy/tiff/test/tranning_set.csv");
*/

/*
    auto trainer = std::make_unique<Trainer>();
    trainer->ReadFeaturesCSV("/home/oscar/data/biopsy/tiff/test/tranning_set.csv", 4, 7, 8);
    trainer->ProcessSVMRadial();
    //trainer->WriteLearnedFunction("/home/oscar/data/biopsy/tiff/test/learned_function.dat");
*/


/*
    auto tester = std::make_unique<Tester>();
    tester->ReadFeaturesCSV("/home/oscar/data/biopsy/tiff/test/tranning_set.csv", 1,2,3,6,7);
    tester->ReadLearnedFunction("/home/oscar/data/biopsy/tiff/test/learned_function.dat");
*/


    std::cout << "END" << std::endl;

    class A
    {
    public:
        int x{2};
        int y;
        A(const A& copyOfA) {std::cout<<"copy"<<std::endl;} ;
        A():x{3}{std::cout<<"create"<<std::endl;}
    };


    A c;
    c.x = 20;
    A b{c};

    std::cout<<b.y<<std::endl;

    return 0;
}
