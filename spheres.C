//===========================================================
//===========================================================
//===========================================================
//
//  Molecular dynamics simulation of hardspheres
//
//===========================================================
//===========================================================
//===========================================================

#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>
#include <time.h>
#include <string.h>
#include <string>

#include "box.h"
#include "sphere.h"
#include "event.h"
#include "heap.h"
#include "read_input.h"


int main(int argc, char **argv)
{
  read_input input;
  int error = input.read(argc, argv);
  if (error) return error;

  double d, r;   // initial diameter and radius of spheres

  if(strcasecmp(input.readfile, "new")==0)
    input.readfile[0]=0;

  if (input.readfile[0]) // read in existing configuration
    {
      // read the header
      std::ifstream infile(input.readfile);
      if (!infile)
	{
	  std::cout << "error, can't open " << input.readfile  << std::endl;
	  exit(-1);
	}
      else
	{
	  int dim;
	  infile >> dim; infile.ignore(256, '\n');
	  if (dim != DIM)  // quit if dimensions don't match
	    {
	      std::cout << "error, dimensions don't match" << std::endl;
	      exit(-1);
	    }
	  infile.ignore(256, '\n');  // ignore the N 1 line
	  infile >> input.N; infile.ignore(256, '\n');
	  std::cout << "N = " << input.N << std::endl;
	  infile >> d; infile.ignore(256, '\n');
	  std::cout << "d = " << d << std::endl;
	  r = d/2.;
	  std::cout << "r = " << r << std::endl;
	}
    }
  else // create a new configuration
    {
      r = pow(input.initialpf*pow(SIZE, DIM)/(input.N*VOLUMESPHERE), 1.0/((double)(DIM)));
    }

  // build seed-tagged output filenames: base_<seed>.ext
  auto tag_filename = [](const char* name, unsigned int seed) -> std::string {
    std::string s(name);
    std::string tag = "_" + std::to_string(seed);
    size_t dot = s.rfind('.');
    if (dot != std::string::npos) s.insert(dot, tag);
    else s += tag;
    return s;
  };
  std::string writefile = tag_filename(input.writefile, input.seed);
  std::string datafile  = tag_filename(input.datafile,  input.seed);
  std::cout << "   writefile : " << writefile << std::endl;
  std::cout << "   datafile  : " << datafile  << std::endl;

  box b(input.N, r, input.growthrate, input.maxpf, input.seed);
  
  std::cout << "ngrids = " << b.ngrids << std::endl;
  std::cout << "DIM = " << DIM << std::endl;

  if(input.readfile[0])
    {
      std::cout << "Reading in positions of spheres" << std::endl;
      b.RecreateSpheres(input.readfile, input.temp);
    }
  else 
    {
      std::cout << "Creating new positions of spheres" << std::endl;
      b.CreateSpheres(input.temp);
    } 
  
  std::ofstream output(datafile);
  output.precision(16);

  while ((b.pf < input.maxpf) && (b.pressure < input.maxpressure))
    {
      b.Process(input.eventspercycle*input.N);
      output << b.pf << " " << b.pressure << " " <<
	b.energychange << " " << b.neventstot << " " << std::endl;

      b.Synchronize(true);
    }

  // equilibration: run dynamics at fixed radius after reaching target packing fraction
  if (input.equilsteps > 0)
    {
      std::cout << "Equilibrating for " << input.equilsteps << " cycles at pf = " << b.pf << "..." << std::endl;
      b.growthrate = 0.;
      for (int cyc = 0; cyc < input.equilsteps; cyc++)
        {
          b.Process(input.eventspercycle*input.N);
          b.Synchronize(true);
        }
      std::cout << "Equilibration done." << std::endl;
    }

  output.close();

  b.WriteConfiguration(writefile.c_str());
  
 
  
  return 0;
}
