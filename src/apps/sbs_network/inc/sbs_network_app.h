//------------------------------------------------------------------------------
/**
 *
 * @file: sbs_network_app.h
 *
 * @Created on: August 27th, 2019
 * @Author: Yarib Nevarez
 *
 *
 * @brief - SbSNetworkApp application
 * <Requirement Doc Reference>
 * <Design Doc Reference>
 *
 * @copyright Copyright [2019]
 * All Rights Reserved.
 *
 */
//------------------------------------------------------------------------------

// IFNDEF ----------------------------------------------------------------------
#ifndef SBS_NETWORK_APP_H_
#define SBS_NETWORK_APP_H_

// INCLUDES --------------------------------------------------------------------
#include "application.h"
#include "sbs_network.h"

#include <vector>
#include <chrono>
#include <mutex>
#include <unistd.h>
#include <map>


namespace item {
namespace snn {
namespace apps {
// FORWARD DECLARATIONS --------------------------------------------------------

// TYPEDEFS AND DEFINES --------------------------------------------------------

// EUNUMERATIONS ---------------------------------------------------------------

// CLASS DECLARATION -----------------------------------------------------------

/**
 *  SbSNetworkApp. SbSNetworkApp service application
 */
class SbSNetworkApp: public app_framework::Application
{
public:
  SbSNetworkApp(int argc = 0, char * argv[] = NULL);
	virtual ~SbSNetworkApp();

	virtual int run(void);

protected:

	virtual Result initialize(void);

	virtual Result appProcess(void);

};

} // namespace apps
} // namespace snn
} // namespace item
#endif /* SBS_NETWORK_APP_H_ */
