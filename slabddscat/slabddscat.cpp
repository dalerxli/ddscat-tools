//*****************************************************************************
//
//		slabddscat
//
//		v. 4.2-20131005
//
//		Copyright (C) 2013 - Nicola Ferralis - ferralis@mit.edu
//
//		Create slabs out of adjacent spheres to be used in DDSCAT
//		in the framework of "Spheres - SPH"
//
//
//		This program (source code and binaries) is free software;
//		you can redistribute it and/or modify it under the terms of the
//		GNU General Public License as published by the Free Software
//		Foundation, in version 3 of the License.
//
//		This program is distributed in the hope that it will be useful,
//		but WITHOUT ANY WARRANTY; without even the implied warranty of
//		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//		GNU General Public License for more details.
//
//		You can find a complete copy of the GNU General Public License at:
//		http://www.gnu.org/licenses/gpl.txt
//
//******************************************************************************

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
using namespace std;

int operate(char *namein);
void createNew();

char version[]="4.2-20131005";
char extension[]="dds.";
char extensiontarg[]=".targ";
char nameout[]="slab_ddscat.txt";
int maxNumSpheres = 4096;  //max number of spheres allowed by SPH_ANI_N in DDSCAT


int main(int argc, char *argv[])
{
    
    if(argc<2)
	{cout<< "\n slabddscat v."<<version<< "\n\n";
    cout<< " Usage:\n slabddscat <filename> \n";
    cout<<" To create a new source file: slabddscat -n \n\n";}
    
    if(argc>=2)
	{   if(strcmp(argv[1],"-n")==0)
            {createNew();}
        else
           {operate(argv[1]);}

    return 0;
    }
}


//OPERATE 
int operate(char *namein)

{   int numLayer = 1;
    //int maxLayers = 10;
    double X, ymin, ymax, zmin, zmax, R, prec, Y, Z;
    int zpoints, ypoints, comp;
    bool targfile = true;
    
    int numSpheres =0;
    
    ifstream infile(namein);
	
    if(!infile)
    {cout<<"\n file '"<< namein<<"' not found\n\n";
		return 0;}
    
    cout<<"\n Creating slab from: "<<namein<<"\n";
    
    cout<<"\n Layer\tcomp\tx\tymin\tymax\tzmin\tzmax\tradius\tprec\tzpnts\typnts\n";

    char* outname = new char[strlen(namein)+1];    
    
    if(targfile==false) {
        sprintf(outname,"%s%s",extension,namein);
    }
    
    else {
        sprintf(outname, "%.*s%s", (int)  strlen(namein)-4, namein,extensiontarg);
    }
    ofstream outfile(outname);
    
    
    string line;
    
    getline(infile, line);
    getline(infile, line);
    targfile=(bool) atof(line.c_str());
    
    getline(infile, line);
    getline(infile, line);
    numLayer=(int) atof(line.c_str());
    
    
    
    
    for (int n=0; n<numLayer; n++) {
        
        
        if(getline(infile, line)==NULL)
            {cout<<"\n Only "<<n<<" slabs with defined parameters were written.\n";
                break;}
        
            getline(infile, line);
            comp=atof(line.c_str());
        
            getline(infile, line);
            getline(infile, line);
            X=atof(line.c_str());
    
            getline(infile, line);
            getline(infile, line);
            ymin=atof(line.c_str());
    
            getline(infile, line);
            getline(infile, line);
            ymax=atof(line.c_str());
    
            getline(infile, line);
            getline(infile, line);
            zmin=atof(line.c_str());
    
            getline(infile, line);
            getline(infile, line);
            zmax=atof(line.c_str());

            getline(infile, line);
            getline(infile, line);
            R=atof(line.c_str());
    
            getline(infile, line);
            getline(infile, line);
            prec=atof(line.c_str());
        
        cout<<" "<<n+1<<"\t"<<comp<<"\t";
        cout<<X<<"\t"<<ymin<<"\t"<<ymax;
        cout<<"\t"<<zmin<<"\t"<<zmax<<"\t"<<R<<"\t"<<prec<<"\t";
    
        zpoints = (int) ((zmax- zmin)/ prec);
        ypoints = (int) ((ymax- ymin)/ prec);
        cout<<zpoints<<"\t"<<ypoints<<"\n";

        numSpheres += (zpoints+1)*(ypoints+1);
 
    
        if(targfile==true) {
            outfile<<"\t\t"<<numSpheres+1;
            outfile<<"\n Sphere + slab generated by slabddscat\n";
            outfile<<"  0.390732 -0.920504 -0.000001 = A_1 vector\n";
            outfile<<"  0.920504  0.390732  0.000000 = A_2 vector\n";
            outfile<<"\t x(j)        y(j)        z(j)      a(j)    IC1 IC2 IC3 th,ph,be\n";
            outfile<<"\t0.000000    0.000000    0.000000  1.000000 1    1   1   0 0 0 \n";
        }
        
        for (int i=0; i<= zpoints; i++) {
            Z= zmin + i* prec;
            for (int j=0; j<= ypoints; j++) {        
                Y=ymin+j* prec;
      
                outfile<<"\t"<<X<<"\t"<<Y<<"\t"<<Z<<"\t"<<R<<"\t"<<comp<<"\t"<<comp<<"\t"<<comp<<"\t0\t0\t0\n";
            }
        }
    
        
    }
    
    outfile.close();
    infile.close();
 
    cout<<"\n Saved in: "<<outname;
    delete outname;
    
    cout<<"\n\n Number of spheres included slab saved: "<<numSpheres<<"\n";
    if(numSpheres>maxNumSpheres)
        {cout<<" Warning: the number of spheres exceed that allowed by DDSCAT 7.3 (4096)\n";}
    
    cout<<endl;

        
return 0;}
           
           
void createNew(){
    ofstream outfile(nameout);
    outfile<<"# output file: (1: target for ddscat) (0: coordinates only)\n1\n";
    outfile<<"# number of layers\n2\n# composition - layer 1\n1\n# x - layer 1\n1.5\n# ymin - layer 1\n-2\n# ymax - layer 1\n2\n# zmin - layer 1\n-2\n# zmax - layer 1\n2\n# radius - layer 1\n0.5\n# precision - layer 1\n0.1\n";
    outfile<<"# composition - layer 2\n1\n# x - layer 2\n2.5\n# ymin - layer 2\n-2\n# ymax - layer 2\n2\n# zmin - layer 2\n-2\n# zmax - layer 2\n2\n# radius - layer 2\n0.5\n# precision - layer 2\n0.1\n";
    outfile.close();
    cout<<"\n Saved in: "<<nameout<<"\n\n";

}


