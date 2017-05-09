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
// nn_read.cpp 
//

//#define DEBUG
#include <iostream>

#include <math.h>
//#include <malloc.h>
//#include <stdio.h>
#include <string.h>

#include "data.h"
#include "memory_file.h"
#include "matrix.h"
#include "nn_model_mem.h"

NN_CONFIG::NN_CONFIG()
{
}
NN_CONFIG::~NN_CONFIG()
{
}

INOUT::INOUT()
{
}
INOUT::~INOUT()
{
  //delete scalpar;
}

WEIGHTS::WEIGHTS()
{
}
WEIGHTS::~WEIGHTS()
{
  //delete w;
  //delete b;
}

NN_WEIGHTS::NN_WEIGHTS()
{
}

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
int NN_MODEL::read_nn_control(NN_CONTROL *control)
{
  int index;

  mem_file->fread((char *)&index,sizeof(int),1);
  mem_file->fread((char *)&control->nin,sizeof(int),1);
  mem_file->fread((char *)&control->nlayer,sizeof(int),1);
  mem_file->fread((char *)&control->s1,sizeof(int),1);
  mem_file->fread((char *)&control->s2,sizeof(int),1);
  mem_file->fread((char *)&control->nout,sizeof(int),1);

  return(index);
}

/*--------------------------------------------------------------------*/

int NN_MODEL::read_net()
{
  NN_CONTROL control1, control2;
  int nn_index;

  assert(mem_file);

  // type of neural network 
  mem_file->fread((char *)&config.netid,sizeof(int),1);
#ifdef DEBUG
  printf("netid: %4d\n",config->netid);
#endif
  mem_file->fread((char *)&config.nboot,sizeof(int),1);
#ifdef DEBUG
  printf("nboot: %4d\n",config.nboot);
#endif
  // input layer 
  mem_file->fread((char *)&config.nin,sizeof(int),1);
#ifdef DEBUG
  printf("nin: %4d\n",config.nin);
#endif
  for (int i=1;i<=config.nin;i++)
    {
      //inout=new INOUT();
      INOUT inout;

      mem_file->fread((char *)&inout.type,sizeof(int),1);
#ifdef DEBUG
      printf("inout->type: %d\n",inout.type);
#endif
      mem_file->fread((char *)&inout.scalenr_in,sizeof(int),1);
      inout.scalpar=read_vector(mem_file);
      config.inputlist.push_back(inout);
    }

  mem_file->fread((char *)&config.nlayer,sizeof(int),1);
#ifdef DEBUG
  printf("nlayer: %4d\n",config.nlayer);
#endif

  for (int i=1;i<=config.nlayer;i++)
    {
      //      layer=new LAYER();
      LAYER layer;

      mem_file->fread((char *)&layer.nnodes,sizeof(int),1);
      mem_file->fread((char *)&layer.ttype,sizeof(int),1);
      config.layerlist.push_back(layer);

    }

  mem_file->fread((char *)&config.nout,sizeof(int),1);
#ifdef DEBUG
  printf("nout: %4d\n",config.nout);
#endif
  for (int i=1;i<=config.nout;i++)
    {
      //inout=new INOUT();
      INOUT inout;

      mem_file->fread((char *)&inout.type,sizeof(int),1);
      mem_file->fread((char *)&inout.scalenr_out,sizeof(int),1);
      inout.scalpar=read_vector(mem_file);
      config.outputlist.push_back(inout);
    }

  for (int nb=1;nb<=config.nboot;nb++)
    {
#ifdef DEBUG
      printf("%d\n",nb);
#endif
      if (nb==1) {
	nn_index=read_nn_control(&control1);
	assert ((config.nin==control1.nin));
	assert ((config.nout==control1.nout));
      }
      else 
	{
	  nn_index=read_nn_control(&control2);
	  if (memcmp(&control1,&control2,sizeof(NN_CONTROL))!=0)
	    {
	      std::cout << "NN configurations do not compare [readnet()]" << std::endl;
	    }
	}

      //pnn_net=new NN_WEIGHTS();	
  
      NN_WEIGHTS pnn_net;

      pnn_net.index=nn_index;

      //std::cout << nb << " " << nn_index << std::endl; 
      for (int i=1;i<=config.nlayer;i++)
	{
	  WEIGHTS weights;
	  //weights=new WEIGHTS;
	  weights.w=read_matrix(mem_file);
	  weights.b=read_vector(mem_file);
	  pnn_net.weightslist.push_back(weights);
	  //std::cout <<"i: " << i << std::endl; 
	  //std::cout << weights.w->cols <<" " << weights.w->rows << std::endl;
	  //std::cout << weights.w->rows << std::endl;

	}
      //std::cout << "outside loop\n";
      nn_netlist.push_back(pnn_net);
      //std::cout << "end loop\n";

    }
  //printf("Done\n");
  return(1);
}

// this one is used by SP and Rosetta
MATRIX *NN_MODEL::nn_forward(RosettaInput *rosettainput)
{
  INOUT inout;
  VECTOR *inp;
  inp=new VECTOR(config.nin);

  /*convert input data into inp*/
  for (int i=1;i<=config.nin;i++)
    {
      inout=config.inputlist[i-1];

      switch (inout.type)
	{
	case GRAVEL:
	  {
	    inout.scale_in(inout.scalenr_in,&rosettainput->gravel,inout.scalpar,&inp->vector[i]);
	    break;
	  }
	case SAND:
	  {
	    inout.scale_in(inout.scalenr_in,&rosettainput->sand,inout.scalpar,&inp->vector[i]);
	    break;
	  }
	case SILT:
	  {
	    inout.scale_in(inout.scalenr_in,&rosettainput->silt,inout.scalpar,&inp->vector[i]);
	    break;
	  }
	case CLAY:
	  {
	    inout.scale_in(inout.scalenr_in,&rosettainput->clay,inout.scalpar,&inp->vector[i]);
	    break;
	  }
	case BD:
	  {
	    inout.scale_in(inout.scalenr_in,&rosettainput->bd,inout.scalpar,&inp->vector[i]);
	    break;
	  }
	case TH33:
	  {
	    inout.scale_in(inout.scalenr_in,&rosettainput->th33,inout.scalpar,&inp->vector[i]);
	    break;
	  }
	case TH15:
	  {
	    inout.scale_in(inout.scalenr_in,&rosettainput->th1500,inout.scalpar,&inp->vector[i]);
	    break;
	  }
	case VGTHR:
	  {
	    inout.scale_in(inout.scalenr_in,&rosettainput->vgthr,inout.scalpar,&inp->vector[i]);
	    break;
	  }
	case VGTHS:
	  {
	    inout.scale_in(inout.scalenr_in,&rosettainput->vgths,inout.scalpar,&inp->vector[i]);
	    break;
	  }
	case VGALP:
	  {
	    inout.scale_in(inout.scalenr_in,&rosettainput->vgalp,inout.scalpar,&inp->vector[i]);
	    break;
	  }	
	case VGNPAR:
	  {
	    inout.scale_in(inout.scalenr_in,&rosettainput->vgnpar,inout.scalpar,&inp->vector[i]);
	    break;
	  }
	default:
	  {
	    std::cout << "Error in nn_forward" << std::endl;
	  }
	}
    }
  
  // not a recursive, but overloaded call to nn_forward(VECTOR *)
  return(nn_forward(inp));
}

MATRIX *NN_MODEL::nn_forward(VECTOR *inp)
{
  int nb;
  INOUT inout;
  //int i,k;
  LAYER *layer;

  assert(inp);

#ifdef DEBUG
  printf("number of input variables: %8d\n",config->nin);
#endif
	
  MATRIX *output;
  VECTOR *temp1;
  VECTOR *temp;
  NN_WEIGHTS *wp;
  WEIGHTS *weights;

  wp=&nn_netlist[0];
  nb=config.nboot;
  output=new MATRIX(nb,config.nout);

  // check out the dimensions of this two layer NN and allocate workspace
  weights=&wp->weightslist[0];
  temp=new VECTOR(weights->b->rows);

  weights=&wp->weightslist[1];
  temp1=new VECTOR(weights->b->rows);

  for (int i=1;i<=nb;i++){


    // layer 1.... (hidden layer)
    wp=&nn_netlist[i-1];
    weights=&wp->weightslist[0];
    layer=&config.layerlist[0];

    //std::cout << "i" << i << "\n";
    //std::cout << "w" << weights->w->rows<< " "<< weights->w->cols << "\n";
    //std::cout << "inp" << inp->rows << "\n";
    //std::cout << "temp" << temp->rows << "\n";

    matrix_vector_mul(weights->w,inp,temp);
    vector_add(weights->b,temp,temp);
    layer->tfunc(layer->ttype,temp,temp); 

    // layer 2.... (output layer)
    weights=&wp->weightslist[1];
    layer=&config.layerlist[1];
    matrix_vector_mul(weights->w,temp,temp1);
    vector_add(weights->b,temp1,temp1);
    layer->tfunc(layer->ttype,temp1,temp1); 

    // rescale output layer
    for (int k=1;k<=config.nout;k++)
      {
	inout=config.outputlist[k-1];
	inout.scale_out(inout.scalenr_out,&temp1->vector[k],inout.scalpar,&temp1->vector[k]);
	//	std::cout << "temp1  " << temp1->rows << "\n";
	//	std::cout << "output " << output->rows<< " "<< output->cols << "\n";
	output->matrix[i][k]=temp1->vector[k];
	//	std::cout << output->matrix[i][k] << std::endl;
      }
  }

  delete inp;
  delete temp1;
  delete temp;

  return(output);
}


int NN_MODEL::calc_avg(MATRIX *output,VECTOR *average,VECTOR *stddev,MATRIX *cmat)
{
  // int i,j,nb;
  VECTOR *temp;

  temp=new VECTOR(config.nout);
  for (int i=1;i<=config.nout;i++) 
    {
      average->vector[i]=0.0;
      stddev->vector[i]=0.0;

      for (int j=1;j<=config.nout;j++) 
	{
	  cmat->matrix[i][j]=0.0;
	}
    }

  for (int nb=1;nb<=config.nboot;nb++)
    {
      for (int i=1;i<=config.nout;i++)
	{
	  average->vector[i]+=output->matrix[nb][i];
	}
    }

  for (int i=1;i<=config.nout;i++) 
    {
      average->vector[i]/=config.nboot;
    }

  for (int nb=1;nb<=config.nboot;nb++)
    {
      for (int i=1;i<=config.nout;i++)
	{
	  temp->vector[i]=(output->matrix[nb][i]-average->vector[i]);
	  stddev->vector[i]+=temp->vector[i]*temp->vector[i];

	  for (int j=1;j<=i;j++)
	    {
	      cmat->matrix[i][j]+=temp->vector[i]*temp->vector[j];
	    }
	}
    }
  for (int i=1;i<=config.nout;i++) 
    {
      stddev->vector[i]/=(config.nboot-1);
      stddev->vector[i]=sqrt(stddev->vector[i]);

      for (int j=1;j<=i;j++) 
	{
	  cmat->matrix[i][j]/=(config.nboot-1);
	}
    }

  for (int i=1;i<=config.nout;i++) 
    {
      for (int j=i+1;j<=config.nout;j++) 
	{
	  cmat->matrix[i][j]=cmat->matrix[j][i]/(stddev->vector[i]*stddev->vector[j]);
	}
    }

  delete temp;

  return(0);
}

