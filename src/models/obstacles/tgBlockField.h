/*
 * Copyright © 2014, United States Government, as represented by the
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

#ifndef TG_BLOCK_FIELD
#define TG_BLOCK_FIELD

/**
 * @file tgBlockField.h
 * @brief Contains the definition of class tgBlockField.
 * A random field of blocks used to test tensegrities
 * @date January 2015
 * @author Brian Mirletz
 * $Id$
 */

// This library
#include "core/tgModel.h"
// The Bullet Physics Library
#include "LinearMath/btVector3.h"
// The C++ Standard Library
#include <vector>

// Forward declarations
class tgModelVisitor;
class tgStructure;
class tgWorld;

/**
 * Class that creates a number of boxes strewn about an area
 * @todo Add parameters to vary the size and rotation of the boxes
 */
class tgBlockField : public tgModel
{
public: 
    
    struct Config
    {
        public:
            Config(btVector3 origin = btVector3(0.0, 0.0, 0.0),
                    btScalar friction = 0.5,
                    btScalar restitution = 0.0,
                    btVector3 minPos = btVector3(-100.0, 0.0, -100.0),
                    btVector3 maxPos = btVector3(100.0, 0.0, 100.0),
                    size_t nBlocks = 500,
                    double blockLength = 5.0,
                    double blockWidth = 5.0,
                    double blockHeight = 5.0);

            /** Origin position of the block field */
            btVector3 m_origin;
            
            /** Friction value of the blocks, must be between 0 to 1 */
            btScalar  m_friction;

            /** Restitution coefficient of the blocks, must be between 0 to 1 */
            btScalar  m_restitution;

            /** Provides the minimum coordinates of a bounding box within which the blocks will be placed */
            btVector3 m_minPos;
            
            /** Provides the maximum coordinates of a bounding box within which the blocks will be placed */
            btVector3 m_maxPos;

            /** Number of blocks in the field */
            size_t m_nBlocks;

            /** Length of the blocks */
            double m_length;
            
            /** Width of the blocks */
            double m_width;
            
            /** Height of the blocks */
            double m_height;
    };
    
   /**
    * Default constructor. Sets the default config values
    */
    tgBlockField();

    /**
        * Origin constructor. Copies the config to m_config
        * @param[in] config - specifies the parameters of the block field
        */
    tgBlockField(tgBlockField::Config& config);

    /**
        * Destructor. Deletes controllers, if any were added during setup.
        * Teardown handles everything else.
        */
    virtual ~tgBlockField();

    /**
        * Create the model.
        * @param[in] world - the world we're building into
        */
    virtual void setup(tgWorld& world);

    /**
        * Step the model, its children. Notifies controllers of step.
        * @param[in] dt, the timestep. Must be positive.
        */
    virtual void step(double dt);

    /**
        * Receives a tgModelVisitor and dispatches itself into the
        * visitor's "render" function. This model will go to the default
        * tgModel function, which does nothing.
        * @param[in] r - a tgModelVisitor which will pass this model back
        * to itself 
        */
    virtual void onVisit(tgModelVisitor& r);

    /**
        * Undoes setup. Deletes child models. Called automatically on
        * reset and end of simulation. Notifies controllers of teardown
        */
    void teardown();  

private:

   /**
    * A function called during setup that determines the positions of
    * the nodes (center points of opposing box faces) 
    * based on construction parameters.
    * @param[in] s: the tgStructure that we're building into
    */
    void addNodes(tgStructure& s);
    
    tgBlockField::Config m_config;

};

#endif // TETRA_COLLISIONS_WALL
