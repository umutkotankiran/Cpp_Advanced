#include "textbox.h"
#include "slider.h"
#include "alarm_system.h"
#include "weatherdata.h"

int main()
{
	TextBox textbox;
	Slider slider;
	AlarmSystem alarm_system;

	WeatherData ws; 

	ws.register_observer(&textbox);
	ws.register_observer(&slider);
	ws.register_observer(&alarm_system);

	ws.set_state(30, 3.5, 1.1200);
	ws.set_state(65, 5.3, 4.8);
}