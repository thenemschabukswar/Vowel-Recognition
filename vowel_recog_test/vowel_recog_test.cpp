
// vowel_recog_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>


using namespace std;

int main()
{
	unsigned int pos=0, count=0, std_amp=10000, N=320,p=12, min_pos=0;
	double temp=0, sum=0, avg_dc=0,max_amp=0,factor=0, h[320],alpha[5][13][13],A[5][13],K[5][13],R[5][13],C_test[5][13],C_ref[5][13],E[5][13], buffer[1600], start_pos, samples[5][320];
	double w[13]={0.0, 1.0,3.0,7.0,13.0,19.0,22.0,25.0,33.0,42.0,50.0,56.0,61.0};
	long double tokhura_distance, cepstral_distance, final_dist[5];
	ifstream fin;
	ofstream fot;
	string fil_name_with_ext, fil_name, rest_str;
	fil_name = "204101047_";

	//Part 1: Recording silence to calculate DC shift
	cout<<"Recording silence to calculate DC shift. Press enter after recording is stopped."<<endl;
	system("Recording_Module.exe 2 silence.wav silence.txt");
	fin.open("silence.txt");

	//Throw away the first-second/16000 samples of recording
	for (int i=0;i<16000;i++)
		fin>>temp;
	temp = 0;

	//Calculate the DC shift on 10 frames/3200 samples
	sum = 0;
	for(int i=0;i<3200;i++)
	{
		fin>>temp;
		sum += temp;
	}
	fin.close();
	avg_dc = sum/3200;

	
	//Take test file
	cout<<"Testing program."<<endl;
	cout<<"Enter the file name after underscore[For 204101047_a_05.txt, enter a_05.txt]:";
	cin>>rest_str;
	fil_name += rest_str;
	fin.open(fil_name);
	if(!fin.is_open())
	{
		cout<<"No such file exists"<<endl;
		return 0;
	}
	cout<<"File "<<fil_name<<" read in progress."<<endl;
	fot.open("DC_adjusted.txt");
	//Throw away first 1000 samples
	for (int i=0;i<1000;i++)
		fin>>temp;
	temp = 0;
	while(!fin.eof())
	{
		fin>>temp;
		temp -= avg_dc;
		fot<<temp<<"\n";
	}
	cout<<"Done with DC shift"<<endl;
	fin.close();
	fot.close();

	//Normalization
	fin.open("DC_adjusted.txt");
	max_amp = 0;
	while(!fin.eof())
	{
		fin>>temp;
		if(abs(temp) > max_amp)
		{
			max_amp = abs(temp);
			pos = count;
		}
		count++;
	}
	fin.close();
	factor = std_amp / max_amp;
	fin.open("DC_adjusted.txt");
	fot.open("Normalized.txt");
	while(!fin.eof())
	{
		fin>>temp;
		temp *= factor;
		fot<<temp<<"\n";
	}
	fin.close();
	fot.close();

	fin.open("Normalized.txt");
	//fot.open("5_frames.txt");
	start_pos = pos - 800;
			
	//Position should not be negative
	if(start_pos < 0)
		start_pos = 0;
	//Delete first start_pos - 1 samples
	for(unsigned long int i=0;i<start_pos;i++)
		fin>>temp;
	temp=0;

	//buffer has the 1600 samples
	for(unsigned int i=0;i<1600;i++)
		fin>>buffer[i];
	fin.close();

	//Save the values of Hamming window in h
	for(unsigned int i=0;i<N;i++)
		h[i] = 0.54 - (0.46 * cos((2*3.14*i)/319));

	for(int i=0;i<5;i++)
	{
		for(int j=0;j<320;j++)
		{
			samples[i][j] = buffer[i*320+j];
			samples[i][j] = h[j] * samples[i][j];
		}
	}
	for(unsigned int l=0;l<5;l++)
	{
		for(unsigned int i=0;i<13;i++)
		{
			for(int j=0;j<13;j++)
				alpha[l][i][j] = 0;
			A[l][i] = 0;
			K[l][i] = 0;
			E[l][i] = 0;
			R[l][i] = 0;
			C_test[l][i] = 0;
		}
	}


	//Calculating Ri's
	for (unsigned int i = 0 ; i<5; i++)
	{
		for(int j=0;j<=p;j++)
		{
		R[i][j] = 0;
		sum = 0;
		for(int m=0;m<5;m++)
			for (int l=0; l < (N-i-1);l++)
				sum += samples[m][l] * samples[m][l+j];
		R[i][j] = sum;
		//fot2<<R[i]<<"\n";
		}
	}
	/*
	//Calculating Ai's
	for(int i=0;i<5;i++)
		E[i][0] = R[i][0];

	for(int i=0;i<5;i++)
	{
		for (int j=1;j<=p;j++)
		{
			A[i][j] = 0;
			sum = 0;
			for(int l=1;l<j;l++)
				sum += alpha[i][j-1][l] * R[i][j-l];
			K[i][j] = (R[i][j] - sum) / E[i][j-1];
			alpha[i][j][j] = K[i][j];
			for(int l=1;l<i;l++)
				alpha[i][j][l] = alpha[i][j-1][l] - (K[i][j] * alpha[i][j-1][j-l]);
			E[i][j] = (1 - (K[i][j] * K[i][j])) * E[i][j-1];
		}
	}
	for(int i=0;i<5;i++)
	{
		for(int j=1;j<=p;j++)
			A[i][j] = alpha[i][p][j];
		//fot2<<A[i]<<"\n";
	}
	*/
	for (int i=0;i<5;i++)
	E[i][0] = R[i][0];
	
	for (int z=0;z<5;z++)
	{
	for (int i=1;i<=p;i++)
	{
		sum = 0;
		for(int j=1;j<i;j++)
			sum += alpha[z][i-1][j] * R[z][i-j];
		K[z][i] = (R[z][i] - sum) / E[z][i-1];
		alpha[z][i][i] = K[z][i];

		for(int j=1;j<i;j++)
			alpha[z][i][j] = alpha[z][i-1][j] - (K[z][i] * alpha[z][i-1][i-j]);
		E[z][i] = (1 - (K[z][i] * K[z][i])) * E[z][i-1];
	}
	}
	for(int z=0;z<5;z++)
	for(int i=1;i<=p;i++)
	{
		A[z][i] = alpha[z][p][i];
		//fot<<A[z][i]<<"\n";
	}
	
	//Invert A[i]'s for C[i]'s
	for(int i=0;i<5;i++)
		for(int j=1;j<=p;j++)
			A[i][j] = -A[i][j];

	//Calculate Ci's
	fot.open("final_C.txt");
	for(int i=0;i<5;i++)
		C_test[i][0] = log(R[i][0]);

	for(int i=0;i<5;i++)
	{
		for (int j=1;j<=p;j++)
		{
			sum = 0;
			for(int l=1;l<i;l++)
				sum += (l * C_test[i][l] * A[i][j-l]) / j;
			C_test[i][j] = A[i][j] + sum;
			fot<<C_test[i][j]<<"\t";
		}
		fot<<"\n";
	}
	fot.close();
	cout<<"Works till here!"<<endl;

	fil_name = "";
	fil_name = "reference_";
	char name;
	//names[] = "a", "e", "i", "o", "u"; 
	
	//Calculate Tokhura distance for given test vectors against reference vectors
	for(int i=0;i<5;i++)
	{
		//For the 5 vowels
		switch(i)
		{
			case 0:
				name = 'a';
				break;
			case 1:
				name = 'e';
				break;
			case 2:
				name = 'i';
				break;
			case 3:
				name = 'o';
				break;
			case 4:
				name = 'u';
		}

		fil_name_with_ext = fil_name + name + ".txt";
		fin.open(fil_name_with_ext);
		for(int j=0;j<5;j++)
		{
			//For the 5 vectors in each vowel

			for(int l=1;l<13;l++)
			{
				//For the 12 columns in each vector
				fin>>C_ref[j][l];
			}
		}
		tokhura_distance = 0;
		cepstral_distance = 0;
		for(int j=0;j<5;j++)
		{
			for (int l=1;l<13;l++)
			{
				cepstral_distance += w[l]*pow((C_ref[j][l]-C_test[j][l]),2);
			}
			tokhura_distance += cepstral_distance;
		}
		fin.close();
		final_dist[i] = tokhura_distance;
		if(final_dist[i] < final_dist[min_pos])
			min_pos = i;

		
	}
	cout<<"And till the end!"<<endl;

	switch(min_pos)
		{
			case 0:
				name = 'a';
				break;
			case 1:
				name = 'e';
				break;
			case 2:
				name = 'i';
				break;
			case 3:
				name = 'o';
				break;
			case 4:
				name = 'u';
		}
	cout<<"Predicted character:"<<name<<endl;
	return 0;
}

