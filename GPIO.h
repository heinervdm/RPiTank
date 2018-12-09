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

#ifndef GPIO_H
#define GPIO_H

#include <QObject>

class GPIO : public QObject
{
    Q_OBJECT

public:
	GPIO();
	~GPIO();

public slots:
	void set(int left, int right);

private:
	void init();

private:
	bool initialized;
};

#endif // GPIO_H
