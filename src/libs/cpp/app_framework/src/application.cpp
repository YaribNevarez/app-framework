//------------------------------------------------------------------------------
/**
 *
 * @file: application.cpp
 *
 * @Created on: April 29th, 2019
 * @Author: Yarib Nevarez
 *
 *
 * @brief - Application class, This class is the skeleton for multi-threading
 * application.
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
#include "application.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

namespace item {
namespace snn {
namespace app_framework {
// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------

// EUNUMERATIONS ---------------------------------------------------------------

// STRUCTS AND NAMESPACES ------------------------------------------------------

// CLASS DEFINITION ------------------------------------------------------------
Application::Application(int argc, char * argv[]):
argc(argc),
argv(argv)
{}

Application::~Application()
{
	while (!threads.empty())
	{
		if (threads.back() != NULL)
			delete threads.back();

		threads.pop_back();
	}
}

int Application::becomeDaemon(int flags)
{
  int maxfd, fd;

  // Returns 0 on success, -1 on error
  switch (fork())
  {
  case -1:
    return -1;
  case 0:
    break;
  default:
    _exit(EXIT_SUCCESS);
  }

  // Become background process
  if (setsid() == -1)
    return -1; // Become leader of new session
  switch (fork()) {
  case -1:
    return -1;
  case 0:
    break;
  default:
    _exit(EXIT_SUCCESS);
  }

  // Ensure we are not session leader
  // Child falls through...
  // while parent terminates

  if (!(flags & BD_NO_UMASK0))
    umask(0); // Clear file mode creation mask
  if (!(flags & BD_NO_CHDIR))
    chdir("/"); // Change to root directory
  if (!(flags & BD_NO_CLOSE_FILES))
  { // Close all open files
    maxfd = sysconf(_SC_OPEN_MAX);
    if (maxfd == -1)
      // Limit is indeterminate...
      maxfd = BD_MAX_CLOSE;
    // so take a guess
    for (fd = 0; fd < maxfd; fd++)
      close(fd);
  }

  if (!(flags & BD_NO_REOPEN_STD_FDS))
  {
    close (STDIN_FILENO);
    // Reopen standard fd's to /dev/null
    fd = open("/dev/null", O_RDWR);
    if (fd != STDIN_FILENO)
      // 'fd' should be 0
      return -1;
    if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
      return -1;
    if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
      return -1;
  }
  return 0;
}

void Application::register_thread(Thread * thread)
{
	threads.push_back(thread);
}

int Application::passed_arg(std::string argument)
{
	int index = -1;

	if ((0 < argc) && (argv != NULL))
		for (signed i = 0; (-1 == index) && (i < argc) && (argv[i] != NULL); i ++)
			if (argument.compare(argv[i]) == 0) index = i;

	return index;
}

std::string Application::passed_arg(int i)
{
	std::string argument = "";
	if ((0 < argc) && (argv != NULL) && (argv[i] != NULL))
		argument = argv[i];
	return argument;
}

int Application::passed_arg(void)
{
	return argc;
}

int Application::start_threads(void)
{
	int result = EXIT_SUCCESS;

	for (unsigned int i = 0;
		(i < threads.size()) && (result == EXIT_SUCCESS);
		i ++)
	{
		result = threads[i]->start();
	}

	return result;
}

} // namespace app_framework
} // namespace snn
} // namespace item
