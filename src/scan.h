/*
 *   Copyright 2017 Sam Gonshaw
 *
 *   This file is part of Cookie Clicker Auto Reindeer Clicker.
 *   Cookie Clicker Auto Reindeer Clicker
 *
 *   Cookie Clicker Auto Reindeer Clicker is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Cookie Clicker Auto Reindeer Clicker is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Cookie Clicker Auto Reindeer Clicker.
 *   If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SCAN_H
#define SCAN_H

/**
 * Begin scanning a window for changes and clicking on them.
 *
 * @param[in] windowID The X11 ID for the window
 */
void initiateScan(int windowID);

#endif // SCAN_H
