/*
 * joystick.hpp
 *
 *  Created on: Sep 19, 2017
 *      Author: Yarib Nevárez <yarib_007@hotmail.com>
 */

#ifndef JOYSTICK_HPP_
#define JOYSTICK_HPP_
#include <string>

namespace item {
namespace snn {
namespace joystick {

class Joystick
{
public:
	Joystick(const char * file_name);
	virtual ~Joystick(void);

	bool open_device(void);
	bool is_open(void);
	void close_device(void);

	std::string get_name(void);
	unsigned get_number_of_axis(void);
	unsigned get_number_of_buttons(void);
	int get_axis_value(int axis);
	int get_button_value(int button);

protected:
	void features_info(void);
	void event_handler(void);

	std::string	file_name;
	int   file_descriptor;

	std::string joystick_name;
	int   number_of_axis;
	int   number_of_buttons;
	int * axis;
	int * buttons;
};

} // namespace joystick
} // namespace snn
} // namespace item
#endif /* JOYSTICK_HPP_ */
