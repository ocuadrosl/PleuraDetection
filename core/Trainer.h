#ifndef TRAINER_H
#define TRAINER_H

#include <string>
#include <iostream>

#include <dlib/array2d.h>
#include <dlib/svm.h>

#include "../util/InputOutput.h"

class Trainer
{

    using FeaturesT  = dlib::matrix<double>;
    using FeaturesVectorT = std::vector<FeaturesT>;
    using KernelT = dlib::radial_basis_kernel<FeaturesT>;
    using DecisionFunctionT = dlib::decision_function<KernelT>;
    using FunctionT = dlib::normalized_function<DecisionFunctionT>;


public:
    Trainer();

    void ReadFeaturesCSV(const std::string& fileName,
                         unsigned featuresBeginIndex,
                         unsigned featuresEndIndex,
                         unsigned labelColumn);
    void ProcessKrr();
    void ProcessSVMRadial();
    void WriteLearnedFunction(const std::string& fileName);

private:

     FeaturesVectorT  FeaturesVector;
     FunctionT LearnedFunction;
     std::vector<double> Labels;


};

#endif // TRAINER_H
