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

// SbS Inference population class

InferencePopulation::InferencePopulation(uint8_t neurons):
    mt_rand(rand())
{
  StateVariable initial_h = 1.0f/(float)neurons;

  for (uint8_t neuron = 0; neuron < neurons; neuron ++)
    H_.push_back(initial_h);

  ASSERT(H_.size() == neurons);
}

InferencePopulation::~InferencePopulation ()
{}

SpikeID InferencePopulation::genSpike(void)
{
  float random_s = float(mt_rand())/float(0xFFFFFFFF);
  float sum = 0.0;

  ASSERT(random_s <= 1.0);

  for (SpikeID spikeID = 0; spikeID < H_.size() - 1; spikeID ++)
  {
    sum += H_[spikeID];

    ASSERT(sum <= 1.0);

    if (random_s <= sum)
      return spikeID;
  }

  return H_.size() - 1;
}


Layer::Layer (uint8_t rows, uint8_t columns, uint8_t neurons)
{
  // Instantiate IPs
  for (uint8_t row = 0; row < rows; row ++)
  {
    PopulationRow * IP_row = new PopulationRow();

    ASSERT(IP_row != nullptr);

    for (uint8_t column = 0; column < columns; column ++)
    {
        InferencePopulation * IP = new InferencePopulation(neurons);

        ASSERT(IP != nullptr);

        IP_row->push_back(IP);
    }

    ASSERT(IP_row->size() == columns);

    IP_matrix_.push_back(IP_row);
  }

  ASSERT(IP_matrix_.size() == rows);

  // Instantiate P
  for (uint8_t row = 0; row < rows; row ++)
  {
    PopulationRow * IP_row = new PopulationRow();

    ASSERT(IP_row != nullptr);

    for (uint8_t column = 0; column < columns; column ++)
    {
        InferencePopulation * IP = new InferencePopulation(neurons);

        ASSERT(IP != nullptr);

        IP_row->push_back(IP);
    }

    ASSERT(IP_row->size() == columns);

    IP_matrix_.push_back(IP_row);
  }

  ASSERT(IP_matrix_.size() == rows);
}

Layer::~Layer ()
{
  PopulationRow * IP_row = nullptr;
  InferencePopulation * IP = nullptr;
  for (uint8_t row = 0; row < IP_matrix_.size(); row ++)
  {
    IP_row = IP_matrix_[row];
    IP_matrix_[row] = nullptr;

    ASSERT(IP_row != nullptr);

    for (uint8_t column = 0; column < IP_row->size(); column ++)
    {
      IP = (*IP_row)[column];
      (*IP_row)[column] = nullptr;

      ASSERT(IP != nullptr);

      delete IP;
    }

    delete IP_row;
  }
}

InputLayer::InputLayer (uint8_t rows, uint8_t columns, uint8_t neurons) :
    Layer (rows, columns, neurons)
{

}

InputLayer::~InputLayer ()
{

}

ConvolutionLayer::ConvolutionLayer (uint8_t rows, uint8_t columns, uint8_t neurons) :
    Layer (rows, columns, neurons)
{

}

ConvolutionLayer::~ConvolutionLayer ()
{

}

PoolingLayer::PoolingLayer (uint8_t rows, uint8_t columns, uint8_t neurons) :
    Layer (rows, columns, neurons)
{

}

PoolingLayer::~PoolingLayer ()
{

}

} // namespace sbs
} // namespace snn
} // namespace item
