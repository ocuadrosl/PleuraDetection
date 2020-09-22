#ifndef FRACTALDIMENSIONFILTER_H
#define FRACTALDIMENSIONFILTER_H

#include <itkImage.h>
#include <cmath>
#include <numeric>
#include <set>
#include <itkImageRegionConstIterator.h>


//#include "Math.h"


template<typename ImageT>
class FractalDimensionCalculator
{

    using ImageP = typename ImageT::Pointer;




public:
    FractalDimensionCalculator();
    void SetInputImage(const ImageP& InputImage);

    void SetUnitTileLenght(unsigned lenght); //in pixels
    void SetResolution(unsigned reolution);
    void SetScale(float scale);
    void PrintWarningsOff();
    void SetBackGround(const uint8_t& backgound);

    float GetDimension() const;

    void Compute();




private:

    ImageP InputImage;

    unsigned InitialTileSize{10  }; //Initial tile size
    unsigned ScaleIterations{3   }; //number of tile reduction (scaling)
    float    ScaleFactor    {0.5f}; //must be in [0 1];
    float    Dimension      {0.f }; //result
    bool     PrintWarnings  {true};
    uint8_t  Background{0};

    unsigned CountTiles(unsigned unitTileLenght);


};

template class FractalDimensionCalculator<itk::Image<unsigned char, 2>>;
template class FractalDimensionCalculator<itk::Image<unsigned, 2>>;

#endif // FRACTALDIMENSIONFILTER_H
