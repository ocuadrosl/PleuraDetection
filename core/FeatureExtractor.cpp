#include "FeatureExtractor.h"

FeatureExtractor::FeatureExtractor()
{

}


void FeatureExtractor::SetDatasetPath(const std::string& dataSetPath)
{
    DatasetPath = (*dataSetPath.rbegin() == '/') ? dataSetPath.substr(0, dataSetPath.length()-1) : dataSetPath;
}


void FeatureExtractor::Process()
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
    for(; imagePathIt != imagePaths.end(); ++imagePathIt, ++imageNameIt)
    {

        io::printWait("Boundary Extraction image "+*imageNameIt);
        boundaries = io::ReadImage<GrayImageT>(*imagePathIt);

    }




}
