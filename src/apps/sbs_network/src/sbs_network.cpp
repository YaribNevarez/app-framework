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

#include <assert.h>

#include <iostream>
#include <stdio.h>
#include <string.h>

#include <fstream>

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
  for (uint16_t row = 0; row < rows; row++)
    at(row).resize(columns);
}

Spikes::~Spikes()
{

}

Weights::Weights(uint16_t rows, uint16_t columns, std::string file_name)
{
  // Instantiate IPs
  resize(rows);
  for (uint16_t row = 0; row < rows; row++)
    at(row).resize(columns);

  load(file_name);
}

Weights::~Weights()
{

}

bool Weights::load(std::string file_name)
{
  if (file_name.empty())
    return false;

  std::ifstream file(file_name, std::ios::binary);

  for (uint16_t s = 0; file.good() && s < size(); s++)
    file.read((char*) at(s).data(), sizeof(Weight) * at(s).size());

  return file.good();
}

// SbS Inference population class

std::mt19937 InferencePopulation::mt_rand_(MERSENNE_TWISTER_SEED);

InferencePopulation::InferencePopulation(uint16_t neurons)
{
  resize(neurons);

  ASSERT(size() == neurons);

  initialize();
}

InferencePopulation::~InferencePopulation()
{
}

void InferencePopulation::update(WeightRow weightRow, Epsilon e)
{
  ASSERT(weightRow.size() == size());
  StateVector temp_h_p(size());
  StateVariable sum = 0.0;

  for (uint16_t i = 0; i < size(); i++)
  {
    temp_h_p[i] = at(i) * weightRow.at(i);
    sum += temp_h_p[i];
  }

  if (sum < 1e-20) // TODO: DEFINE constant
    return;

  StateVariable reverse_e = 1.0 / (1.0 + e);
  StateVariable e_over_sum = e / sum;

  for (uint16_t i = 0; i < size(); i++)
  {
    at(i) = reverse_e * (at(i) + temp_h_p[i] * e_over_sum);
  }
}

SpikeID InferencePopulation::genSpike(void)
{
  StateVariable random_s = StateVariable(mt_rand_()) / StateVariable(0xFFFFFFFF);
  StateVariable sum = 0.0;

  ASSERT(random_s <= 1.0);

  for (SpikeID spikeID = 0; spikeID < size() - 1; spikeID++)
  {
    sum += at(spikeID);

    ASSERT(sum <= 1 + 1e-5);

    if (random_s <= sum)
      return spikeID;
  }

  return size() - 1;
}

void InferencePopulation::initialize(void)
{
  StateVariable initial_value_h = 1.0f / StateVariable(size());

  for (uint16_t neuron = 0; neuron < size(); neuron++)
    at(neuron) = initial_value_h;
}

BaseLayer::BaseLayer(uint16_t rows,
                     uint16_t columns,
                     uint16_t neurons,
                     uint16_t kernel_size,
                     uint16_t kernel_stride,
                     WeightSectionShift weight_section_shift,
                     uint16_t neurons_prev_Layer) :
    kernel_size_(kernel_size),
    kernel_stride_(kernel_stride),
    weight_section_shift_(weight_section_shift),
    neurons_prev_Layer_(neurons_prev_Layer)
{
  resize(rows);
  for (uint16_t row = 0; row < rows; row++)
  {
    at(row).resize(columns, neurons);

    ASSERT(at(row).size() == columns);
  }

  ASSERT(size() == rows);
}

void BaseLayer::setWeights(Weights * weights)
{
  ASSERT(weights != nullptr);
  weights_ = weights;
}

Spikes BaseLayer::generateSpikes(void)
{
  uint16_t rows = size();
  uint16_t columns = at(0).size();
  Spikes spikes(rows, columns);

  for (uint16_t row = 0; row < rows; row++)
    for (uint16_t column = 0; column < columns; column++)
      spikes[row][column] = at(row).at(column).genSpike();

  return spikes;
}

void BaseLayer::update(Spikes spikes)
{
  ASSERT(weights_ != nullptr);

  if (weights_ == nullptr || kernel_size_ == 0)
    return;

  SpikeID spikeID = 0;

  uint16_t row_shift = kernel_size_;
  uint16_t column_shift = 1;
  uint16_t section_shift = 0;

  if (weight_section_shift_ == ROW_SHIFT)
  {
    row_shift = 1;
    column_shift = kernel_size_;
  }

  for (uint16_t kernel_row_pos = 0, layer_row = 0;
       kernel_row_pos < spikes.size() - (kernel_size_ - 1);
       kernel_row_pos += kernel_stride_, layer_row ++)
  {
    for (uint16_t kernel_column_pos = 0, layer_column = 0;
         kernel_column_pos < spikes.at(0).size() - (kernel_size_ - 1);
         kernel_column_pos += kernel_stride_, layer_column ++)
    {
      for (uint16_t kernel_row = 0;
          kernel_row < kernel_size_;
          kernel_row++)
      {
        for (uint16_t kernel_column = 0;
            kernel_column < kernel_size_;
            kernel_column++)
        {
          spikeID = spikes[kernel_row_pos + kernel_row][kernel_column_pos + kernel_column];

          section_shift = (kernel_row * row_shift + kernel_column * column_shift) * neurons_prev_Layer_;

          WeightRow weightRow = weights_->at(spikeID + section_shift);

          at(layer_row).at(layer_column).update(weightRow, epsilon_);
        }
      }
    }
  }

}

void BaseLayer::initialize(void)
{
  for (size_t row = 0; row < size(); row ++)
  {
    for (size_t column = 0; column < at(row).size(); column ++)
    {
      at(row).at(column).initialize();
    }
  }
}

void BaseLayer::setEpsilon(float epsilon)
{
  epsilon_ = epsilon;
}

BaseLayer::~BaseLayer()
{

}

InputLayer::InputLayer(uint16_t rows, uint16_t columns, uint16_t neurons) :
    BaseLayer(rows, columns, neurons)
{

}

InputLayer::~InputLayer()
{

}

bool InputLayer::load(std::string file_name)
{
  std::ifstream file(file_name, std::ios::binary);

  for (uint16_t x = 0; file.good() && x < at(0).size(); x++)
    for (uint16_t y = 0; file.good() && y < size(); y++)
      file.read((char*) at(y).at(x).data(), sizeof(StateVariable) * at(y).at(x).size());

  file.read((char*) &label_, sizeof(label_));
  label_ --;

  return file.good();
}

uint8_t InputLayer::getLabel()
{
  return label_;
}

ConvolutionLayer::ConvolutionLayer(uint16_t rows,
                                   uint16_t columns,
                                   uint16_t neurons,
                                   uint16_t kernel_size,
                                   WeightSectionShift weight_section_shift,
                                   uint16_t neurons_prev_Layer) :
    BaseLayer(rows, columns, neurons, kernel_size, 1, weight_section_shift, neurons_prev_Layer)
{
  ASSERT(kernel_size_ == kernel_size);
  ASSERT(kernel_stride_ == 1);
}

ConvolutionLayer::~ConvolutionLayer()
{

}

void ConvolutionLayer::update(Spikes spikes)
{
  ASSERT(spikes.size() - kernel_size_ + 1 == size());
  ASSERT(spikes[0].size() - kernel_size_ + 1 == at(0).size());

  BaseLayer::update(spikes);
}

PoolingLayer::PoolingLayer(uint16_t rows,
                           uint16_t columns,
                           uint16_t neurons,
                           uint16_t kernel_size,
                           WeightSectionShift weight_section_shift,
                           uint16_t neurons_prev_Layer) :
    BaseLayer(rows, columns, neurons, kernel_size, kernel_size, weight_section_shift,
        neurons_prev_Layer)
{
  ASSERT(kernel_size_ == kernel_size);
  ASSERT(kernel_stride_ == kernel_size);
}

PoolingLayer::~PoolingLayer()
{

}

void PoolingLayer::update(Spikes spikes)
{
  ASSERT(spikes.size() % kernel_size_ == 0);
  ASSERT(spikes[0].size() % kernel_size_ == 0);

  BaseLayer::update(spikes);
}

FullyConnectedLayer::FullyConnectedLayer(uint16_t neurons,
                                         uint16_t kernel_size,
                                         WeightSectionShift weight_section_shift,
                                         uint16_t neurons_prev_Layer) :
    BaseLayer(1, 1, neurons, kernel_size, 1, weight_section_shift, neurons_prev_Layer)
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

OutputLayer::OutputLayer(uint16_t neurons,
                         WeightSectionShift weight_section_shift,
                         uint16_t neurons_prev_Layer) :
    BaseLayer(1, 1, neurons, 1, 1, weight_section_shift, neurons_prev_Layer)
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
  StateVariable max_value = 0;
  uint16_t max_value_position = 0;

  for (size_t i = 0; i < at(0).at(0).size(); i++)
  {
    if (at(0).at(0).at(i) > max_value)
    {
      max_value_position = i;
      max_value = at(0).at(0).at(i);
    }
  }

  return max_value_position;
}

Network::Network()
{

}

Network::~Network()
{

}

bool Network::loadInput(std::string file_name)
{
  ASSERT(size() > 0);
  if (size() == 0)
    return false;

  InputLayer * input_layer = dynamic_cast<InputLayer *>(at(0));

  ASSERT(input_layer != nullptr);
  if (input_layer == nullptr)
    return false;

  return input_layer->load(file_name);
}

uint8_t Network::getInputLabel(void)
{
  ASSERT(size() > 0);
  if (size() == 0)
    return -1;

  InputLayer * input_layer = dynamic_cast<InputLayer *>(at(0));

  ASSERT(input_layer != nullptr);
  if (input_layer == nullptr)
    return -1;

  return input_layer->getLabel();
}

void Network::updateCycle(uint16_t cycles)
{
  ASSERT(size() > 0);
  if (size() == 0)
    return;

  for (uint16_t i = 1; i < size(); i ++)
  {
    ASSERT(at(i) != nullptr);
    if (at(i) == nullptr)
      return;

    at(i)->initialize();
  }

  SpikesVector spikesVector(size() - 1);

  while (cycles --)
  {
    for (uint16_t i = 0; i < size() - 1; i ++)
    {
      spikesVector[i] = at(i)->generateSpikes();
    }

    for (uint16_t i = 1; i < size(); i ++)
    {
      at(i)->update(spikesVector[i - 1]);
    }

    if (cycles % 100 == 0)
      std::cout << " - Spike cycle: " << cycles << std::endl;
  }
}

uint16_t Network::getOutput(void)
{
  ASSERT(size() > 0);
  if (size() == 0)
    return -1;

  OutputLayer * output_layer = dynamic_cast<OutputLayer *>(at(size() - 1));

  ASSERT(output_layer != nullptr);
  if (output_layer == nullptr)
    return -1;

  return output_layer->getOutput();
}

} // namespace sbs
} // namespace snn
} // namespace item
