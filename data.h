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
// data.h: 
#ifndef DATA_H
#define DATA_H

#include <list>
#include <stdio.h>
#include <string.h>

#include "rosetta.h"

#define MINBD	0.5
#define MAXBD	2.0
#define MINGRAVEL	0.0
#define MAXGRAVEL	100.0
#define MINSUM	99
#define MAXSUM	101
#define MINOC	0.0
#define MAXOC	100.0
#define MINTH33	0.0
#define MAXTH33 1.0
#define MINTH15 0.0
#define MAXTH15 1.0



#define MISSING -9.9
#define NDES	129

#include <ostream>
#include <stdlib.h>

typedef char DESCRIPTION[129]; 

class RosettaInput 
{
 public:
  //TXT_CLASS *txt_class;
  long long code;
  DESCRIPTION description;
  double bd;
  double gravel;
  double sand;
  double coarse_sand;
  double fine_sand;
  double oc;
  double silt;
  double clay;
  double th33;
  double th1500;
 
  // input for unsatk prediction
  double vgthr;
  double vgths;
  double vgalp;
  double vgnpar;

  int ReadInputData(FILE *inputfile);

 public:
  int is_valid_ssc();
  int is_valid_csfssc();
  int is_valid_bd();
  int is_valid_oc();
  int is_valid_th33();
  int is_valid_th1500();
  int is_valid_gravel();
  int is_valid_VG4();

 public:
  RosettaInput()
    {
      initialize();
    }		
  ~RosettaInput()
    {
    }

  void Invalid()
  {
    initialize();
  }
  
 protected:

  void initialize()
    {
      code=0;
      strcpy(description,"none");
      sand=MISSING;
      coarse_sand=MISSING;
      fine_sand=MISSING;
      silt=MISSING;
      clay=MISSING;
      bd=MISSING;
      oc=MISSING;
      gravel=MISSING;
      th33=MISSING;
      th1500=MISSING;
 
    }


/*
std::ostream& operator<<(std::ostream& output)
{
  output << this->code;
  output << "   " << this->description;
  output << "   " << this->bd;
  output << "   " << this->gravel;
  output << "   " << this->sand;
  output << "   " << this->silt;
  output << "   " << this->clay;
  return output;
}
*/

};

class RosettaOutput 
{
 public:
  int code;
  char description[NDES];
  ANN_MODEL ann_model; 
  
  double vgthr;
  double vgths;
  double vgalp;
  double vgnpar;
  double ks;
  double stdvgthr;
  double stdvgths;
  double stdvgalp;
  double stdvgnpar;
  double stdks;

  double unsks;
  double unsl;
  double stdunsks;
  double stdunsl;


  void initialize()
  {
    code=0;
    strcpy(description,"none");
    vgthr=MISSING;
    vgths=MISSING;
    vgalp=MISSING;
    vgnpar=MISSING;
    ks=MISSING;
    stdvgthr=MISSING;
    stdvgths=MISSING;
    stdvgalp=MISSING;
    stdvgnpar=MISSING;
    stdks=MISSING;
  }
  
 public:
  RosettaOutput()
    {
      initialize();
    }

  void Invalid()
    {
      initialize();
    }

};


class RosettaData
{
 public:
  RosettaInput Input;
  RosettaOutput Output;
 public:
  RosettaData(RosettaInput& in, RosettaOutput& out)
    {
      Input=in;
      Output=out;
    }

  RosettaData& operator=(const RosettaData &rhs)
    {
      this->Input = rhs.Input;
      this->Output = rhs.Output;
      return *this;
    }
  

};

//typedef std::list<RosettaData> DM_SOIL;

#endif
