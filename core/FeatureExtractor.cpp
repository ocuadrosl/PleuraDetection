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

    //kernel size must be odd
    this->KernelSize = (kernelSize%2==0) ? kernelSize+1 : kernelSize;
}

/*


*/
void FeatureExtractor::FindCenters(GrayImageP boundaries, std::vector<GrayImageT::IndexType>& centers)
{


    using DuplicatorType = itk::ImageDuplicator<GrayImageT>;
    DuplicatorType::Pointer duplicator = DuplicatorType::New();
    duplicator->SetInputImage(boundaries);
    duplicator->Update();
    auto boundariesDuplicate = duplicator->GetOutput();

    /* //tmp image for testing
    auto tmpImage = GrayImageT::New();
    tmpImage->SetRegions(boundaries->GetRequestedRegion());
    tmpImage->Allocate();
    tmpImage->FillBuffer(100); //100 is just a middle-value different enough from 0 and 255
*/
    itk::NeighborhoodIterator<GrayImageT>::RadiusType radius;
    radius.Fill(KernelSize/2);

    itk::ConstNeighborhoodIterator<GrayImageT> it(radius, boundariesDuplicate, boundariesDuplicate->GetRequestedRegion());
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
                if(boundariesDuplicate->GetRequestedRegion().IsInside(index))
                {
                    boundariesDuplicate->SetPixel(index, Background);
                    //tmpImage->SetPixel(index, Foreground);
                }
            }
            //VTKViewer::visualize<GrayImageT>(tmpImage, "Centers");
        }


    }

    //VTKViewer::visualize<GrayImageT>(tmpImage, "Centers");
    io::printOK("Compute Centers");

}

void FeatureExtractor::ComputeLBPFeatures(GrayImageP grayImage, const CentersT& centers, FeaturesVectorT& featuresVector)
{
    featuresVector.clear();
    featuresVector.reserve(centers.size());

    using grayPixeT = GrayImageT::PixelType;
    using dlibGrayImageT = dlib::array2d<grayPixeT>;

    //ITK to Dlib image
    dlibGrayImageT dlibGrayImage;
    image::ITKToDlib<grayPixeT, grayPixeT>(grayImage, dlibGrayImage);

    //compute uniform LBP
    using lbpImageT = dlib::array2d<unsigned char>;
    lbpImageT lbpImage;
    dlib::make_uniform_lbp_image(dlibGrayImage, lbpImage);


    //dlib::image_window my_window(lbpImage, "LBP");
    //my_window.wait_until_closed();

    const auto& ExtractNeighborhood = image::ExtractNeighborhood<lbpImageT, GrayImageT::IndexType>;
    dlib::array2d<unsigned char> neighborhood;

    dlib::matrix<unsigned long> lbpHistogramLongT;
    FeaturesT lbpHistogram(59,1);

    for(const auto& index: centers)
    {

        ExtractNeighborhood(lbpImage, index, KernelSize, neighborhood);
        //dlib::image_window my_window2(neighborhood, "LBP");
        // my_window2.wait_until_closed();
        dlib::get_histogram(neighborhood, lbpHistogramLongT, 59);


        auto itLong = lbpHistogramLongT.begin();
        auto itDouble = lbpHistogram.begin();
        for(; itLong != lbpHistogramLongT.end(); ++itLong, ++itDouble)
        {
            (*itDouble) = static_cast<double>(*itLong);

        }

        featuresVector.push_back(lbpHistogram);


    }

    io::printOK("Local Binary Pattern");


}

void FeatureExtractor::ComputeShapeFeatures(GrayImageP grayImage, const CentersT& centers, FeaturesVectorT& featuresVector)
{

    //label maps types
    using LabelType = unsigned short;
    using OutputImageT = itk::Image<unsigned, 2>;
    using ShapeLabelObjectType = itk::ShapeLabelObject<LabelType, 2>;
    using LabelMapType = itk::LabelMap<ShapeLabelObjectType>;

    using ConnectedComponentImageT = itk::ConnectedComponentImageFilter<GrayImageT, OutputImageT>;
    using LabelImageToShapeLabelMapT = itk::LabelImageToShapeLabelMapFilter<OutputImageT, LabelMapType>;
    using RelabelComponentT = itk::RelabelComponentImageFilter<OutputImageT, OutputImageT>;


    const auto& extractNeighborhood = image::ExtractNeighborhoodITK<GrayImageT>;

    featuresVector.clear();
    featuresVector.reserve(centers.size());

    FeaturesT features(3,1);

    for(const auto& center: centers)
    {
        GrayImageT::Pointer tile;
        extractNeighborhood(grayImage, center, KernelSize, tile);

        //VTKViewer::visualize<GrayImageT>(tile, "neigh");


        ConnectedComponentImageT::Pointer connected = ConnectedComponentImageT::New();
        connected->SetInput(tile);
        connected->FullyConnectedOn();
        connected->SetBackgroundValue(Background); //black
        connected->Update();

        //Relabel objects so that the largest object has label #1,
        RelabelComponentT::Pointer relabelFilter = RelabelComponentT::New();
        relabelFilter->SetInput(connected->GetOutput());
        relabelFilter->Update();

        LabelImageToShapeLabelMapT::Pointer labelImageToShapeMap = LabelImageToShapeLabelMapT::New();
        labelImageToShapeMap->SetInput(relabelFilter->GetOutput());
        labelImageToShapeMap->Update();

        auto largestObject = labelImageToShapeMap->GetOutput()->GetLabelObject(1);


        //fractal dimension
        auto fractalDimension = std::make_unique<FractalDimensionCalculator<GrayImageT>>();
        fractalDimension->SetInputImage(tile);
        fractalDimension->PrintWarningsOff();
        fractalDimension->SetUnitTileLenght(2);
        fractalDimension->Compute();

        features(0,0) = largestObject->GetFlatness();
        features(1,0) = largestObject->GetRoundness();
        features(2,0) = fractalDimension->GetDimension();
        featuresVector.emplace_back(features);

        //std::cout<<features<<std::endl;


        /*
        //Label-map to RGB image
        using  rgbImageT =  itk::Image<itk::RGBPixel<unsigned char>, 2>;
        typedef itk::LabelMapToRGBImageFilter<LabelMapType, rgbImageT> RGBFilterType;
        typename RGBFilterType::Pointer labelMapToRGBFilter = RGBFilterType::New();
        labelMapToRGBFilter->SetInput(labelImageToShapeMap->GetOutput());
        labelMapToRGBFilter->Update();



        VTKViewer::visualize<rgbImageT>(labelMapToRGBFilter->GetOutput(), "Connected components RGB");
*/


    }

    io::printOK("Shape features");




}


void FeatureExtractor::ComputeFractalDimension(const GrayImageP& grayImage, const CentersT& centers, FeaturesVectorT& featuresVector)
{

    //VTKViewer::visualize<GrayImageT>(grayImage, "neigh");

    const auto& extractNeighborhood = image::ExtractNeighborhoodITK<GrayImageT>;

    featuresVector.clear();
    featuresVector.reserve(centers.size());

    //flatness
    //elongation

    FeaturesT features(1,1);

    auto fractalDimension = std::make_unique<FractalDimensionCalculator<GrayImageT>>();
    fractalDimension->PrintWarningsOff();
    fractalDimension->SetBackGround(255);
    for(const auto& center: centers)
    {
        GrayImageT::Pointer neighborhood;
        extractNeighborhood(grayImage, center, KernelSize, neighborhood);
        //VTKViewer::visualize<GrayImageT>(neighborhood, "neigh");


        fractalDimension->SetInputImage(neighborhood);
        fractalDimension->Compute();
        features(0,0) = fractalDimension->GetDimension();










        featuresVector.emplace_back(features);
        //std::cout<<features<<std::endl;

    }

    io::printOK("Fractal dimension features");



}

void FeatureExtractor::ComputeShapeAndFractalFeatures(GrayImageP grayImage, GrayImageP boundaries, const CentersT& centers, FeaturesVectorT& featuresVector)
{


    //label maps types
    using LabelType = unsigned short;
    using OutputImageT = itk::Image<unsigned, 2>;
    using ShapeLabelObjectType = itk::ShapeLabelObject<LabelType, 2>;
    using LabelMapType = itk::LabelMap<ShapeLabelObjectType>;

    using ConnectedComponentImageT = itk::ConnectedComponentImageFilter<GrayImageT, OutputImageT>;
    using LabelImageToShapeLabelMapT = itk::LabelImageToShapeLabelMapFilter<OutputImageT, LabelMapType>;
    using RelabelComponentT = itk::RelabelComponentImageFilter<OutputImageT, OutputImageT>;

    const auto& extractNeighborhood = image::ExtractNeighborhoodITK<GrayImageT>;

    featuresVector.clear();
    featuresVector.reserve(centers.size());

    //flatness
    //elongation

    FeaturesT features(4,1);

    auto fractalDimension = std::make_unique<FractalDimensionCalculator<GrayImageT>>();
    fractalDimension->PrintWarningsOff();

    for(const auto& center: centers)
    {
        //fractal image
        GrayImageT::Pointer tileImage;

        extractNeighborhood(grayImage, center, KernelSize, tileImage);
        fractalDimension->SetInputImage(tileImage);
        fractalDimension->SetBackGround(Foreground);
        fractalDimension->Compute();
        features(0,0) = fractalDimension->GetDimension();


        //boundaries
        GrayImageT::Pointer tileBoundaries;
        extractNeighborhood(boundaries, center, KernelSize, tileBoundaries);

        ConnectedComponentImageT::Pointer connected = ConnectedComponentImageT::New();
        connected->SetInput(tileBoundaries);
        connected->FullyConnectedOn();
        connected->SetBackgroundValue(Background); //black
        connected->Update();

        //Relabel objects so that the largest object has label #1,
        RelabelComponentT::Pointer relabelFilter = RelabelComponentT::New();
        relabelFilter->SetInput(connected->GetOutput());
        relabelFilter->Update();

        LabelImageToShapeLabelMapT::Pointer labelImageToShapeMap = LabelImageToShapeLabelMapT::New();
        labelImageToShapeMap->SetInput(relabelFilter->GetOutput());
        labelImageToShapeMap->Update();

        auto largestObject = labelImageToShapeMap->GetOutput()->GetLabelObject(1);

        features(1,0) = largestObject->GetFlatness();
        features(2,0) = largestObject->GetRoundness();


        fractalDimension->SetInputImage(tileBoundaries);
        fractalDimension->SetBackGround(Background);
        fractalDimension->Compute();
        features(3,0) = fractalDimension->GetDimension();

        featuresVector.emplace_back(features);
        //std::cout<<features<<std::endl;

    }

    io::printOK("Fractal and Shape dimension features");



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

        features(0,0) = histFeatures->GetEnergy();
        features(1,0) = histFeatures->GetEntropy();
        features(2,0) = histFeatures->GetCorrelation();
        features(3,0) = histFeatures->GetInertia();

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
        csvFile<<"Image Name,Col, Row,";
        for(unsigned i=0; i< FeaturesVector.begin()->size() ; ++i  )
        {
            csvFile<<"feature_"<<i+1<<",";
        }
        csvFile<<"Label"<<std::endl;

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
        //std::cout<<(*iIt)<<std::endl;

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

    csvFile.close();
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

void FeatureExtractor::Process()
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
        //ComputeCooccurrenceMatrixFeatures(image, centers, featuresVector);
        //ComputeLBPFeatures(image, centers, featuresVector);
        //ComputeFractalDimension(image, centers, featuresVector);
        //ComputeShapeFeatures(boundaries, centers, featuresVector);
        ComputeShapeAndFractalFeatures( image, boundaries, centers, featuresVector);


        FeaturesVector.insert(FeaturesVector.end(), std::move_iterator(featuresVector.begin()),  std::move_iterator(featuresVector.end()));
        CentersVector.insert(CentersVector.end(),  std::move_iterator(centers.begin()), std::move_iterator(centers.end()));
        LabelsVector.insert(LabelsVector.end(), std::move_iterator(labelsVector.begin()), std::move_iterator(labelsVector.end()));

        CentersNumberPerImage.emplace_back(centers.size());
    }


    io::printOK("Feature extraction for trainning");

}
