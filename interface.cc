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
#include "ros_base.h"

extern "C"  
{
  void * CreateInstance( void )
  {
    static ROS_BASE rosetta;
    return (void *) &rosetta;
  }

  bool Allocated(void *pRosetta)
  {
    ROS_BASE * rosetta = reinterpret_cast<ROS_BASE *>(pRosetta);
    return(rosetta->is_allocated());
  }

  const char *ID(void *pRosetta)
  {
    ROS_BASE * rosetta = reinterpret_cast<ROS_BASE *>(pRosetta);
    return(rosetta->ID());
  }

  int nboot_ret(void *pRosetta, int ann)
  {
    ROS_BASE * rosetta = reinterpret_cast<ROS_BASE *>(pRosetta);
    return(rosetta->nboot_ret((ANN_MODEL)ann));
  }
  int nboot_ks(void *pRosetta, int ann)
  {
    ROS_BASE * rosetta = reinterpret_cast<ROS_BASE *>(pRosetta);
    return(rosetta->nboot_ks((ANN_MODEL)ann));
  }
  int nboot_unsk(void *pRosetta, int ann=0)
  {
    ROS_BASE * rosetta = reinterpret_cast<ROS_BASE *>(pRosetta);
    return(rosetta->nboot_unsk((ANN_MODEL)ann));
  }

  int Make_Estimate(void *pRosetta,
		    int ann,

		    double sand,
		    double silt,
		    double clay,
		    double bd,
		    double th33,
		    double th1500,

		    double *theta_r,
		    double *theta_s,
		    double *alpha,
		    double *npar,
		    double *Ks,
		    double *Ko,
		    double *lpar,

		    double *theta_r_std,
		    double *theta_s_std,
		    double *alpha_std,
		    double *npar_std,
		    double *Ks_std,
		    double *Ko_std,
		    double *lpar_std)
  {

    RosettaOutput rosoutput;
    RosettaInput rosinput;

    // printf("sand: %f\n",sand);
    // printf("silt: %f\n",silt);
    // printf("clay: %f\n",clay);
    // printf("bd: %f\n",bd);

    rosinput.sand=sand;
    rosinput.silt=silt;
    rosinput.clay=clay;
    rosinput.bd=bd;
    rosinput.th33=th33;
    rosinput.th1500=th1500;

    ANN_MODEL ann_model= (ANN_MODEL) ann;
    int res;
    try
      {
	ROS_BASE * rosetta = reinterpret_cast<ROS_BASE *>(pRosetta);
	//	printf("Before call\n");
	res=rosetta->Make_Estimate(ann_model,rosinput,rosoutput);
	//	printf("result is %d\n",res);
      }
    catch(...)
      {
	return -1;
      }
    
    //printf("%f %f %f %f\n",rosoutput.vgthr,rosoutput.vgths,rosoutput.vgalp,rosoutput.vgnpar);

    *theta_r=rosoutput.vgthr;
    *theta_s=rosoutput.vgths;
    *alpha=rosoutput.vgalp;
    *npar=rosoutput.vgnpar;
    *Ks=rosoutput.ks;
    *Ko=rosoutput.unsks;
    *lpar=rosoutput.unsl;

    *theta_r_std=rosoutput.stdvgthr;
    *theta_s_std=rosoutput.stdvgths;
    *alpha_std=rosoutput.stdvgalp;
    *npar_std=rosoutput.stdvgnpar;
    *Ks_std=rosoutput.stdks;
    *Ko_std=rosoutput.stdunsks;
    *lpar_std=rosoutput.stdunsl;
  
    return(res);
  }


} 
