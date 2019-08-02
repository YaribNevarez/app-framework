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

// INCLUDES --------------------------------------------------------------------
#include "thread.h"

namespace item {
namespace snn {
namespace app_framework {
// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------

// EUNUMERATIONS ---------------------------------------------------------------

// STRUCTS AND NAMESPACES ------------------------------------------------------

// CLASS DEFINITION ------------------------------------------------------------

Thread::Thread (void):
thread(0)
{}

Thread::~Thread (void)
{
  quit ();
}

int Thread::start (void)
{
  int result = -1;
  if (!isRunning ())
  {
    // TODO: Evaluate the usage of CThread
    result = pthread_create (&thread, NULL, process_trigger, this);
  }
  return result;
}

int Thread::quit (void)
{
  int result = EXIT_SUCCESS;
  if (isRunning ())
  {
    pthread_cancel (thread);
    pthread_join (thread, (void **) &result);

    if ((void *)(result) == PTHREAD_CANCELED)
    {
      result = EXIT_SUCCESS;
    }
  }
  return result;
}

int Thread::yield (void)
{
  return pthread_yield ();
}

bool Thread::isRunning (void)
{
  int result = -1;
  if (thread != 0)
  {
    result = pthread_tryjoin_np (thread, NULL);
  }
  return result == EBUSY;
}

void * Thread::process_trigger (void * runnable)
{
  if (runnable != NULL)
  {
    pthread_exit ((void *) ((Runnable *) runnable)->run ());
  }
  return EXIT_SUCCESS;
}
} // namespace app_framework
} // namespace snn
} // namespace item
