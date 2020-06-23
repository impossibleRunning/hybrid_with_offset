#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "map.h"
#include<time.h>
clock_t p0, p1, p2, p3, p4, p5, p6;

void nikon_blk();
void linux_blk();
void digicam_new_blk();


void linux_hy();
void digicam_new_hy();
void nikon_hy();
void print_hybrid();
void hybrid_open();
int repeat(int a[PAGES_PER_BLOCK], int lsn);

double calculateTime(int read, int write, int erase);

FREEBLK freeblk = BLOCKS_PER_DEVICE - 1;
SpareData spare[BLOCKS_PER_DEVICE*PAGES_PER_BLOCK];
BlockMapTbl table;
FILE *devicefp;
int lbn;
int used_blk = 0;
int erase = 0;
int write = 0;
int read = 0;

int copy = 0;

void random() {

	hybrid_open();
	FILE *fpWrite = fopen("E:\\FAST\\random.txt", "r");
	int i, a;
	if (fpWrite == NULL)
	{
		printf("error\n");
		//		return -1;
	}
	for (i = 0; i < 1000; i++) //69576
	{
		fscanf(fpWrite, "%d", &a);
		hybrid_write(a);
	}

	fclose(fpWrite);
	printf("-----linux_blk-----\n");
	printf("erase: %d\n", erase);
	printf("read: %d\n", read);
	printf("write: %d\n", write);
	printf("time: %f\n", calculateTime(read, write, erase));

}

int main() {
	
	printf("hybrid with offset\n");
	
	digicam_new_hy();
	p4 = clock();
	double time4 = ((double)p4 - p3) / CLK_TCK;
	printf("time: %F\n", time4);
	printf("calculateed time: %f\n", calculateTime(read, write, erase, copy));


	linux_hy();
	p5 = clock();
	double time5 = ((double)p5 - p4) / CLK_TCK;
	printf("time: %F\n", time5);
	printf("calculateed time: %f\n", calculateTime(read, write, erase, copy));

	
	nikon_hy();
	p6 = clock();
	double time6 = ((double)p6 - p5) / CLK_TCK;
	printf("time: %F\n", time6);
	printf("calculateed time: %f\n", calculateTime(read, write, erase, copy));
	
	/*
	hybrid_open();
	
	hybrid_write(3);
	hybrid_write(3);
	hybrid_write(3);
	hybrid_write(4);
	hybrid_write(4);
	
	hybrid_write(3);
	
	hybrid_write(2);
	
	hybrid_write(2);
	*/


	//print_hybrid();

	//random();
	

	system("pause");
}


double calculateTime(int read, int write, int erase) {
	double total = read * 0.02 + write * 0.2 + erase * 2 ;
	return total;
}

