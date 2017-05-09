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
#ifndef TXT_CLASS_000
#define TXT_CLASS_000

#define TINY 0.01
#define MIN_TXT_SUM	99.0
#define MAX_TXT_SUM	101.0

#include <string> 
#include <iostream>

enum TXT_INDEX
  {
    
    TXT_CLAY=0,
    TXT_CLAYLOAM=1,
    TXT_LOAM=2,
    TXT_LOAMYSAND=3,
    TXT_SAND=4,
    TXT_SANDYCLAY=5,
    TXT_SANDYCLAYLOAM=6,
    TXT_SANDYLOAM=7,
    TXT_SILT=8, 
    TXT_SILTYCLAY=9,
    TXT_SILTYCLAYLOAM=10,
    TXT_SILTYLOAM=11,
    TXT_UNKNOWN=12
  };

enum TXT_HYDPAR
  {
    TXT_THR=0,
    TXT_THR_STD=1,
    TXT_THS=2,
    TXT_THS_STD=3,
    TXT_ALPHA=4,
    TXT_ALPHA_STD=5,
    TXT_NPAR=6,
    TXT_NPAR_STD=7,
    TXT_KS=8,
    TXT_KS_STD=9
  };

class TXT_CLASS
{

 public:
  
  TXT_CLASS(const double txt_matrix[13][10]);
  ~TXT_CLASS();

  /* m_clay_string= wxT("Clay"); */
  /* m_clayloam_string=wxT("Clay Loam"); */
  /* m_loam_string=wxT("Loam"); */
  /* m_loamysand_string=wxT("Loamy Sand"); */
  /* m_sand_string=wxT("Sand"); */
  /* m_sandyclay_string=wxT("Sandy Clay"); */
  /* m_sandyclayloam_string=wxT("Sandy Clay Loam"); */
  /* m_sandyloam_string=wxT("Sandy Loam"); */
  /* m_silt_string=wxT("Silt"); */
  /* m_siltyclay_string=wxT("Silty Clay"); */
  /* m_siltyclayloam_string=wxT("Silty Clay Loam"); */
  /* m_siltyloam_string=wxT("Silty Loam"); */
  /* m_txtunknown_string=wxT("Unknown"); */

  
  std::string m_clay_string;
  std::string m_clayloam_string;
  std::string m_loam_string;
  std::string m_loamysand_string;
  std::string m_sand_string;
  std::string m_sandyclay_string;
  std::string m_sandyclayloam_string;
  std::string m_sandyloam_string;
  std::string m_silt_string;
  std::string m_siltyclay_string;
  std::string m_siltyclayloam_string;
  std::string m_siltyloam_string;
  std::string m_txtunknown_string;

  double Get_Value(TXT_INDEX index, TXT_HYDPAR hyd_par)
  {
    if ((index<0) || ( index >12))
      {
	std::cout <<"ERROR index " << index << std::endl;
      }
    assert((index>=0) && (index<=12));
    assert((hyd_par>=0) && (hyd_par<=9));
    return(txt_class[index][hyd_par]);  
  }

  bool is_clay(double,double);
  bool is_siltyclay(double,double);
  bool is_siltyclayloam(double,double);
  bool is_clayloam(double,double);
  bool is_loam(double,double);
  bool is_siltyloam(double,double);
  bool is_silt(double,double);
  bool is_sandyclay(double,double);
  bool is_sandyclayloam(double,double);
  bool is_sandyloam(double,double);
  bool is_loamysand(double,double);
  bool is_sand(double,double);
  bool is_valid_ssc(double, double, double);
  int normalize_texture(double sand, double silt, double clay, double *sand_n, double *silt_n, double *clay_n);

  TXT_INDEX determine_texture_class(double sand, double silt, double clay);

  //  wxString determine_name(TXT_INDEX txt_index);

  std::string determine_texture_name(double sand, double silt, double clay)
  {
    TXT_INDEX txt_index=determine_texture_class( sand,silt, clay);
    std::string s;
    switch (txt_index) 
      {
      case TXT_CLAY : s = m_clay_string; break;
      case TXT_CLAYLOAM : s = m_clayloam_string; break;
      case TXT_LOAM : s = m_loam_string; break;
      case TXT_LOAMYSAND : s = m_loamysand_string; break;
      case TXT_SAND : s = m_sand_string; break;
      case TXT_SANDYCLAY : s = m_sandyclay_string; break;
      case TXT_SANDYCLAYLOAM : s = m_sandyclayloam_string; break;
      case TXT_SANDYLOAM : s = m_sandyloam_string; break;
      case TXT_SILT : s =m_silt_string; break;
      case TXT_SILTYCLAY : s=m_siltyclay_string; break;
      case TXT_SILTYCLAYLOAM : s=m_siltyclayloam_string; break;
      case TXT_SILTYLOAM :s = m_siltyloam_string; break;
      default: s = m_txtunknown_string; break;
      }    
    return(s);
  }

  double txt_class[13][10];

};

#endif
