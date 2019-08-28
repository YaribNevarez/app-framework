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


namespace item {
namespace snn {
namespace sbs {
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

// SbS Inference population class

InferencePopulation::InferencePopulation(uint16_t neurons):
    mt_rand(rand())
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
  float sum = 0.0;

  for (uint16_t i = 0; i < P.size(); i ++)
  {
    temp_h_p[i] = at(i) * P.at(i);
    sum += temp_h_p[i];
  }

  if (sum < 0.000000000000001) // TODO: DEFINE constant
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

    ASSERT(sum <= 1.0);

    if (random_s <= sum)
      return spikeID;
  }

  return size() - 1;
}


BaseLayer::BaseLayer (uint16_t rows, uint16_t columns, uint16_t neurons, uint16_t  kernel_size, uint16_t  kernel_stride):
    kernel_size_(kernel_size),
    kernel_stride_(kernel_stride)
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

  if (weights_ != nullptr && kernel_size_ != 0)
    return;

  uint16_t layer_row_size;
  uint16_t layer_column_size;
  SpikeID spikeID = 0;
  const float EPSILON = 0.1; // TODO: Find a place for Epsilon

  layer_row_size = size();
  layer_column_size = at(0)->size();

  for (uint16_t kernel_row_pos = 0;
       kernel_row_pos < layer_row_size;
       kernel_row_pos += kernel_stride_)
    for (uint16_t kernel_column_pos = 0;
         kernel_column_pos < layer_column_size;
         kernel_column_pos += kernel_stride_)

      for (uint16_t kernel_row = 0; kernel_row < kernel_size_; kernel_row ++)
        for (uint16_t kernel_column = 0; kernel_column < kernel_size_; kernel_column ++)
        {
          spikeID = spikes[kernel_row_pos + kernel_row][kernel_column_pos + kernel_column];

          WeightRow weightRow = weights_->at(spikeID);

          (*(*this)[kernel_row_pos])[kernel_column_pos]->updateH(weightRow, EPSILON);
        }
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


ConvolutionLayer::ConvolutionLayer (uint16_t rows, uint16_t columns, uint16_t neurons, uint16_t kernel_size) :
    BaseLayer (rows, columns, neurons, kernel_size, 1)
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

PoolingLayer::PoolingLayer (uint16_t rows, uint16_t columns, uint16_t neurons, uint16_t kernel_size) :
    BaseLayer (rows, columns, neurons, kernel_size, kernel_size)
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

FullyConnectedLayer::FullyConnectedLayer(uint16_t rows, uint16_t columns, uint16_t neurons, uint16_t kernel_size) :
        BaseLayer (rows, columns, neurons, kernel_size, 1)
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

OutputLayer::OutputLayer(uint16_t rows, uint16_t columns, uint16_t neurons) :
        BaseLayer (rows, columns, neurons)
{

}

OutputLayer::~OutputLayer()
{

}

void OutputLayer::update(Spikes spikes)
{

}

} // namespace sbs
} // namespace snn
} // namespace item
