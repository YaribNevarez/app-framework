//------------------------------------------------------------------------------
/**
 *
 * @file: thread.h
 *
 * @Created on: April 29th, 2019
 * @Author: Yarib Nevarez
 *
 *
 * @brief - Runnable and Thread class, Implementation of active-object pattern
 *
 *
 * <Requirement Doc Reference>
 * <Design Doc Reference>
 *
 * @copyright Copyright [2019] ITEM.
 * All Rights Reserved.
 *
 *
 */
//------------------------------------------------------------------------------

// IFNDEF ----------------------------------------------------------------------
#ifndef TREAD_HPP_
#define TREAD_HPP_

// INCLUDES --------------------------------------------------------------------
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>

namespace item {
namespace snn {
namespace app_framework {
// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------

// EUNUMERATIONS ---------------------------------------------------------------

// STRUCTS AND NAMESPACES ------------------------------------------------------

// CLASS DECLARATION -----------------------------------------------------------

/**
 * Runnable is an abstract class that provides the prototype pattern for active
 * objects.
 */
class Runnable
{
public:
  /**
   * Running entry point.
   */
	virtual int run(void) = 0;
};

/**
 * Thread class provides the threading handling to implement the active object.
 * This class does not implement the pure virtual method run(), the result is
 * again an abstract class with threading mechanisms.
 * TODO: Evaluate the usage of CThread
 */
class Thread: public Runnable
{
public:
	Thread(void);

	virtual ~Thread(void);

	virtual int start(void);

	virtual int quit(void);

	virtual int yield(void);

	virtual bool isRunning(void);

private:

	static void * process_trigger(void * runnable);
	pthread_t	thread; // Thread identifier
};

} // namespace app_framework
} // namespace snn
} // namespace item

// ENDIF -----------------------------------------------------------------------
#endif /* TREAD_HPP_ */
