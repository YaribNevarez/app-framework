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
typedef float                               Epsilon;
typedef float                               StateVariable;

typedef uint16_t                            SpikeID;
typedef std::vector<SpikeID>                SpikeIDRow;
typedef std::vector<SpikeIDRow>             SpikeIDMatrix;

// typedef std::vector<std::unique_ptr<Neuron>> NeuronVector;
typedef std::vector<StateVariable>          StateVector;
typedef std::vector<InferencePopulation *>  PopulationRow;
typedef std::vector<PopulationRow *>        PopulationMatrix;


typedef float                               Weight;
typedef std::vector<Weight>                 WeightRow;
typedef std::vector<WeightRow>              WeightMatrix;

// EUNUMERATIONS ---------------------------------------------------------------

// CLASS DECLARATIONS -----------------------------------------------------------
class Spikes: public SpikeIDMatrix
{
public:
  Spikes(uint16_t rows, uint16_t columns);
  virtual ~Spikes();
};

// Instantiate P
class Weights: public WeightMatrix
{
public:
  Weights(uint16_t rows, uint16_t columns);
  virtual ~Weights();
};

// SbS Inference population class

class InferencePopulation: public StateVector
{
public:
  InferencePopulation(uint16_t neurons);
  ~InferencePopulation();

  void updateH(WeightRow P, Epsilon e);

  SpikeID genSpike(void);

private:
  std::mt19937 mt_rand;
};


// SbS Layer class

class BaseLayer: public PopulationMatrix
{
public:
  BaseLayer(uint16_t rows, uint16_t columns, uint16_t neurons, uint16_t kernel_size = 0, uint16_t kernel_stride = 0);
  virtual ~BaseLayer();

  virtual void giveWeights(Weights * weights);
  virtual Spikes generateSpikes(void);
  virtual void update(Spikes spikes);

protected:
  Weights * weights_ = nullptr;
  uint16_t  kernel_size_ = 0;
  uint16_t  kernel_stride_ = 0;
};

class InputLayer: public BaseLayer
{
public:
  InputLayer(uint16_t rows, uint16_t columns, uint16_t neurons);
  virtual ~InputLayer();
};

class ConvolutionLayer: public BaseLayer
{
public:
  ConvolutionLayer(uint16_t rows, uint16_t columns, uint16_t neurons, uint16_t kernel_size);
  virtual ~ConvolutionLayer();
  virtual void update(Spikes spikes);
};

class PoolingLayer: public BaseLayer
{
public:
  PoolingLayer(uint16_t rows, uint16_t columns, uint16_t neurons, uint16_t kernel_size);
  virtual ~PoolingLayer();
  virtual void update(Spikes spikes);
};

class FullyConnectedLayer: public BaseLayer
{
public:
  FullyConnectedLayer(uint16_t rows, uint16_t columns, uint16_t neurons, uint16_t kernel_size);
  virtual ~FullyConnectedLayer();
  virtual void update(Spikes spikes);
};

class OutputLayer: public BaseLayer
{
public:
  OutputLayer(uint16_t rows, uint16_t columns, uint16_t neurons);
  virtual ~OutputLayer();
  virtual void update(Spikes spikes);
};

} // namespace sbs
} // namespace snn
} // namespace item
#endif /* SBS_NETWORK_H_ */
