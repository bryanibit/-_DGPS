//#include <vector>
#include <string.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "art/UTM.h"

using namespace std;


std::vector<std::string> split(const std::string& s, char seperator)
{
	std::vector<std::string> output;

	std::string::size_type prev_pos = 0, pos = 0;

	while ((pos = s.find(seperator, pos)) != std::string::npos)
	{
		std::string substring(s.substr(prev_pos, pos - prev_pos));

		output.push_back(substring);

		prev_pos = ++pos;
	}

	output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

	return output;
}

double distance (double x, double y, double originx, double originy)
{
  return (x-originx)*(x-originx)+(y-originy)*(y-originy);
}
int main()
{
  //std::stringstream strstream;
  int count = 0;
  ifstream infile;
  infile.open("../LOG.txt");
  string s;
  std::vector<pair<double,double>> positionLL;
  while(getline(infile, s))
  {
    long int longitude_frac_part = 0;
    long int latitude_frac_part = 0;
    double latitude = 0.0;
    double longitude = 0.0;
    vector<string> s_char = split(s,',');
    if(s_char.at(0) == "$GNGGA")
    {
      double L_north=.0;
      double L_east=.0;
      int utmnum=0;
      char utmZone;
      auto iterator = s_char.at(2).c_str();
      latitude_frac_part = atol((iterator+5));
      latitude = 39.0 + (3.0 + longitude_frac_part/100000000.0)/60.0;

      auto iterator2 = s_char.at(4).c_str();
      longitude_frac_part = atol(iterator2+6);
      longitude = 118.0 + (25.0 + latitude_frac_part/100000000.0)/60.0;

      UTM::LLtoUTM(latitude,longitude,L_north,L_east,utmnum,utmZone);
      positionLL.push_back(std::make_pair(L_east, L_north));
      //cout << setprecision(10) <<longitude_frac_part/100000000.0<<endl;
      cout << "longtitude is "<< setprecision(10)<<longitude <<endl;
      cout << "latitude is "<< setprecision(10)<<latitude<<endl;
      cout << "x and y, utmnum is "<< L_east<<" "<< L_north<< " "<< utmnum<<endl;
      count++;
    }
  }
  double originx = .0;
  double originy = .0;
  double error_max = -10000;
  //double dist_ave = -1;
  // calculate the center of all of points;
  for(auto iter = positionLL.begin(); iter != positionLL.end(); ++iter)
  {
    if(iter == positionLL.begin())
    {
      originx = iter->first;
      originy = iter->second;
    }
    else
    {
      originx = (originx+(iter->first))/2;
      originy = (originy+(iter->second))/2;
    }
  }
  // find the max error;
  for(auto iter = positionLL.begin(); iter != positionLL.end(); ++iter)
  {
    double dist = distance(iter->first,iter->second,originx, originy);
    if(dist > error_max)
    {
      error_max = dist;
    }
  }
  cout << "the average or center is "<< originx << " "<< originy <<endl;
  cout <<"the square max error is "<< error_max<< endl;
  cout<< "the whole file has " <<count<<" message"<<endl;

}
