#include "CImg.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


using namespace cimg_library;
using namespace std;


/*unsigned short MIP(unsigned short* data,int x,int y,int dimX,int dimY,int dimZ)
{
	unsigned short m = getValue(data,x,y,0,dimX,dimY,dimZ);
	for(int i=0;i<dimZ;i++)
	{
		m = max(m,getValue(data,x,y,i,dimX,dimY,dimZ));
	}
	return m;
}

unsigned short MinIP(unsigned short* data,int x,int y,int dimX,int dimY,int dimZ)
{
	unsigned short m = getValue(data,x,y,0,dimX,dimY,dimZ);
	for(int i=0;i<dimZ;i++)
	{
		m = min(m,getValue(data,x,y,i,dimX,dimY,dimZ));
	}
	return m;
}*/


int main(int argc, char *argv[])
{
	//TP1 EXO1
	const char *const filename = "Image/accuracy_adult01.img";
	float voxel_size[3];

	CImg<unsigned int> img;
	img.load_analyze(filename, voxel_size);

	cout <<  voxel_size[0] << endl;
	cout <<  voxel_size[1] << endl;
	cout <<  voxel_size[2] << endl;

	cout << "width : " << img.width() << endl;
	cout << "height : " << img.height() << endl;
	cout << "depth : " << img.depth() << endl;
	cout << "min : " << img.min() << endl;
	cout << "max : " << img.max() << endl;


	cout << img(256, 256, 12) << endl;


	img.threshold(50, false, true); //on seuil
	img.erode(5, 5, 5); //on érode
	img.label(true, 0); //on labélise

	int histogramme[img.max()] = {0};

	//on fais l'histogramme de l'image
	for(int i = 0; i < img.width()-1; i++)
	{
		for (int j = 0; j < img.height()-1; j++)
		{
			for (int k = 0; k < img.depth()-1; k++)
			{
				histogramme[*img.data(i, j, k)]++;
			}
		}
	}

	int seuil = 0;
	int valeur = 0;

	//on cherche la plus haute valeur de l'histogramme 
	// et on enregistre la valeur
	for (int i = 1; i < img.max(); i++)
	{
		if(histogramme[i] > seuil)
		{
			seuil = histogramme[i];
			valeur = i;
		}
	}

	//si le pixel n'a pas la valeur voulu par rapport
	// à l'histogramme alors on met la valeur à 0
	for(int i = 0; i < img.width()-1; i++)
	{
		for (int j = 0; j < img.height()-1; j++)
		{
			for (int k = 0; k < img.depth()-1; k++)
			{
				if(*img.data(i, j, k) != valeur)
					img(i, j, k) = 0;
			}
		}
	}

	img.dilate(5, 5, 5); //on dilate

	//TP1 EXO2
	int z = 0;

	CImgDisplay disp(img.get_slice(z), "Test");
	disp.resize(512,512);

	while (!disp.is_closed())
	{
		disp.set_wheel();
		disp.wait();

		if(disp.key() == XK_Left || disp.wheel() < 0)
		{
			if(z > 0)
			{
				z--;
			}
		}

		else if(disp.key() == XK_Right || disp.wheel() > 0)
		{
			if(z < img.depth()-1)
			{
				z++;
			}
		}

		else if(disp.key() == XK_Escape)
		{
			disp.close();
		}

		else if(disp.key() == XK_m)
		{
			img.blur(1.20);
		}

		else if(disp.key() == XK_a)
		{
			//Mode AIP
			CImg<int> imgAIP(img.height(), img.width());
			int som = 0;

			//int tab[img.width()][img.height()] = {0};

			for(int i = 0; i < img.width(); i++)
			{
				for(int j = 0; j < img.height(); j++)
				{
					for(int k = 0; k < img.depth(); k++)
					{
						som += img(i, j, k);
					}

					imgAIP(i, j) = som / img.depth();
					//tab[img.width()][img.height()] = som / img.depth();
					som = 0;
				}
			}

			CImgDisplay AIP(imgAIP, "Mode AIP");

			while (!AIP.is_closed())
			{
				AIP.wait();

				if(AIP.key() == XK_Escape)
					AIP.close();
			}
		}

		else if(disp.key() == XK_b)
		{
			//Mode MIP
			CImg<int> imgMIP(img.height(), img.width());
			int max = -1;

			for(int i = 0; i < img.width(); i++)
			{
				for(int j = 0; j < img.height(); j++)
				{
					for(int k = 0; k < img.depth(); k++)
					{
						if(max < img(i, j, k))
							max = img(i, j, k);
					}

					imgMIP(i, j) = max;
					//tab[img.width()][img.height()] = som / img.depth();
					max = -1;
				}
			}

			CImgDisplay MIP(imgMIP, "Mode MIP");

			while (!MIP.is_closed())
			{
				MIP.wait();

				if(MIP.key() == XK_Escape)
					MIP.close();
			}
		}

		int x = disp.mouse_x();
		int y = disp.mouse_y();

		//cout << x << "  " << y << endl;

		CImg<> rdr_img = img.get_slice(z);
		rdr_img.resize(512,512);
		disp.display(rdr_img);
	}


	return 0;
}

