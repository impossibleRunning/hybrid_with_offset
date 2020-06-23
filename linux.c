#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "map.h"
FILE *devicefp;
extern int lbn;
extern int used_blk;
extern int erase;
extern int write;
extern int read;
extern int repeat;
extern int copy;
void hybrid_open();
int hybrid_read(int lsn);
int hybrid_write(int lsn);
void linux_hy() {
	hybrid_open();
	/*
	char *blockbuf;
	char sectorbuf[SECTOR_SIZE];


	devicefp = fopen("flashmemory", "w+b");
	if (devicefp == NULL)
	{
		printf("file open error\n");
		exit(1);
	}

	//
	// flash memory의 모든 바이트를 '0xff'로 초기화한다.
	//
	blockbuf = (char *)malloc(BLOCK_SIZE);
	memset(blockbuf, 0xFF, BLOCK_SIZE);

	for (i = 0; i < BLOCKS_PER_DEVICE; i++)
	{
		fwrite(blockbuf, BLOCK_SIZE, 1, devicefp);
	}

	free(blockbuf);
	*/
	int lsn, i;
	int a;
	FILE* LBN = fopen("linux.trace", "r");


	if (LBN == NULL)
	{
		printf("error\n");
		//		return -1;
	}
	for (i = 0; i < 18900; i++) //69576
	{
		fscanf(LBN, "%d", &a);
		hybrid_write(a);
	}

	fclose(LBN);
	// ftl_read(), ftl_write() 호출하기 전에 이 함수를 반드시 호출해야 함

	//
	// call ftl_write() functions for experimental evaluation 
	// using given pattern data files
	//
	printf("-----linux_hy-----\n");
	printf("erase: %d\n", erase);
	printf("read: %d\n", read);
	printf("write: %d\n", write);
	//	getrepeat();
	printf("copy:%d\n", copy);


	//	fclose(devicefp);

	return 0;



}