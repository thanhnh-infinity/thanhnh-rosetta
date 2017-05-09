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

/*
 * Thanh H. Nguyen modifed for landpks project
 */
#include <stdlib.h>
#include <cassert>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <memory>

#include <sstream>
#include <string>

#include "nn_model_mem.h"
#include "rosetta.h"
#include "txt_class.h"
#include "ros_base.h"


/*
 * Run Rosetta Unix version different in window
 * $<Arg 0 : Rosetta Executablf file> <Arg 1 : Flag : old or thanhnh> <Arg 2 : Model : 2 - SSC> <full input file path> <full output file paht>
 * $rosetta old 2 input.txt output.txt
 *
 * Output is file
 *
 * Exampe : ./test_rosetta old 2 /Volumes/Workspace_ThanhNH/All_Workspace/C_Cpp_Workspace/Rosetta_Model_Application/input/4673_in.txt /Volumes/Workspace_ThanhNH/All_Workspace/C_Cpp_Workspace/Rosetta_Model_Application/output/4673_out.txt
 */

/*
 * Run Rosetta Unix by thanhnh style
 * $<Arg 0 : Rosetta Executablf file> <Arg 1 : Flag : old or thanhnh> <Arg 2 : Model : 2 - SSC> <sand value> <silt value> <clay value>
 * $rosetta thanhnh 2 80.0 15.0 5.0
 *
 * Output is JSON data
 *
 * Example : ./test_rosetta thanhnh 2 80.0 15.0 5.0
 */

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

bool BothAreSpaces(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); }

bool is_file_exist(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

std::string getPrecisionValue(double number,int pre){
     std::stringstream str;
	 str << std::fixed << std::setprecision(pre) <<number;
	 std::string str_fix = str.str();
	 return str_fix;
}


template<class ForwardIt>
ForwardIt unique(ForwardIt first, ForwardIt last)
{
    if (first == last)
        return last;

    ForwardIt result = first;
    while (++first != last) {
        if (!(*result == *first) && ++result != first) {
            *result = std::move(*first);
        }
    }
    return ++result;
}

template<class ForwardIt, class BinaryPredicate>
ForwardIt unique(ForwardIt first, ForwardIt last, BinaryPredicate p)
{
    if (first == last)
        return last;

    ForwardIt result = first;
    while (++first != last) {
        if (!p(*result, *first) && ++result != first) {
            *result = std::move(*first);
        }
    }
    return ++result;
}

int main(int argc, char* argv[])
{
  RosettaInput rosinput;
  RosettaOutput rosoutput;

  if (argc >= 2){
	  char* flag = argv[1];
	  if (strcmp(flag,"old") == 0){
		  //Read data from input file
		  if (argc != 5){
			  std::cout << "IN-CORRECT-ARGV" << std::endl;
			  return -1;
		  }
		  char* input_file_name = argv[3];
		  std::string data_write_to_file;
		  char* output_file_name = argv[4];
		  char* chModel = argv[2];
		  int model = atoi(chModel);

		  data_write_to_file = "Code    Description        Theta_r    Theta_s    Alpha      N          Ks         Ko         L          \n";
		  data_write_to_file += "                           cm3/cm3    cm3/cm3    1/cm       no units   cm/day     cm/day     no units   \n";

		  //std::ifstream infile("/Volumes/Workspace_ThanhNH/All_Workspace/C_Cpp_Workspace/Rosetta_Model_Application/input/4673_in.txt");
		  std::ifstream infile(input_file_name);
		  std::string str;
		  int i = 0 ;
		  while (std::getline(infile, str)){
			  /* 7 soil horizon layers */
			  if (i >= 2 && i < 9){
				  std::string sam_id;
				  std::string layer_id;
				  char* hz_tp;
				  char* hz_bt;
				  double sand_tot, silt_tot, clay_tot, bulk_dens, tot_C, w3cld, w15ad;

				  std::string::iterator new_end = unique(str.begin(), str.end(), BothAreSpaces);
				  str.erase(new_end, str.end());

				  std::vector<std::string> considerLayer = split(str, ' ');
				  sam_id = considerLayer[0].c_str();
				  layer_id = considerLayer[1].c_str();
				  /*
				  std::cout <<"Layer : " << considerLayer[1] << std::endl;
				  std::cout <<"Top : " << considerLayer[2] << std::endl;
				  std::cout <<"Bottom : " << considerLayer[3] << std::endl;
				  std::cout <<" Sand : " << considerLayer[4] << std::endl;
				  std::cout <<" Silt : " << considerLayer[5] << std::endl;
				  std::cout <<" Clay : " << considerLayer[6] << std::endl;
				  std::cout << "---------------------------------" << std::endl;
			      */
				  /* Calculate value */


				  ANN_MODEL ann_model=SSC;
				  if (model == 2){
					  ann_model=SSC;
					  sand_tot = atof(considerLayer[4].c_str());
					  silt_tot = atof(considerLayer[5].c_str());
					  clay_tot = atof(considerLayer[6].c_str());
					  rosinput.sand=sand_tot;
					  rosinput.silt=silt_tot;
					  rosinput.clay=clay_tot;
				  } else if (model == 3) {
					  ann_model=SSCBD;
					  sand_tot = atof(considerLayer[4].c_str());
					  silt_tot = atof(considerLayer[5].c_str());
					  clay_tot = atof(considerLayer[6].c_str());
					  rosinput.sand=sand_tot;
					  rosinput.silt=silt_tot;
					  rosinput.clay=clay_tot;
				  } else {
					  ann_model=SSC;
					  sand_tot = atof(considerLayer[4].c_str());
					  silt_tot = atof(considerLayer[5].c_str());
					  clay_tot = atof(considerLayer[6].c_str());
					  rosinput.sand=sand_tot;
					  rosinput.silt=silt_tot;
					  rosinput.clay=clay_tot;
				  }

				  if (sand_tot == -9.9 || silt_tot == -9.9 || clay_tot == -9.9){
					  std::string new_line;
					  std::string strTheta_r = getPrecisionValue(-9.9,5);
					  std::string strTheta_s = getPrecisionValue(-9.9,5);
					  std::string strAlpha = getPrecisionValue(-9.9,5);
					  std::string strNPar = getPrecisionValue( -9.9,5);
					  std::string strKs = getPrecisionValue(-9.9,5);
					  std::string strKo = getPrecisionValue(-9.9,5);
					  std::string strLPar = getPrecisionValue(-9.9,5);

					  new_line = sam_id + "     " +  layer_id + "                  " + strTheta_r + "    " + strTheta_s + "    " + strAlpha + "    " + strNPar + "  " + strKs + "   " +  strKo + "   " + strLPar;
					  //std::cout <<  new_line << std::endl;


					  data_write_to_file += new_line +"\n";
				  } else {
					  int res;
					  ROS_BASE rosetta;
					  res = rosetta.Make_Estimate(ann_model, rosinput, rosoutput);
					  if (res) {  // anything not 0 is an error, thus rosoutput is bogus
						  std::cout <<"res : " << res << std::endl;
						  std::cout << "Something went wrong in making estimates" << std::endl;
						  return -3;
								  // handle this error
					  } else {
						  /*
						  std::cout << "\n Print data result : \n" << std::endl;
						  std::cout << "theta_r " << rosoutput.vgthr << std::endl;
						  std::cout << "theta_s " << rosoutput.vgths << std::endl;
						  std::cout << "alpha   " << rosoutput.vgalp << std::endl;
						  std::cout << "n par   " << rosoutput.vgnpar << std::endl;
						  std::cout << "Ks      " << rosoutput.ks << std::endl;
						  std::cout << "Ko      " << rosoutput.unsks << std::endl;
						  std::cout << "l par   " << rosoutput.unsl << std::endl;
						  std::cout << "===================================" << std::endl;
						  */

						  std::string new_line;



						  std::string strTheta_r = getPrecisionValue(rosoutput.vgthr,5);
						  std::string strTheta_s = getPrecisionValue(rosoutput.vgths,5);
						  std::string strAlpha = getPrecisionValue(rosoutput.vgalp,5);
						  std::string strNPar = getPrecisionValue( rosoutput.vgnpar,5);
						  std::string strKs = getPrecisionValue( rosoutput.ks,5);
						  std::string strKo = getPrecisionValue( rosoutput.unsks,5);
						  std::string strLPar = getPrecisionValue(rosoutput.unsl,5);

						  new_line = sam_id + "     " +  layer_id + "                  " + strTheta_r + "    " + strTheta_s + "    " + strAlpha + "    " + strNPar + "  " + strKs + "   " +  strKo + "   " + strLPar;
						  //std::cout <<  new_line << std::endl;


						  data_write_to_file += new_line +"\n";
				       }



				  }

			  }

			  i++;
		  }

		  /* save to output file */
		  std::ofstream outFile;
		  outFile.open (output_file_name);
		  outFile << data_write_to_file;
		  outFile.close();


		  return 1;
	  } else if (strcmp(flag,"thanhnh") == 0){

		  if (argc != 6){
		  	  std::cout << "IN-CORRECT-ARGV" << std::endl;
		  	  return -1;
		  }

		  int res;
		  // allocate models
		  ROS_BASE rosetta;
		  // this is data from some source (user input, database, file)
		  rosinput.sand=atof(argv[3]);
		  rosinput.silt=atof(argv[4]);
		  rosinput.clay=atof(argv[5]);

		  //rosinput.bd=1.4;
		  //ANN_MODEL ann_model=SSCBD; //Sand Silk Clay , Bulk Density
		  ANN_MODEL ann_model=SSC;  //Sand,Silk, Clay
		  res=rosetta.Make_Estimate(ann_model, rosinput, rosoutput);
		  if (res) {  // anything not 0 is an error, thus rosoutput is bogus
			  std::cout <<"res : " << res << std::endl;
			  std::cout << "Something went wrong in making estimates" << std::endl;
			  // handle this error
		  }
		  /*
		  std::cout << "\n Print data result : \n" << std::endl;
		  std::cout << "theta_r " << rosoutput.vgthr << std::endl;
		  std::cout << "theta_s " << rosoutput.vgths << std::endl;
		  std::cout << "alpha   " << rosoutput.vgalp << std::endl;
		  std::cout << "n par   " << rosoutput.vgnpar << std::endl;
		  std::cout << "Ks      " << rosoutput.ks << std::endl;
		  std::cout << "Ko      " << rosoutput.unsks << std::endl;
		  std::cout << "l par   " << rosoutput.unsl << std::endl;
		  */
		  std::string strTheta_r = getPrecisionValue(rosoutput.vgthr,5);
		  std::string strTheta_s = getPrecisionValue(rosoutput.vgths,5);
		  std::string strAlpha = getPrecisionValue(rosoutput.vgalp,5);
		  std::string strNPar = getPrecisionValue(rosoutput.vgnpar,5);
		  std::string strKs = getPrecisionValue(rosoutput.ks,5);
		  std::string strKo = getPrecisionValue(rosoutput.unsks,5);
		  std::string strLPar = getPrecisionValue(rosoutput.unsl,5);

		  std::string data = "{\"theta_r\":" + strTheta_r + ",\"theta_s\":" + strTheta_s + ",\"alpha\":" + strAlpha + ",\"npar\":" + strNPar + ",\"Ks\":" + strKs + ",\"Ko\":" + strKo + ",\"lpar\":"  + strLPar + "}";

		  std::cout << data << std::endl;

		  return 1;
	  } else {
		  std::cout << "IN-CORRECT-ARGV" << std::endl;
		  return -1;
	  }
  } else {
	  std::cout << "IN-CORRECT-ARGV" << std::endl;
	  return -1;
  }
}
