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
// data.cpp: implementation of the Cdata class.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "data.h"
#define TINY 0.01

int RosettaInput::is_valid_ssc()

{
  double sum;
  int ok;

  ok=1;
  if ((sand<0.0)||(sand>101.0)) ok=0;
  if ((silt<0.0)||(silt>101.0))   ok=0;
  if ((clay<0.0)||(clay>101.0))   ok=0;
  sum=sand+clay+silt;
  if ((sum>MAXSUM)||(sum<MINSUM)) 
    {
      ok=0; 
    }
  return(ok);
}

int RosettaInput::is_valid_csfssc()

{
  double sum;
  int ok;

  ok=1;
  if ((coarse_sand<0.0)||(coarse_sand>101.0)) ok=0;
  if ((fine_sand<0.0)||(fine_sand>101.0)) ok=0;
  if ((silt<0.0)||(silt>101.0))   ok=0;
  if ((clay<0.0)||(clay>101.0))   ok=0;
  sum=coarse_sand+fine_sand+clay+silt;
  if ((sum>MAXSUM)||(sum<MINSUM)) 
    {
      ok=0; 
    }
  return(ok);
}


int RosettaInput::is_valid_bd()
{
  int ok;

  ok=1;
  if (bd<MINBD) {
    ok=0;
  }
  if (bd>MAXBD) {
    ok=0;
  }
  return(ok);
}

int RosettaInput::is_valid_oc()
{
  int ok;

  ok=1;
  if (oc<0.0) {
    ok=0;
  }
  if (oc>MAXOC) {
    ok=0;
  }
  return(ok);
}

int RosettaInput::is_valid_th33()
{
  int ok;
  ok=1;
  if (th33<MINTH33) ok=0;
  if (th33>MAXTH33) ok=0;

  return(ok);
}

int RosettaInput::is_valid_th1500()
{
  int ok;
  ok=1;
  if (th1500<MINTH15) ok=0;
  if (th1500>MAXTH15) ok=0;

  if (th1500>th33) ok=0;

  return(ok);
}

int RosettaInput::is_valid_gravel()
{
  int ok;

  ok=1;
  if (gravel<MINGRAVEL) {
    ok=0;
  }
  if (gravel>MAXGRAVEL) {
    ok=0;
  }
  return(ok);
}
int RosettaInput::is_valid_VG4()
{

  // the hydraulic parameters should be linear, i.e. not scaled or log values.
  int ok=1;

  if (vgthr<0.0) ok=0;
  if (vgthr>1.0) ok=0;
  if (vgths<0.0) ok=0;
  if (vgths>1.0) ok=0;
  if (vgths<vgthr) ok=0;
  if (vgalp<0) ok=0;
  if (vgalp>1) ok=0;
  if (vgnpar<=1.0) ok=0;
  // this will screw up people with very high n values.......
  if (vgnpar>10.0) ok=0;

  return(ok);
}


int RosettaInput::ReadInputData(FILE *inputfile)
{
  int ncount=0;

  // this to avoid reading on a empty new line
  if (!feof(inputfile))
    {
      ncount=0;
      ncount+=fscanf(inputfile,"%Ld",&code);
      ncount+=fscanf(inputfile,"%s",description);		
      ncount+=fscanf(inputfile,"%lf",&bd);
      ncount+=fscanf(inputfile,"%lf",&gravel);
      ncount+=fscanf(inputfile,"%lf",&sand);
      ncount+=fscanf(inputfile,"%lf",&silt);
      ncount+=fscanf(inputfile,"%lf",&clay);;
      if ((ferror(inputfile))||(ncount!=7))
	{
	  return(0);
	}
    }
  else
    {
      return(0);
    }
  return(ncount);
}

// NOTE NOT a member function!
// std::ostream& operator<<(std::ostream& output, const RosettaData& rosdata)
// {
//   output << rosdata.Input.code;
//   output << ' ' << rosdata.Input.description;
//   output << ' ' << rosdata.Input.bottom_level;
//   return output;
// }

// std::ostream& operator<<(std::ostream& output, RosettaInput& rosinput)
// {
//   output << rosinput.code;
//   output << "   " << rosinput.description;
//   output << "   " << rosinput.bd;
//   output << "   " << rosinput.gravel;
//   output << "   " << rosinput.sand;
//   output << "   " << rosinput.silt;
//   output << "   " << rosinput.clay;
//   return output;
// }




