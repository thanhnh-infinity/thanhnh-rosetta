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
// matrix.c : implementation file
//

#include <stdio.h>
//#include <malloc.h>
#include <iostream>
#include <cassert>
#include <stdlib.h>

//#include <stdio.h>
#include "memory_file.h"
#include "matrix.h"

/*-----------------------------------------------------------------------*/

MATRIX::MATRIX(int nrows, int ncols)
{
	
  rows=nrows;
  cols=ncols;
  matrix=new double*[(unsigned) (rows)];
  if (!matrix) 
    {
      std::cout<< "allocation failure 1 in matrix()" << std::endl;
      exit(1);
    }
  matrix -= 1;
  for(int i=1;i<=rows;i++) {
    matrix[i]=new double[(unsigned) (cols)];
    if (!matrix[i])
      {
	std::cout << "allocation failure 2 in matrix()" << std::endl;
	exit(1);
      }
    matrix[i] -= 1;
  }
}


MATRIX::~MATRIX()
{
  int nrl,ncl;	
    
  nrl=1;
  ncl=1;

  for(int i=rows;i>=nrl;i--)
    {
      if (matrix[i]) delete [] (matrix[i]+ncl);
    }

  if (matrix) delete [] (matrix+nrl);

}
/*-----------------------------------------------------------------------*/

VECTOR::VECTOR(int nrows)
{
  rows=nrows;
  vector=new double[(unsigned) (rows)];
  if (!vector) 
    {
      std::cout << "allocation failure in vector()" << std::endl;
      exit(1);
    }
  vector-=1;
}

VECTOR::~VECTOR()
{

  delete [] (vector+1);

}
/*-----------------------------------------------------------------------*/
MATRIX *read_matrix(Memory_File *mem_file)
{
  int i,j;
  MATRIX *m;
  int rows,cols;


  mem_file->fread((char *)&rows,sizeof(int),1);
  mem_file->fread((char *)&cols,sizeof(int),1);

  m=new MATRIX(rows,cols);
  m->rows=rows;
  m->cols=cols;

  for (j=1;j<=m->cols;j++){
    for (i=1;i<=m->rows;i++){
      mem_file->fread((char *)&m->matrix[i][j],sizeof(double),1);

    }
  }
  return(m);
}
/*-----------------------------------------------------------------------*/
VECTOR *read_vector(Memory_File *mem_file)
{
  int i;
  VECTOR *v;
  int rows,cols;

  mem_file->fread((char *)&rows,sizeof(int),1);
  mem_file->fread((char *)&cols,sizeof(int),1);
  /*	printf("%d %d\n",rows,cols);*/
  if (cols!=1) 
    {
      std::cout << rows << "  " << cols << std::endl;
      std::cout << "cols not equal to 1 in read_vector" << std::endl;
    }

  v=new VECTOR(rows);

  for (i=1;i<=v->rows;i++){
    mem_file->fread((char *)&v->vector[i],sizeof(double),1);
  }
  return(v);
}

/*-----------------------------------------------------------------------*/
int matrix_mul(MATRIX *m1, MATRIX *m2, MATRIX *m3)
{
  assert((m1->cols==m2->rows)); 
  assert((m1->rows==m3->rows));
  assert((m2->cols==m3->cols));

  for (int i=1;i<=m2->cols;i++){
    for (int j=1;j<=m1->rows;j++){
      m3->matrix[j][i]=0.0;
      for (int k=1;k<=m2->cols;k++){
	m3->matrix[j][i]+=m1->matrix[j][k]*m2->matrix[k][i];
      }
    }

  }
  return(0);
}
/*-----------------------------------------------------------------------*/
int matrix_add(MATRIX *m1, MATRIX *m2, MATRIX *m3)
{
  assert ((m1->cols==m2->cols));
  assert ((m1->cols==m3->cols));
  assert ((m1->rows==m2->rows));  
  assert ((m1->rows==m3->rows));

  for (int i=1;i<=m2->cols;i++){
    for (int j=1;j<=m1->rows;j++){
      m3->matrix[j][i]=m1->matrix[j][i]+m2->matrix[j][i];
    }
    
  }
  return(0);
}
/*-----------------------------------------------------------------------*/
int matrix_vector_mul(MATRIX *m1,VECTOR *v1,VECTOR *v2)
{
  assert ((m1->cols==v1->rows));
  assert ((m1->rows==v2->rows));
  
  for (int j=1;j<=m1->rows;j++){
    v2->vector[j]=0.0;
    for (int k=1;k<=v1->rows;k++){
      v2->vector[j]+=m1->matrix[j][k]*v1->vector[k];
    }
  }
  return(0);
}
/*-----------------------------------------------------------------------*/

int vector_add(VECTOR *v1,VECTOR *v2,VECTOR *v3)
{
  assert  ((v1->rows==v2->rows));
  assert ((v1->rows==v3->rows));
  
  for (int j=1;j<=v1->rows;j++)
    {
      v3->vector[j]=v1->vector[j]+v2->vector[j];
    }
  return(0);
}
