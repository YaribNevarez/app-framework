//------------------------------------------------------------------------------
/**
 *
 * @file: sbs_network.h
 *
 * @Created on: August 27th, 2019
 * @Author: Yarib Nevarez
 *
 *
 * @brief - SbS network definitions
 * <Requirement Doc Reference>
 * <Design Doc Reference>
 *
 * @copyright Copyright [2019]
 * All Rights Reserved.
 *
 */
//------------------------------------------------------------------------------

// IFNDEF ----------------------------------------------------------------------
#ifndef SBS_NETWORK_H_
#define SBS_NETWORK_H_

// INCLUDES --------------------------------------------------------------------

#include <vector>
#include <map>
#include <chrono>
#include <unistd.h>
#include <random>

namespace item {
namespace snn {
namespace sbs {
// FORWARD DECLARATIONS --------------------------------------------------------
class Neuron;
class InferencePopulation;

// TYPEDEFS AND DEFINES --------------------------------------------------------
typedef float                               StateVariable;
typedef uint8_t                             SpikeID;

// typedef std::vector<std::unique_ptr<Neuron>> NeuronVector;
typedef std::vector<StateVariable>          StateVector;
typedef std::vector<InferencePopulation *>  PopulationRow;
typedef std::vector<PopulationRow *>        PopulationMatrix;


typedef float                               Weight;
typedef std::vector<Weight *>               WeightRow;
typedef std::vector<WeightRow *>            WeightMatrix;

// EUNUMERATIONS ---------------------------------------------------------------

// CLASS DECLARATIONS -----------------------------------------------------------

// Instantiate P
class Weights: public WeightMatrix
{
  Weights(uint8_t rows, uint8_t columns);
  ~Weights();
};

// SbS Inference population class

class InferencePopulation: public StateVector
{
public:
  InferencePopulation(uint8_t neurons);
  ~InferencePopulation();

  SpikeID genSpike(void);

private:
  std::mt19937 mt_rand;
};


// SbS Layer class

class Layer: public PopulationMatrix
{
public:
  Layer(uint8_t rows, uint8_t columns, uint8_t neurons);
  virtual ~Layer();

private:
  WeightMatrix * P_matrix_;
};

class InputLayer: public Layer
{
public:
  InputLayer(uint8_t rows, uint8_t columns, uint8_t neurons);
  virtual ~InputLayer();
};

class ConvolutionLayer: public Layer
{
public:
  ConvolutionLayer(uint8_t rows, uint8_t columns, uint8_t neurons);
  virtual ~ConvolutionLayer();
};

class PoolingLayer: public Layer
{
public:
  PoolingLayer(uint8_t rows, uint8_t columns, uint8_t neurons);
  virtual ~PoolingLayer();
};

} // namespace sbs
} // namespace snn
} // namespace item
#endif /* SBS_NETWORK_H_ */
