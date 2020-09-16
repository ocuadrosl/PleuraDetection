#include "Trainer.h"

Trainer::Trainer()
{

}

void Trainer::ReadFeaturesCSV(const std::string& fileName,
                              unsigned featuresBeginIndex,
                              unsigned featuresEndIndex,
                              unsigned labelColumn)
{

    std::ifstream file(fileName);

    unsigned featuresSize = featuresEndIndex - featuresBeginIndex+1;
    //std::cout<<featuresSize<<std::endl;
    FeaturesT features(featuresSize,1);
    std::string line;
    std::string value;
    std::istringstream isLine;

    if (file.is_open())
    {
        while (std::getline(file, line, '\n'))
        {
            isLine = std::istringstream(line);

            for (unsigned lIndex=1; std::getline(isLine, value, ','); ++lIndex)
            {

                if(lIndex == labelColumn)
                {
                    Labels.push_back(std::stod(value));
                    //std::cout<<*Labels.rbegin()<<std::endl;
                }
                else if (lIndex == featuresBeginIndex)
                {
                    features(0) = static_cast<double>(std::stold(value)); //verify this
                    for (unsigned fIndex=1; fIndex < featuresSize ; ++fIndex)
                    {
                        std::getline(isLine, value, ',');
                        features(fIndex) = std::stod(value);
                        ++lIndex;
                    }
                    FeaturesVector.push_back(features);
                    //std::cout<<*FeaturesVector.rbegin()<<std::endl;

                }
                else
                {
                    continue;
                }

            }

        }
    }

    file.close();
}



void Trainer::ProcessSVMRadial()
{

    dlib::vector_normalizer<FeaturesT> normalizer;
    normalizer.train(FeaturesVector);

    for (auto & features : FeaturesVector)
    {
        features = normalizer(features);
    }

    dlib::svm_c_trainer<KernelT> trainer;

    double  maxClassificationAccuracy = 0;
    for (double gamma = 0.00001; gamma <= 1; gamma *= 5)
    {
        for (double C = 1; C < 100000; C *= 5)
        {

            // tell the trainer the parameters we want to use
            trainer.set_kernel(KernelT(gamma));
            trainer.set_c(C);
            std::cout << "gamma: " << gamma << "    C: " << C;
            std::cout << " cross validation accuracy: " << cross_validate_trainer(trainer, FeaturesVector, Labels, 3);


        }
    }

}


void Trainer::ProcessKrr()
{

    dlib::vector_normalizer<FeaturesT> normalizer;
    normalizer.train(FeaturesVector);

    for (auto & features : FeaturesVector)
    {
        features = normalizer(features);
    }


    dlib::krr_trainer<KernelT> trainer;

    trainer.use_classification_loss_for_loo_cv();

    double  maxClassificationAccuracy = 0;
    for (double gamma = 0.000001; gamma <= 1; gamma *= 5)
    {
        // tell the trainer the parameters we want to use
        trainer.set_kernel(KernelT(gamma));

        // loo_values will contain the LOO predictions for each sample.  In the case
        // of perfect prediction it will end up being a copy of labels.
        std::vector<double> looValues;
        trainer.train(FeaturesVector, Labels, looValues);

        // Print gamma and the fraction of samples correctly classified during LOO cross-validation.
        const double classificationAccuracy = dlib::mean_sign_agreement(Labels, looValues);

        maxClassificationAccuracy = (maxClassificationAccuracy<classificationAccuracy)? classificationAccuracy:maxClassificationAccuracy;

        std::cout << "gamma: " << gamma << "     LOO accuracy: " << classificationAccuracy << std::endl;
    }


    trainer.set_kernel(KernelT(maxClassificationAccuracy));

    LearnedFunction.normalizer = normalizer;  // save normalization information
    LearnedFunction.function = trainer.train(FeaturesVector, Labels); // perform the actual training and save the results


    // print out the number of basis vectors in the resulting decision function
    //std::cout << "\nnumber of basis vectors in our learned_function is " << learned_function.function.basis_vectors.size() << std::endl;


    dlib::serialize("/home/oscar/function.dat")<<LearnedFunction;


    /*
    functT learned_function2;
    dlib::deserialize("/home/oscar/function.dat")>>learned_function2;

    std::cout << "\nnumber of basis vectors in our learned_function is " << learned_function2.function.basis_vectors.size() << std::endl;

    */
    io::printOK("Kernel Ridge Classification");

}


void Trainer::WriteLearnedFunction(const std::string& fileName)
{
    dlib::serialize(fileName)<<LearnedFunction;
}









