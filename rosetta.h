/*
    Rosetta, a Artificial neural network pedotransfer function to estimate
    "van Genuchten"-type parameters for soils and other porous media
    Copyright (C) 2012  Marcel G. Schaap

    Contact:
    mschaap@cals.arizona.edu
    Marcel Schaap
    Dept. of Soil Water and Environmental Sccience
    Shantz Bldng. Rm. 4.29
    College of Agriculture and Life Sciences
    The University of Arizona
    1177 East Fourth Street
    Tucson, AZ 85721-0038

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License (see
    License.txt) along  with this program; if not, write to the Free 
    Software Foundation, Inc.,  51 Franklin Street, Fifth Floor, Boston, 
    MA 02110-1301 USA.

*/
#ifndef ROSETTA_H
#define ROSETTA_H


enum ANN_MODEL
  {
    TXT=0,
    SSC=1,
    SSCBD=2,
    SSCBDTH33=3,
    SSCBDTH3315=4,
    ANN_NONE=5,  // hack!
    BPM=6,    
  };


#endif