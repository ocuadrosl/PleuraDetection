#include "BoundariesExtractor.h"

BoundariesExtractor::BoundariesExtractor()
{

}

void BoundariesExtractor::SetDatasetPath(const std::string& dataSetPath)
{
    DatasetPath = (*dataSetPath.rbegin() == '/') ? dataSetPath.substr(0, dataSetPath.length()-1) : dataSetPath;
}

void BoundariesExtractor::SetOutputPath(const std::string& outputPath)
{
    //delete '/' at the end of the path
    OutputPath = ( (*outputPath.rbegin()) == '/') ? outputPath.substr(0, outputPath.length()-1) : outputPath;
}


void BoundariesExtractor::ConnectBackground(GrayImageP& grayImage)
{
    const auto upperIndex = grayImage->GetRequestedRegion().GetUpperIndex();

    for (unsigned col=0; col <= upperIndex[0]; ++col)
    {
        grayImage->SetPixel({col,0}, Background);
        grayImage->SetPixel({col,upperIndex[1]}, Background);
    }


    for (unsigned row=0; row <= upperIndex[1]; ++row)
    {
        grayImage->SetPixel({0,row}, Background);
        grayImage->SetPixel({upperIndex[0], row}, Background);
    }


}


BoundariesExtractor::GrayImageP BoundariesExtractor::ExtractBoundaries(GrayImageP binaryImage)
{

    //Connecting background
    ConnectBackground(binaryImage);


    //Detecting background
    using ConnectedFilterType = itk::ConnectedThresholdImageFilter<GrayImageT, GrayImageT>;
    ConnectedFilterType::Pointer connectedThreshold = ConnectedFilterType::New();
    connectedThreshold->SetInput(binaryImage);
    connectedThreshold->SetLower(Background);
    connectedThreshold->SetUpper(Background);
    connectedThreshold->SetSeed({0,0}); //TODO define it
    connectedThreshold->SetReplaceValue(Foreground);

    connectedThreshold->Update();



    using binaryContourImageFilterType = itk::BinaryContourImageFilter<GrayImageT, GrayImageT>;
    binaryContourImageFilterType::Pointer binaryContourFilter = binaryContourImageFilterType::New();
    binaryContourFilter->SetInput(connectedThreshold->GetOutput());
    binaryContourFilter->SetBackgroundValue(Background);
    binaryContourFilter->SetForegroundValue(Foreground);
    binaryContourFilter->FullyConnectedOn();
    binaryContourFilter->Update();

    io::printOK("Extract boundaries");

    return binaryContourFilter->GetOutput();

}

BoundariesExtractor::GrayImageP BoundariesExtractor::ThinningBoundaries(GrayImageP boundaries)
{

    using BinaryThinningImageFilterType = itk::BinaryThinningImageFilter<GrayImageT, GrayImageT>;
    BinaryThinningImageFilterType::Pointer binaryThinningImageFilter = BinaryThinningImageFilterType::New();
    binaryThinningImageFilter->SetInput(boundaries);
    binaryThinningImageFilter->Update();

    // Rescale the image so that it can be seen (the output is 0 and 1, we want 0 and 255)
    using RescaleType = itk::RescaleIntensityImageFilter<GrayImageT, GrayImageT>;
    RescaleType::Pointer rescaler = RescaleType::New();
    rescaler->SetInput(binaryThinningImageFilter->GetOutput());
    rescaler->SetOutputMinimum(Background);
    rescaler->SetOutputMaximum(Foreground);
    rescaler->Update();


    io::printOK("Thinning Boundaries");


    return rescaler->GetOutput();



}

BoundariesExtractor::GrayImageP BoundariesExtractor::GrayToBinary(GrayImageP grayImage, bool show)
{

    auto binaryImage = GrayImageT::New();
    binaryImage->SetRegions(grayImage->GetRequestedRegion());
    binaryImage->Allocate();


    itk::ImageRegionConstIterator<GrayImageT> gIt(grayImage, grayImage->GetRequestedRegion()); //edges it
    itk::ImageRegionIterator<GrayImageT>      bIt(binaryImage, binaryImage->GetRequestedRegion()); //output it


    for(; !gIt.IsAtEnd(); ++gIt, ++bIt)
    {


        //std::cout<<gIt.Get()<<std::endl;
        bIt.Set( (gIt.Get() == Foreground )? Background : Foreground );

    }


    if(show)
    {

        VTKViewer::visualize<GrayImageT>(binaryImage, "Gray to Binary");
    }


    io::printOK("Simple gray to binary");

    return binaryImage;

}


void BoundariesExtractor::Process(bool returnResults)
{

    //get all files in dataset path
    std::vector<std::string> imagePaths;
    std::vector<std::string> imageNames;
    // std::string imageName;
    for(auto filePath: std::filesystem::directory_iterator(DatasetPath))
    {
        imagePaths.push_back(filePath.path());
        imageNames.push_back( *io::Split( *io::Split(filePath.path(), '/').rbegin(), '.').begin() );

    }

    auto imagePathIt = imagePaths.begin();
    auto imageNameIt = imageNames.begin();
    GrayImageP boundaries;
    GrayImageP binaryImage;
    for(; imagePathIt != imagePaths.end(); ++imagePathIt, ++imageNameIt)
    {

        io::printWait("Boundary Extraction image "+*imageNameIt);
        binaryImage = GrayToBinary(io::ReadImage<GrayImageT>(*imagePathIt));
        boundaries = ExtractBoundaries(binaryImage);

        if(ThinBoundaries)
        {
            OutputImages.push_back(ThinningBoundaries(boundaries));
        }
        else
        {
            OutputImages.push_back(boundaries);
        }
        //To-Do save results..... delete small components.....
    }


}


