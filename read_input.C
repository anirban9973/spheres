#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <fstream>

#include "read_input.h"

//================================================================
//
// Source File for input
//
//================================================================
int read_input::read(int argc, char * argv[])
{
  int error = 0;
  if (argc != 5)
    {
    std::cout << "Usage: spheres <inputfile> <seed> <initialpf> <maxpf>" << std::endl;
    std::cout << "  <inputfile>  : path to input parameter file" << std::endl;
    std::cout << "  <seed>       : positive integer seed for reproducible runs" << std::endl;
    std::cout << "  <initialpf>  : initial packing fraction" << std::endl;
    std::cout << "  <maxpf>      : target packing fraction" << std::endl;
    std::cout << "Example: ./spheres input 42 0.40 0.73" << std::endl;
    error = 1;
    }
  else
    {
    seed      = (unsigned int)atoi(argv[2]);
    initialpf = atof(argv[3]);
    maxpf     = atof(argv[4]);
    std::ifstream infile;
    infile.open(argv[1]);
    if(!infile)
      {
	std::cout << "Can't open " << argv[1] << " for input." << std::endl;
	error = 2;
	return error;
      } 
    else 
      {
	std::cout << "Reading input from file " << argv[1] << std::endl;
      }
    char buf[100],c;
    infile.get(buf,100,'='); infile.get(c); infile >> eventspercycle;
    infile.get(buf,100,'='); infile.get(c); infile >> N;
    infile.get(buf,100,'='); infile.get(c); infile >> temp;
    infile.get(buf,100,'='); infile.get(c); infile >> growthrate;
    infile.get(buf,100,'='); infile.get(c); infile >> maxpressure;
    infile.get(buf,100,'='); infile.get(c); infile >> equilsteps;
    infile.get(buf,100,'='); infile.get(c);
    infile.width(NAME_LEN-1); infile >> readfile;
    infile.get(buf,100,'='); infile.get(c); 
    infile.width(NAME_LEN-1); infile >> writefile;
    infile.get(buf,100,'='); infile.get(c); 
    infile.width(NAME_LEN-1); infile >> datafile;

    if(infile.eof()) 
      {
	std::cout << "Error reading input file " << argv[1] << std::endl;
	error = 3;
      }
    std::cout << "   eventspercycle : " << eventspercycle << std::endl;
    std::cout << "   N : " << N << std::endl;
    std::cout << "   initialpf : " << initialpf << std::endl;
    std::cout << "   maxpf : " << maxpf << std::endl;
    std::cout << "   temp : " << temp << std::endl;
    std::cout << "   growthrate : " << growthrate << std::endl;
    std::cout << "   maxpressure : " << maxpressure << std::endl;
    std::cout << "   equilsteps : " << equilsteps << std::endl;
    std::cout << "   seed : " << seed << std::endl;
    std::cout << "   readfile : " << readfile << std::endl;
    std::cout << "   writefile : " << writefile << std::endl;
    std::cout << "   datafile : " << datafile << std::endl;
    }
  return error;
}
