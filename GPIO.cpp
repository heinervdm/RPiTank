/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2018  Thomas Zimmermann <zimmermann@vdm-design.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GPIO.h"
#include <pigpio.h>

#define LEFT_GPIO_1 5
#define LEFT_GPIO_2 6

#define RIGHT_GPIO_1 13
#define RIGHT_GPIO_2 26

GPIO::GPIO() : initialized(false) {
	init();
}

void GPIO::init() {
	if (gpioInitialise() < 0)
	{
		qDebug() << "Failed to initialize GPIOs";
		return;
	}
	initialized = true;
	
	/* Set GPIO modes */
	gpioSetMode(LEFT_GPIO_1, PI_OUTPUT);
	gpioSetMode(LEFT_GPIO_2, PI_OUTPUT);
	gpioSetMode(RIGHT_GPIO_1, PI_OUTPUT);
	gpioSetMode(RIGHT_GPIO_2, PI_OUTPUT);

	gpioWrite(LEFT_GPIO_1,0);
	gpioWrite(LEFT_GPIO_2,0);
	gpioWrite(RIGHT_GPIO_1,0);
	gpioWrite(RIGHT_GPIO_2,0);
}

GPIO::~GPIO() {
	gpioWrite(LEFT_GPIO_1,0);
	gpioWrite(LEFT_GPIO_2,0);
	gpioWrite(RIGHT_GPIO_1,0);
	gpioWrite(RIGHT_GPIO_2,0);
	gpioTerminate();
}

void GPIO::set(int left, int right) {
	if (left < 0) {
		if (left < -255) {
			left = -255;
		}
		gpioWrite(LEFT_GPIO_1, 0);
		gpioPWM(LEFT_GPIO_2, (-1*left));
	} else {
		if (left > 255) {
			left = 255;
		}
		gpioWrite(LEFT_GPIO_2, 0);
		gpioPWM(LEFT_GPIO_1, left);
	}

	if (right < 0) {
		if (right < -255) {
			right = -255;
		}
		gpioWrite(RIGHT_GPIO_1, 0);
		gpioPWM(RIGHT_GPIO_2, -1*right);
	} else {
		if (right > 255) {
			right = 255;
		}
		gpioWrite(RIGHT_GPIO_2, 0);
		gpioPWM(RIGHT_GPIO_1, right);
	}
}
