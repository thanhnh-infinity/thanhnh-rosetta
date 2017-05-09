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
#include <stdlib.h>
#include <cassert>
#include <vector>

#include "nn_model_mem.h"
#include "rosetta.h"
#include "txt_class.h"

// Input ERRORs
#define SSC_INVALID 1
#define BD_INVALID  2
#define TH33_INVALID  4
#define TH1500_INVALID  8
#define VG4_INVALID  16
#define MODEL_INVALID 32

class ROS_BASE
{
 public:
  ROS_BASE()
    {
      allocated=Init_Rosetta_Models();
   }
  int Make_Estimate(ANN_MODEL ann_model, RosettaInput& rosinput, RosettaOutput& rosoutput);
  // user can call this to see whether ALL models were allocated succesfully
  bool is_allocated()
  {
    if (allocated==0)
      return true;
    else 
      return false;
  }

  int nboot_ret(ANN_MODEL ann_model)
  {
    return(nn_model_ret[ann_model].Get_nboot());
  }

  int nboot_ks(ANN_MODEL ann_model)
  {
    return(nn_model_ks[ann_model].Get_nboot());
  }

  int nboot_unsk(ANN_MODEL ann_model)
  {
    return(nn_model_unsk[0].Get_nboot());
  }

  const char *ID()
  {
    return("nmodels=5;output=vg_theta_r,vg_theta_s,vg_alpha,vg_npar,ks,vg_ko,vg_lpar;model1=txt;model2=sand,silt,clay;model3=sand,silt,clay,bd;model4=sand,silt,clay,bd,th33;model5=sand,silt,clay,bd,th33,pub=Schaap et al., 2001;comment=retention, Ks and unsaturated K parameters are estimated with different models");
  }
  
 protected:
  // vectors that hold the models
  int allocated;
  std::vector<NN_MODEL> nn_model_ret;
  std::vector<NN_MODEL> nn_model_ks;
  std::vector<NN_MODEL> nn_model_unsk;
  TXT_CLASS *txt_class;

  int Init_Rosetta_Models();
  int transfer(VECTOR *average_vg4,VECTOR *std_vg4,MATRIX *cmat_vg4,VECTOR *average_ks,VECTOR *std_ks,MATRIX *cmat_ks,RosettaOutput *outputm);

};
