//------------------------------------------------------------------------------
/**
*
* @file: SbSNetworkApp.cpp
*
* @Created on: August 27th, 2019
* @Author: Yarib Nevarez
*
*
* @brief - SbSNetworkApp application
* <Requirement Doc Reference>
* <Design Doc Reference>
*
* @copyright Copyright [2019] ITEM.
* All Rights Reserved.
*
*
*/
//------------------------------------------------------------------------------
// INCLUDES --------------------------------------------------------------------
#include "sbs_network.h"

#include "logger.h"
#include <assert.h>

#include <iostream>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <errno.h>

#include <fstream>

namespace item {
namespace snn {
namespace sbs {

  static std::mt19937 mt_rand(666);

  uint32_t random()
  {
    return mt_rand();
  }

// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------
#define ASSERT(EXPR) assert(EXPR)

// EUNUMERATIONS ---------------------------------------------------------------

// STRUCTS AND NAMESPACES ------------------------------------------------------

// CLASS DEFINITION ------------------------------------------------------------

Spikes::Spikes(uint16_t rows, uint16_t columns)
{
  resize(rows);
  for (uint16_t row = 0; row < rows; row ++)
    at(row).resize(columns);
}

Spikes::~Spikes()
{

}


Weights::Weights (uint16_t rows, uint16_t columns)
{
  // Instantiate IPs
  resize(rows);
  for (uint16_t row = 0; row < rows; row ++)
    at(row).resize(columns);
}

Weights::~Weights ()
{

}

bool Weights::load(std::string file_name)
{
  std::ifstream file(file_name, std::ios::binary);

  for (uint16_t s = 0; s < size(); s ++)
  file.read((char*)at(s).data(), sizeof(Weight)*at(s).size());

}

// SbS Inference population class

InferencePopulation::InferencePopulation(uint16_t neurons)
{
  StateVariable initial_h = 1.0f/(float)neurons;

  for (uint16_t neuron = 0; neuron < neurons; neuron ++)
    push_back(initial_h);

  ASSERT(size() == neurons);
}

InferencePopulation::~InferencePopulation ()
{}

void InferencePopulation::updateH(WeightRow P, Epsilon e)
{
  ASSERT (P.size() == size());
  StateVector temp_h_p(P.size());
  double sum = 0.0;

  for (uint16_t i = 0; i < P.size(); i ++)
  {
    temp_h_p[i] = at(i) * P.at(i);
    sum += temp_h_p[i];
  }

  if (sum < 1e-30) // TODO: DEFINE constant
    return;

  float reverse_e = 1.0 / (1.0 + e);
  float e_over_sum = e / sum;

  for (uint16_t i = 0; i < P.size(); i ++)
  {
   (*this)[i] = reverse_e*(at(i) + temp_h_p[i] * e_over_sum);
  }


}

SpikeID InferencePopulation::genSpike(void)
{
  float random_s = float(mt_rand())/float(0xFFFFFFFF);
  float sum = 0.0;

  ASSERT(random_s <= 1.0);

  for (SpikeID spikeID = 0; spikeID < size() - 1; spikeID ++)
  {
    sum += at(spikeID);

    //ASSERT(sum < 1.000001);

    if (random_s <= sum)
      return spikeID;
  }


  return size() - 1;
}

void InferencePopulation::initialize(void)
{
  //TODO:: initialize 1/H all
}


BaseLayer::BaseLayer (uint16_t rows, uint16_t columns, uint16_t neurons, uint16_t  kernel_size, uint16_t  kernel_stride, bool dir_x, uint16_t N_PreLayer):
    kernel_size_(kernel_size),
    kernel_stride_(kernel_stride),
    dir_x_(dir_x),
    N_PreLayer_(N_PreLayer)
{
  // Instantiate IPs
  for (uint16_t row = 0; row < rows; row ++)
  {
    PopulationRow * IP_row = new PopulationRow();

    ASSERT(IP_row != nullptr);

    for (uint16_t column = 0; column < columns; column ++)
    {
        InferencePopulation * IP = new InferencePopulation(neurons);

        ASSERT(IP != nullptr);

        IP_row->push_back(IP);
    }

    ASSERT(IP_row->size() == columns);

    push_back(IP_row);
  }

  ASSERT(size() == rows);
}

void BaseLayer::giveWeights(Weights * weights)
{
  ASSERT(weights != nullptr);
  weights_ = weights;
}

Spikes BaseLayer::generateSpikes(void)
{
  uint16_t rows = size ();
  uint16_t columns = at (0)->size ();
  Spikes spikes (rows, columns);

  for (uint16_t row = 0; row < rows; row++)
    for (uint16_t column = 0; column < columns; column++)
      spikes[row][column] = (*(*this)[row])[column]->genSpike ();

  return spikes;
}

void BaseLayer::update(Spikes spikes)
{
  ASSERT(weights_ != nullptr);

  if (weights_ == nullptr || kernel_size_ == 0)
    return;

  uint16_t layer_row_size;
  uint16_t layer_column_size;
  SpikeID spikeID = 0;

  layer_row_size = size();
  layer_column_size = at(0)->size();
  uint16_t Constant_A = kernel_size_;
  uint16_t Constant_B = 1;

  if (dir_x_)
    {
    Constant_A = 1;
    Constant_B = kernel_size_;
  }

  uint16_t X = 0;
  uint16_t Y = 0;
  for (uint16_t kernel_row_pos = 0;
       kernel_row_pos < spikes.size() - (kernel_size_ - 1);
       kernel_row_pos += kernel_stride_) {
    for (uint16_t kernel_column_pos = 0;
         kernel_column_pos < spikes.at(0).size() - (kernel_size_ - 1);
         kernel_column_pos += kernel_stride_) {

        for (uint16_t kernel_row = 0; kernel_row < kernel_size_; kernel_row ++)
            for (uint16_t kernel_column = 0; kernel_column < kernel_size_; kernel_column ++)
              {
          spikeID = spikes[kernel_row_pos + kernel_row][kernel_column_pos + kernel_column];

          WeightRow weightRow = weights_->at(spikeID + (kernel_row * Constant_A + kernel_column * Constant_B) * N_PreLayer_);
//          std::cout << kernel_row_pos << " " << kernel_column_pos << " " << kernel_row << " " << kernel_column << " " << kernel_row_pos + kernel_row << " " << kernel_column_pos + kernel_column << " " << (kernel_row * Constant_A + kernel_column * Constant_B) * N_PreLayer_ << "\n";
//          for (uint16_t xxx = 0; xxx < weightRow.size(); xxx++){
//              std::cout << weightRow.at(xxx) << " ";
//          }
//          std::cout << "\n";
          (*(*this)[Y])[X]->updateH(weightRow, epsilon_);
        }
          X++;
    }
    Y ++;
    X = 0;

  }

}

void BaseLayer::initialize(void)
{
  // TODO: Initialize all the H(i) =(1/N)
}

void BaseLayer::setEpsilon(float epsilon)
{
  epsilon_ = epsilon;
}

BaseLayer::~BaseLayer ()
{
  PopulationRow * IP_row = nullptr;
  InferencePopulation * IP = nullptr;
  for (uint16_t row = 0; row < size(); row ++)
  {
    IP_row = (*this)[row];
    (*this)[row] = nullptr;

    ASSERT(IP_row != nullptr);

    for (uint16_t column = 0; column < IP_row->size(); column ++)
    {
      IP = (*IP_row)[column];
      (*IP_row)[column] = nullptr;

      ASSERT(IP != nullptr);

      delete IP;
    }

    delete IP_row;
  }
}

InputLayer::InputLayer (uint16_t rows, uint16_t columns, uint16_t neurons) :
    BaseLayer (rows, columns, neurons)
{

}

InputLayer::~InputLayer ()
{

}

bool InputLayer::load(std::string file_name)
{
  std::ifstream file(file_name, std::ios::binary);

  for (uint16_t x = 0; x < at(0)->size(); x ++)
  for (uint16_t y = 0; y < size(); y ++)
  file.read((char*)at(y)->at(x)->data(), sizeof(StateVariable)*at(y)->at(x)->size());

  file.read((char*)&label_, sizeof(label_));
  label_--;
}

uint8_t InputLayer::getLabel()
{
return   label_;
}

ConvolutionLayer::ConvolutionLayer (uint16_t rows, uint16_t columns, uint16_t neurons, uint16_t kernel_size, bool dir_x, uint16_t N_PreLayer) :
    BaseLayer (rows, columns, neurons, kernel_size, 1, dir_x, N_PreLayer)
{
  ASSERT(kernel_size_ == kernel_size);
  ASSERT(kernel_stride_ == 1);
}

ConvolutionLayer::~ConvolutionLayer ()
{

}

void ConvolutionLayer::update(Spikes spikes)
{
  ASSERT(spikes.size() - kernel_size_ + 1 == size());
  ASSERT(spikes[0].size() - kernel_size_ + 1 == at(0)->size());

  BaseLayer::update(spikes);
}

PoolingLayer::PoolingLayer (uint16_t rows, uint16_t columns, uint16_t neurons, uint16_t kernel_size, bool dir_x, uint16_t N_PreLayer) :
    BaseLayer (rows, columns, neurons, kernel_size, kernel_size, dir_x, N_PreLayer)
{
  ASSERT(kernel_size_ == kernel_size);
  ASSERT(kernel_stride_ == kernel_size);
}

PoolingLayer::~PoolingLayer ()
{

}

void PoolingLayer::update(Spikes spikes)
{
  ASSERT(spikes.size() % kernel_size_ == 0);
  ASSERT(spikes[0].size() % kernel_size_ == 0);

  BaseLayer::update(spikes);
}

FullyConnectedLayer::FullyConnectedLayer(uint16_t neurons, uint16_t kernel_size, bool dir_x, uint16_t N_PreLayer) :
        BaseLayer (1, 1, neurons, kernel_size, 1, dir_x, N_PreLayer)
{
}

FullyConnectedLayer::~FullyConnectedLayer()
{

}

void FullyConnectedLayer::update(Spikes spikes)
{
  ASSERT(spikes.size() == kernel_size_);
  ASSERT(spikes[0].size() == kernel_size_);
  BaseLayer::update(spikes);
}

OutputLayer::OutputLayer(uint16_t neurons, bool dir_x, uint16_t N_PreLayer) :
        BaseLayer (1, 1, neurons, 1, 1, dir_x, N_PreLayer)
{

}

OutputLayer::~OutputLayer()
{

}

void OutputLayer::update(Spikes spikes)
{
  ASSERT(spikes.size() == 1);
  ASSERT(spikes[0].size() == 1);

  BaseLayer::update(spikes);
}

uint16_t OutputLayer::getOutput(void)
{
  float MaxValue = 0;
  uint16_t MaxPos = 0;

  for (int16_t i = 0; i < (*(*this)[0])[0]->size(); i ++)
    {

      if ( (*(*this)[0])[0]->at(i) > MaxValue){
          MaxPos = i;
          MaxValue = (*(*this)[0])[0]->at(i);

      }
  }

  return MaxPos;
}
} // namespace sbs
} // namespace snn
} // namespace item
