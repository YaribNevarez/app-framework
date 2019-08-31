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
#include <unistd.h>
#include <random>

namespace item {
namespace snn {
namespace sbs {
// FORWARD DECLARATIONS --------------------------------------------------------
class InferencePopulation;
class BaseLayer;
class Spikes;

// TYPEDEFS AND DEFINES --------------------------------------------------------
#define MERSENNE_TWISTER_SEED             666

typedef float                             Epsilon;
typedef float                             StateVariable;

typedef uint16_t                          SpikeID;
typedef std::vector<SpikeID>              SpikeIDRow;
typedef std::vector<SpikeIDRow>           SpikeIDMatrix;
typedef std::vector<Spikes>               SpikesVector;

typedef std::vector<StateVariable>        StateVector;
typedef std::vector<InferencePopulation>  PopulationRow;
typedef std::vector<PopulationRow>        PopulationMatrix;

typedef float                             Weight;
typedef std::vector<Weight>               WeightRow;
typedef std::vector<WeightRow>            WeightMatrix;

typedef std::vector<BaseLayer *>          LayerVector;
// EUNUMERATIONS ---------------------------------------------------------------

// CLASS DECLARATIONS -----------------------------------------------------------
class Spikes: public SpikeIDMatrix
{
public:
  Spikes(uint16_t rows = 1, uint16_t columns = 1);
  virtual ~Spikes();
};

class Weights: public WeightMatrix
{
public:
  Weights(uint16_t rows, uint16_t columns, std::string file_name = "");
  virtual ~Weights();

  virtual bool load(std::string file_name);
};

class InferencePopulation: public StateVector
{
public:
  InferencePopulation(uint16_t neurons);
  virtual ~InferencePopulation();

  virtual void update(WeightRow P, Epsilon e);

  virtual SpikeID genSpike(void);

  virtual void initialize(void); // Initialize all the H(i) =(1/N)

private:
  static std::mt19937 mt_rand_;
};

class BaseLayer: public PopulationMatrix
{
public:

  typedef enum
  {
    ROW_SHIFT,
    COLUMN_SHIFT
  } WeightSectionShift;

  BaseLayer(uint16_t rows,
            uint16_t columns,
            uint16_t neurons,
            uint16_t kernel_size = 0,
            uint16_t kernel_stride = 0,
            WeightSectionShift weight_section_shift = ROW_SHIFT,
            uint16_t neurons_prev_Layer = 0);
  virtual ~BaseLayer();

  virtual void setWeights(Weights * weights);
  virtual Spikes generateSpikes(void);
  virtual void update(Spikes spikes);

  virtual void initialize(void); // Initialize all the H(i) =(1/N)

  virtual void setEpsilon(float epsilon);

protected:
  Weights * weights_ = nullptr;
  uint16_t kernel_size_ = 0;
  uint16_t kernel_stride_ = 0;

  WeightSectionShift weight_section_shift_ = ROW_SHIFT;
  uint16_t neurons_prev_Layer_ = 0;

  float epsilon_ = 0.1;
};

class InputLayer: public BaseLayer
{
public:
  InputLayer(uint16_t rows, uint16_t columns, uint16_t neurons);
  virtual ~InputLayer();

  virtual bool load(std::string file_name);
  virtual uint8_t getLabel();
private:
  uint8_t label_ = -1;
};

class ConvolutionLayer: public BaseLayer
{
public:
  ConvolutionLayer(uint16_t rows,
                   uint16_t columns,
                   uint16_t neurons,
                   uint16_t kernel_size,
                   WeightSectionShift weight_section_shift,
                   uint16_t neurons_prev_Layer);
  virtual ~ConvolutionLayer();
  virtual void update(Spikes spikes);
};

class PoolingLayer: public BaseLayer
{
public:
  PoolingLayer(uint16_t rows,
               uint16_t columns,
               uint16_t neurons,
               uint16_t kernel_size,
               WeightSectionShift weight_section_shift,
               uint16_t neurons_prev_Layer);
  virtual ~PoolingLayer();
  virtual void update(Spikes spikes);
};

class FullyConnectedLayer: public BaseLayer
{
public:
  FullyConnectedLayer(uint16_t neurons,
                      uint16_t kernel_size,
                      WeightSectionShift weight_section_shift,
                      uint16_t neurons_prev_Layer);
  virtual ~FullyConnectedLayer();
  virtual void update(Spikes spikes);
};

class OutputLayer: public BaseLayer
{
public:
  OutputLayer(uint16_t neurons,
              WeightSectionShift weight_section_shift,
              uint16_t neurons_prev_Layer);
  virtual ~OutputLayer();
  virtual void update(Spikes spikes);
  virtual uint16_t getOutput(void);
};

class Network: public LayerVector
{
public:
  Network();
  virtual ~Network();

  virtual bool loadInput(std::string file_name);

  virtual uint8_t getInputLabel(void);

  virtual void updateCycle(uint16_t cycles);

  virtual uint16_t getOutput(void);
};

} // namespace sbs
} // namespace snn
} // namespace item
#endif /* SBS_NETWORK_H_ */
