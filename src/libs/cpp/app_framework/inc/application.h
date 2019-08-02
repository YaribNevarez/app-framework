//------------------------------------------------------------------------------
/**
 *
 * @file: application.h
 *
 * @Created on: April 29th, 2019
 * @Author: Yarib Nevarez
 *
 *
 * @brief - Application class, This class is the skeleton for multi-threading
 * application.
 *
 * This is an abstract class implementing Runnable interface (active-object
 * pattern), and adding mechanisms of multi-threading handling. This application
 * is capable of registering thread objects in an internal thread vector (maybe
 * during construction), and then the execution of all registered threads is
 * triggered by start_threads(), they can be yielded, synchronized, and stopped
 * at any time by the application. When destroying, the threads are stopped
 * automatically.
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
#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_

// INCLUDES --------------------------------------------------------------------
#include <errno.h>
#include <stdlib.h>
#include <vector>
#include <string>

#include "thread.h"

namespace item {
namespace snn {
namespace app_framework {
// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------

// EUNUMERATIONS ---------------------------------------------------------------

// STRUCTS AND NAMESPACES ------------------------------------------------------

// CLASS DECLARATION -----------------------------------------------------------
/**
 *  Application. This class is the skeleton for multi-threading application
 */
class Application: public Runnable
{
public:
  // SCOPED DEFINITIONS --------------------------------------------------------
  enum
  {   // Bit-mask values for 'flags' argument of becomeDaemon()
    BD_BACKGROUND =        0, // Background process
    BD_NO_CHDIR =          1, // Don't chdir("/")
    BD_NO_CLOSE_FILES =    2, // Don't close all open files
    BD_NO_REOPEN_STD_FDS = 4, // Don't reopen stdin, stdout, and stderr to /dev/null
    BD_NO_UMASK0 =         8, // Don't do a umask(0)
  };

  // CONSTRUCTOR AND DESTRUCTOR ------------------------------------------------
	Application(int argc = 0, char * argv[] = NULL);
	virtual ~Application();

	// FUNCTIONS -----------------------------------------------------------------

protected:
  // FUNCTIONS -----------------------------------------------------------------
	static int becomeDaemon(int flags);
	virtual void register_thread(Thread * thread);
	virtual int  start_threads(void);
	virtual int         passed_arg(std::string argument);
	virtual std::string passed_arg(int i);
	virtual int         passed_arg(void);

	// MEMBERS -------------------------------------------------------------------

private:
  // FUNCTIONS -----------------------------------------------------------------

  // MEMBERS -------------------------------------------------------------------
	// Maximum file descriptors to close if sysconf(_SC_OPEN_MAX) is indeterminate
	static const int BD_MAX_CLOSE = 8192;

	int     argc;
	char ** argv;

	std::vector<Thread *> threads;
};

} // namespace app_framework
} // namespace snn
} // namespace item

// ENDIF -----------------------------------------------------------------------
#endif /* APPLICATION_HPP_ */
