/*
 * Copyright © 2012, United States Government, as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All rights reserved.
 * 
 * The NASA Tensegrity Robotics Toolkit (NTRT) v1 platform is licensed
 * under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0.
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
*/

#ifndef TG_DATA_LOGGER2_H
#define TG_DATA_LOGGER2_H

/**
 * @file tgDataLogger2.h
 * @brief Contains the definition of class tgDataLogger2.
 * @author Drew Sabelhaus
 * $Id$
 */

// Includes from NTRTsim
#include "tgDataManager.h"

/**
 * tgDataLogger2 is a tgDataManager. It records data from sensors and outputs
 * that data to a log file, in comma-separated-value (CSV) format.
 */
class tgDataLogger2 : public tgDataManager
{
 public:

  /**
   * The constructor for tgDataLogger2 takes in a string that specifies the location
   * of the log file to create.
   */
  tgDataLogger2(std::string fileNamePrefix);

  /**
   * The base class handles destruction of the sensors and sensorInfos,
   * so nothing to do here.
   */
  ~tgDataLogger2();

  /**
   * The setup function for tgDataLogger2 will:
   * (1) create all the sensors, (2) create a heading from the sensors, and
   * (3) ouput the heading to a file.
   * Declared virtual here just in case any classes inherit from this.
   */
  virtual void setup();

  /**
   * The teardown function closes the log file.
   * TO-DO: should this class also teardown the sensors, or should we let
   * the superclass handle it??
   */
  virtual void teardown();

  /**
   * The step function for tgDataLogger2 will open the log file for output,
   * write a line of sensor data, then close the log file.
   * Declared virtual here just in case any classes inherit from this.
   */
  virtual void step(double dt);

  /**
   * Overwrite toString for the superclass to specify that this data manager
   * is a tgDataLogger2.
   */
  virtual std::string toString() const;

  // TO-DO: write a new invariant for this subclass, instead of using the parent's.

 protected:

  /**
   * Store the full name of the file for writing data, as well as 
   */
  
}


#endif // TG_DATA_LOGGER2_H
