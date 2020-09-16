#include "FeatureExtractor.h"

FeatureExtractor::FeatureExtractor()
{

}

/*
Boundaries muts be binary images with background value equals to zero and foreground 255
*/
void FeatureExtractor::SetBoundariesPath(const std::string& boundariesPath)
{
    //delet last "/"
    BoundariesPath = (*boundariesPath.rbegin() == '/') ? boundariesPath.substr(0, boundariesPath.length()-1) : boundariesPath;
}

void FeatureExtractor::SetImagesPath(const std::string& imagesPath)
{
    ImagesPath = (*imagesPath.rbegin() == '/') ? imagesPath.substr(0, imagesPath.length()-1) : imagesPath;

}

void FeatureExtractor::SetLabelsPath(const std::string& labelsPath)
{

    LabelsPath = (*labelsPath.rbegin() == '/') ? labelsPath.substr(0, labelsPath.length()-1) : labelsPath;
}


void FeatureExtractor::SetKernelSize(const std::size_t& kernelSize)
{

    this->KernelSize = kernelSize;
}

/*


*/
void FeatureExtractor::FindCenters(GrayImageP boundaries, std::vector<GrayImageT::IndexType>& centers)
{

   //tmp image for testing
    auto tmpImage = GrayImageT::New();
    tmpImage->SetRegions(boundaries->GetRequestedRegion());
    tmpImage->Allocate();
    tmpImage->FillBuffer(100); //100 is just a middle-value different enough from 0 and 255

    itk::NeighborhoodIterator<GrayImageT>::RadiusType radius;
    radius.Fill(KernelSize/2);

    itk::ConstNeighborhoodIterator<GrayImageT> it(radius, boundaries, boundaries->GetRequestedRegion());
    //ensure centers vector is empty
    centers.clear();
    for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {

        if(it.GetCenterPixel() == Foreground)
        {
            centers.push_back(it.GetIndex());
            for (unsigned i=0; i < it.Size(); ++i)
            {
                const auto& index = it.GetIndex(i);
                if(boundaries->GetRequestedRegion().IsInside(index))
                {
                    boundaries->SetPixel(index, Background);
                    //tmpImage->SetPixel(index, Foreground);
                }
            }
            //VTKViewer::visualize<GrayImageT>(tmpImage, "Centers");
        }


    }

    //VTKViewer::visualize<GrayImageT>(tmpImage, "Centers");
    io::printOK("Compute Centers");

}

void FeatureExtractor::ComputeCooccurrenceMatrixFeatures(GrayImageP grayImage,  const CentersT& centers, FeaturesVectorT& featuresVector)
{

    using ScalarImageToCooccurrenceMatrixFilter =  itk::Statistics::ScalarImageToCooccurrenceMatrixFilter<GrayImageT>;

    using histogramT =  ScalarImageToCooccurrenceMatrixFilter::HistogramType;
    using offsetVectorT = itk::VectorContainer<unsigned char, ScalarImageToCooccurrenceMatrixFilter::OffsetType>;

    //using offsetT = GrayImageT::OffsetType;
    //offsetT offset = {-1,1};

    offsetVectorT::Pointer offsets = offsetVectorT::New();
    // offsets->reserve(2);

    offsets->push_back({-1,1});
    offsets->push_back({1,1});
    offsets->push_back({1,-1});


    using  histToFeaturesT = itk::Statistics::HistogramToTextureFeaturesFilter<histogramT>;


    const auto extractNeighborhood = image::ExtractNeighborhoodITK<GrayImageT>;

    featuresVector.clear();
    featuresVector.reserve(centers.size());

    FeaturesT features(4,1);

    for(auto it = centers.begin(); it != centers.end(); ++it)
    {

        GrayImageT::Pointer neighborhood;
        extractNeighborhood(grayImage, *it, KernelSize, neighborhood);

        //std::cout<<*it<<std::endl;
        //VTKViewer::visualize<GrayImageT>(neighborhood, "neigh");


        ScalarImageToCooccurrenceMatrixFilter::Pointer scalarImageToCooccurrenceMatrixFilter = ScalarImageToCooccurrenceMatrixFilter::New();

        scalarImageToCooccurrenceMatrixFilter->SetInput(neighborhood);
        scalarImageToCooccurrenceMatrixFilter->SetOffsets(offsets);
        scalarImageToCooccurrenceMatrixFilter->SetNumberOfBinsPerAxis(256);
        scalarImageToCooccurrenceMatrixFilter->SetPixelValueMinMax(0,255);


        scalarImageToCooccurrenceMatrixFilter->Update();


        histToFeaturesT::Pointer histFeatures =  histToFeaturesT::New();
        histFeatures->SetInput(scalarImageToCooccurrenceMatrixFilter->GetOutput());
        histFeatures->Update();

        features(0,1) = histFeatures->GetEnergy();
        features(1,1) = histFeatures->GetEntropy();
        features(2,1) = histFeatures->GetCorrelation();
        features(3,1) = histFeatures->GetInertia();

        featuresVector.push_back(features);

        //std::cout<<features<<std::endl;

    }

    io::printOK("Co-occurrence Matrix features");

}

void FeatureExtractor::WriteFeaturesCSV(const std::string& fileName, bool writeHeader) const
{
    std::ofstream csvFile(fileName);

    if(writeHeader)
    {
        //To-Do write the header

    }

    auto fIt = FeaturesVector.begin();
    auto lIt = LabelsVector.begin();
    auto cIt = CentersVector.begin();

    auto iIt = ImagesNames.begin();
    auto sNrIt = CentersNumberPerImage.begin();

    std::uint16_t samplesCounter = 0;
    while(fIt != FeaturesVector.end())
    {
        //writing center index
        std::cout<<(*iIt)<<std::endl;

        //Writing Image names
        csvFile.write((*iIt).c_str(), (*iIt).length());
        csvFile<<",";

        //Center index
        csvFile<<(*cIt)[0]<<","<<(*cIt)[1]<<",";

        //Features
        for(const auto& feature : *fIt  )
        {
            csvFile<<feature<<",";
        }

        //Label
        csvFile<<*lIt<<std::endl;

        ++fIt;
        ++lIt;
        ++cIt;
        if(++samplesCounter == *sNrIt )
        {
            ++iIt;
            ++sNrIt;
            samplesCounter = 0;
        }


    }

    io::printOK("Writing CSV file");
}

void FeatureExtractor::FindLabels(const RGBImageP& labelsImage,
                                  const CentersT& centersVector,
                                  std::vector<double>& labelsVector)
{


    labelsVector.clear();
    labelsVector.reserve(centersVector.size());

    itk::RGBPixel<unsigned char> green;
    green.Set(0, 255, 0);

    for (const auto& center : centersVector )
    {
        labelsVector.push_back(( labelsImage->GetPixel(center) == green )? Pleura : NonPleura );
    }

    io::printOK("Finding labels");


}


void FeatureExtractor::ProcessForTrainning()
{

    //get all files in dataset path
    std::vector<std::string> imagesPaths;
    ImagesNames.clear();
    for(auto filePath: std::filesystem::directory_iterator(ImagesPath))
    {
        //imagesNames.push_back( *io::Split( *io::Split(filePath.path(), '/').rbegin(), '.').begin() );
        ImagesNames.push_back( *io::Split(filePath.path(), '/').rbegin() );
    }

    GrayImageP boundaries, image;
    RGBImageP labels;

    std::vector<GrayImageT::IndexType> centers;
    FeaturesVectorT featuresVector;
    std::vector<double> labelsVector;



    CentersNumberPerImage.clear();
    for(const auto& imageName : ImagesNames)
    {
        io::printWait("Feature extraction image "+imageName);

        image      = io::ReadImage<GrayImageT>(ImagesPath+"/"+imageName);
        boundaries = io::ReadImage<GrayImageT>(BoundariesPath+"/"+imageName);
        labels     = io::ReadImage<RGBImageT >(LabelsPath+"/"+imageName);

        FindCenters(boundaries, centers);
        FindLabels(labels, centers, labelsVector);
        ComputeCooccurrenceMatrixFeatures(image, centers, featuresVector);

        FeaturesVector.insert( FeaturesVector.end(), std::move_iterator(featuresVector.begin()),  std::move_iterator(featuresVector.end()));
        CentersVector.insert(CentersVector.end(),  std::move_iterator(centers.begin()), std::move_iterator(centers.end()));
        LabelsVector.insert(LabelsVector.end(), std::move_iterator(labelsVector.begin()), std::move_iterator(labelsVector.end()));

        CentersNumberPerImage.emplace_back(centers.size());
    }


    io::printOK("Feature extraction for trainning");

}
