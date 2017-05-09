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
#include <string>
#include <cassert>

#include "txt_class.h"




TXT_CLASS::TXT_CLASS(const double txt_matrix[13][10])
{
  assert(txt_matrix);

  for (int i=0;i<13;i++)
    {
      for (int j=0; j<10; j++)
	{
	  txt_class[i][j]=txt_matrix[i][j];
	}
    }

}

/*clay */
bool TXT_CLASS::is_clay(double sand, double clay)
{
 
    if ((clay>=40.0-TINY)&&
	(sand<45.0)&&
	(100.0-sand-clay<40)&&(sand>=0.0)&&(clay>=0.0)) return(true);
    else 
      return(false);
  }

/* silty clay */
bool TXT_CLASS::is_siltyclay(double sand, double clay)
  {
    if ((clay>=40.0-TINY)&&
	(100.0-sand-clay>=40.0-TINY)&&(sand>=0.0)&&(clay>=0.0)) return(true);
    else 
      return(false);
  }

/* silty clay loam */
bool TXT_CLASS::is_siltyclayloam(double sand, double clay)
  {
    if ((clay>=28-TINY)&&
	(sand<20.0)&&
	(clay<40.0)&&(sand>=0.0)&&(clay>=0.0)) return(true);
    else 
      return(false);
  }

/* clay loam */
bool TXT_CLASS::is_clayloam(double sand, double clay)
  {
    if ((clay>=27.0-TINY)&&
	(clay<40) &&
	(sand>=20.0-TINY)&&
	(sand<45)&&(sand>=0.0)&&(clay>=0.0)) return(true);
    else
      return(false);
  }

/* loam */
bool TXT_CLASS::is_loam(double sand, double clay)
  {
    if ((clay<27.0)&&
	(clay>=7-TINY) &&
	(sand<52.0)&&
	(100-sand-clay>=28-TINY) &&
	(100-sand-clay<50)&&(sand>=0.0)&&(clay>=0.0)) return(true);
    else 
      return(false);
  }

/* silty loam */
bool TXT_CLASS::is_siltyloam(double sand, double clay)
  {

    if ((100-sand-clay>=50.0-TINY)&&
	(clay<28) &&
	!is_silt(sand,clay)&&(sand+clay>0.0)) return(true);
    else 
      return(false);
  }

/* silt */
bool TXT_CLASS::is_silt(double sand, double clay)
  {
    if ((100-sand-clay>=80.0-TINY)&&
	(clay<12)&&
	(sand>=0.0)&&
	(sand+clay!=0.0)&&
	(clay>=0.0)) return(true);
    else 
      return(false);
  }

/* sandy clay */
bool TXT_CLASS::is_sandyclay(double sand, double clay)
{
  if ((clay>=35.0-TINY)&&
      (sand>=45-TINY)&&(sand>=0.0)&&(clay>=0.0)) return(true);
  else 
    return(false);
}

/* sandy clay loam*/
bool TXT_CLASS::is_sandyclayloam(double sand, double clay)
{
  if ((clay<35.0)&&
      (clay>=20-TINY)&&
      (sand>=45-TINY)&&
      (100-sand-clay<28)&&(sand>=0.0)&&(clay>=0.0)) return(true);
    else 
      return(false);
}

/* sandy loam */
bool TXT_CLASS::is_sandyloam(double sand, double clay)
  {
    bool answer;

    if ((clay<20.0)&&
	(sand>=52-TINY)&&
	!((is_loamysand(sand,clay))||(is_sand(sand,clay))) &&
	(sand>=45-TINY)&&(sand>=0.0)&&(clay>=0.0)) answer=true;
    else if( (sand<52)&&(clay<8)&&(100-sand-clay<50)) 
      answer=true;
    else 
      answer=false;

    return(answer);
  }

/* loamy sand */
bool TXT_CLASS::is_loamysand(double sand, double clay)
  {
    if ((sand-clay>=70-TINY)&&
	!is_sand(sand,clay)&&(sand>=0.0)&&(clay>=0.0)) return(true);
    else 
      return(false);
  }

/* sand */
bool TXT_CLASS::is_sand(double sand, double clay)
  {
    if ((2.0*sand-clay>=170-TINY)&&(sand>=0.0)&&(clay>=0.0)) return(true);
    else 
      return(false);
  }

bool TXT_CLASS::is_valid_ssc(double sand, double silt, double clay) 
  {
    double sum;
    bool ok=true;
    
    if ((sand<0.0)||(sand>MAX_TXT_SUM)) ok=false;
    if ((silt<0.0)||(silt>MAX_TXT_SUM)) ok=false;
    if ((clay<0.0)||(clay>MAX_TXT_SUM)) ok=false;
    sum=sand+clay+silt;
    if ((sum>MAX_TXT_SUM)||(sum<MIN_TXT_SUM)) 
      {
	ok=false; 
      }
    return(ok);
  }

  int normalize_texture(double sand, double silt, double clay, double *sand_n, double *silt_n, double *clay_n)
  {
    double sum;
    int ok=0;

    sum=sand+clay+silt;
    if ((sand<0.0) || (silt<0.0) || ((clay<0.0))) 
      {
	/* reject if any fraction is smaller than 0 */
	ok=0;
	*sand_n=-9.9;
	*silt_n=-9.9;
	*clay_n=-9.9;

      }
    else
      {
	*sand_n=sand/sum;
	*silt_n=silt/sum;
	*clay_n=clay/sum;
	ok=1;
      }
    return(ok);
  }

TXT_INDEX TXT_CLASS::determine_texture_class(double sand, double silt, double clay)
  {
    TXT_INDEX index=TXT_UNKNOWN;
    
    if (is_valid_ssc(sand, silt, clay))
      {
	// determine the TXT class from the percentages clay and sand
	// at this point the sum(sand,silt,clay) should be 100 %
	if (is_clay(sand,clay))			{index=TXT_CLAY;}
	else if (is_clayloam(sand,clay))	{index=TXT_CLAYLOAM;}
	else if (is_loam(sand,clay))		{index=TXT_LOAM;}
	else if (is_loamysand(sand,clay))	{index=TXT_LOAMYSAND;}
	else if (is_sand(sand,clay))		{index=TXT_SAND;}
	else if (is_sandyclay(sand,clay))	{index=TXT_SANDYCLAY;}
	else if (is_sandyclayloam(sand,clay))	{index=TXT_SANDYCLAYLOAM;}
	else if (is_sandyloam(sand,clay))	{index=TXT_SANDYLOAM;}
	else if (is_silt(sand,clay))		{index=TXT_SILT;}
	else if (is_siltyclay(sand,clay))	{index=TXT_SILTYCLAY;}
	else if (is_siltyclayloam(sand,clay))	{index=TXT_SILTYCLAYLOAM;}
	else if (is_siltyloam(sand,clay))	{index=TXT_SILTYLOAM;}
	else					{index=TXT_UNKNOWN;}
      }
    else
      {
	index=TXT_UNKNOWN;
      }
    return(index);
  }
