//DSA Assignment 1
// Mohamad Azri Hadif bin Mohammad Rizal (A21EC0054)
// Nurunnajwa binti Zulkifli (A21EC0121)
// Nur Shuhada Safiah binti Ayob (A21EC0114)

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <cmath>
using namespace std;

const int numBlock = 8;
const int dSize = 800000;
const int bSize = 100000;


void merge(int list[], int l, int m, int r){
	int i, x, y;
	int n1 = m - l + 1;
	int n2 = r - m;

	int* L = new int[n1];
	int* R = new int[n2];

	//int L[n1], R[n2];
    for (i = 0; i < n1; i++)
       L[i] = list[l + i];
    for (x = 0; x < n2; x++)
       R[x] = list[m + 1 + x];
       
    i = 0;
    x = 0;
    y = l;
    
    while (i < n1 && x < n2) {
       if (L[i] <= R[x])
          list[y++] = L[i++];
       else
          list[y++] = R[x++];
    }
    
    while (i < n1)
       list[y++] = L[i++];
    while (x < n2)
       list[y++] = R[x++];

	delete[] L;
	delete[] R;
}

void mergeSort(int list[], int l, int r){
	if(l < r){
		int m = (l + (r-1)) / 2; 
		
		mergeSort(list, l, m);
		mergeSort(list, m+1, r);
		merge(list, l, m, r);
	}
}


void merge2(char* f1, char* f2, char* outp){
	fstream inp1(f1, ios::in | ios::binary);
	fstream inp2(f2, ios::in | ios::binary);
	fstream outf(outp, ios::out | ios::binary);
	
	int d1, d2, d3;
	inp1.read((char*)&d1, sizeof(int));
	inp2.read((char*)&d2, sizeof(int));
	
	while(!inp1.eof() && !inp2.eof()){
		
		if(d1 < d2){
			d3 = d1;
			inp1.read((char*)&d1, sizeof(int));
		}else{
			d3 = d2;
			inp2.read((char*)&d2, sizeof(int));
		}
		
		outf.write((char*)&d3, sizeof(int));

	}
	
	if(inp1.eof()){
		while(!inp2.eof()){
			d3 = d2;
			inp2.read((char*)&d2, sizeof(int));
			outf.write((char*)&d3, sizeof(int));
		}
	}
	if(inp2.eof()){
		while(!inp1.eof()){
			d3 = d1;
			inp1.read((char*)&d1, sizeof(int));
			outf.write((char*)&d3, sizeof(int));
		}
	}


	
}

void merge2(char* filename, char* outp, int cLeft, int cRight, int rSize){
	int sizeL = 0, sizeR = 0;
	int head1 = cLeft, head2 = cRight;
	int L[bSize], R[bSize];
	int list[bSize];
	int i = 0, x = 0, y = 0;

	
	ifstream inp(filename, ios::in | ios::binary);
	if (!inp) {
		cout << "Can't open file!" << endl;
		return;
	}
	
	fstream outf(outp, ios::out | ios::binary);
	
	
	inp.seekg(cLeft, ios::beg);
	inp.read((char*) (L+i), bSize * sizeof(int));
	cLeft = inp.tellg();
	sizeL = cLeft - head1;
	
	inp.seekg(cRight, ios::beg);
	inp.read((char*) (R+x), bSize * sizeof(int));
	cRight = inp.tellg();
	sizeR = cRight - head2;
	
	i = 0, x = 0;
	bool cond = true;
	bool condL, condR;
	int data;
	

	while(cond){
		condL = false;
		condR = false;
		
		while (i < bSize && x < bSize) {
			if (L[i] <= R[x]){
				//			    list[y++] = L[i++];
				outf.write((char*)(L + i), sizeof(int));
				i++;
			}
			else{
				//			    list[y++] = R[x++];
				outf.write((char*)(R + x), sizeof(int));
				x++;
			}
		}
		    
		while (i < bSize)
			outf.write((char*)(L + i), sizeof(int));

		while (x < bSize)
			outf.write((char*)(R + i), sizeof(int));
		    

		if(i == bSize && sizeL < rSize){
			for(int x=0; x<bSize; x++){
				inp.seekg(cLeft, ios::beg);
				inp.read((char*) (L+x), sizeof(int));
				cLeft = inp.tellg();
				sizeL = cLeft - head1;
			}
			i = 0;
			condL = true;
		}
		
		if(x == bSize && sizeR < rSize){
			for(int y=0; y<bSize; y++){
				inp.seekg(cRight, ios::beg);
				inp.read((char*) (R+y), sizeof(int));
				cRight = inp.tellg();
				sizeR = cRight - head2;
			}
			x = 0;
			condR = true;
		}
		
		cond = (condL && condR);
		
	}
	
	outf.close();
	inp.close();
}

void filesort(char* filename){
	
	int array[bSize];
	ifstream inp(filename, ios::in | ios::binary);
	if (!inp) {
		cout << "Can't open file!" << endl;
		return;
	}
	
	fstream outf("sortedfile.dat", ios::app | ios::binary);


	int sizereadfile;
	for(int block = 1; block <= numBlock; block++){
		for(int i=0; i<bSize; i++){
			inp.read((char*) (array+i), sizeof(int));
		}
		
		mergeSort(array, 0, bSize - 1);
		
		for(int i=0; i<bSize; i++){
			outf.write((char*) (array+i), sizeof(int));
//			outf << array[i] << " ";
		}
	}
	
	outf.close();
	inp.close();
}



int main() {
	srand((unsigned)time(NULL));
	int data;
	
	ofstream outp("verybigdata.dat", ios::out | ios::binary);
	if(!outp){
		cout << "Can't open file!" << endl;
		return 1;
	}
	
	for(int i = 0; i < dSize; i++){
		data =(rand() % 1000000);
		outp.write((char*)&data, sizeof(data));
	}
	outp.close();
	
	if (!outp.good()) {
	cout << "Error occured at writing time!" << endl;
	return 1;
	}
	
	int numrun = numBlock;
	char inp[] = "verybigdata.dat";
	char sortedfile[] = "sortedfile.dat";
	
	filesort(inp);

	
	return 0;
}

