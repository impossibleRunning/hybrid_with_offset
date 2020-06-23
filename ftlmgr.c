#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "map.h"

//int dd_write(int ppn, char *pagebuf);
extern FREEBLK freeblk;
extern SpareData spare[BLOCKS_PER_DEVICE*PAGES_PER_BLOCK];
extern BlockMapTbl table;
extern FILE *devicefp;
extern int lbn;
extern int used_blk;
extern int erase;
extern int write;
extern int read;
extern int copy;

void hybrid_open() {
	erase = 0;
	write = 0;
	read = 0;
	copy = 0;
	used_blk = 0;
	freeblk = BLOCKS_PER_DEVICE - 1;
	//RESET MAPPING TABLE
	for (lbn = 0; lbn < DATABLKS_PER_DEVICE; lbn++) {
		table.entry[lbn].pbn = -1;
		table.entry[lbn].first_free_page_offset = 0;
	}

	//RESET SPARE AREA
	for (int i = 0; i <= BLOCKS_PER_DEVICE * PAGES_PER_BLOCK - 1; i++) {
		spare[i].lsn = -1;
	
	}

	for (int i = 0; i < PAGES_PER_BLOCK; i++) {
		spare[freeblk*PAGES_PER_BLOCK + i].lsn = -2;
	}
}

int hybrid_read(int lsn) {
	int pbn = table.entry[lsn / PAGES_PER_BLOCK].pbn;
	int ppn;
	for (int i = 0; i < PAGES_PER_BLOCK; i++) {
		read++;
		if (spare[pbn*PAGES_PER_BLOCK + i].lsn == lsn ) {
			ppn = pbn * PAGES_PER_BLOCK + i;
			break;
		}
	}

	return ppn;
}

int hybrid_write(int lsn) {
	int lbn = lsn / PAGES_PER_BLOCK;
	int pbn = table.entry[lbn].pbn;


	if (pbn == -1) {

		table.entry[lbn].pbn = used_blk;
		used_blk++;
		spare[table.entry[lbn].pbn*PAGES_PER_BLOCK].lsn = lsn;

		write++;
		table.entry[lbn].first_free_page_offset++;

		return table.entry[lbn].pbn*PAGES_PER_BLOCK;
	}

	else if (pbn!=-1&&table.entry[lbn].first_free_page_offset < PAGES_PER_BLOCK) {

		spare[pbn*PAGES_PER_BLOCK + table.entry[lbn].first_free_page_offset].lsn = lsn;
		table.entry[lbn].first_free_page_offset++;
		write++;
		return pbn * PAGES_PER_BLOCK + table.entry[lbn].first_free_page_offset;
		
	}
	else if (table.entry[lbn].first_free_page_offset>= PAGES_PER_BLOCK) {
			//copy & erase
		int offset = 0;
		int a[PAGES_PER_BLOCK] = { 0 };
		for (int i = PAGES_PER_BLOCK - 1; i >= 0; i--) {
			//
			read++;
			if (spare[pbn*PAGES_PER_BLOCK+i].lsn != lsn && repeat(&a,PAGES_PER_BLOCK, spare[pbn*PAGES_PER_BLOCK + i].lsn)==0) {
				/*				*/
				a[offset] = spare[pbn*PAGES_PER_BLOCK + i].lsn;
				spare[freeblk*PAGES_PER_BLOCK + offset].lsn = spare[pbn*PAGES_PER_BLOCK + i].lsn;
				offset++;
				write++;

			}
			spare[table.entry[lbn].pbn*PAGES_PER_BLOCK + i].lsn = -2;	//reset freeblk
			
		}
		
		

		spare[freeblk*PAGES_PER_BLOCK + offset].lsn = lsn;		
		write++;
		offset++;
		table.entry[lbn].first_free_page_offset = offset;

		
		int blk_to_erase = table.entry[lbn].pbn;
		table.entry[lbn].pbn = freeblk;
		freeblk = blk_to_erase;
		table.entry[lbn].first_free_page_offset = offset;
		for (offset; offset < PAGES_PER_BLOCK; offset++) {
			spare[freeblk*PAGES_PER_BLOCK + offset].lsn = -1;

		}
		erase++;

		write++;
		}
	
}

void print_hybrid() {
	for (lbn = 0; lbn < DATABLKS_PER_DEVICE; lbn++) {
		printf("lbn:%d	pbn:%d	offset:%d\n", lbn, table.entry[lbn].pbn,table.entry[lbn].first_free_page_offset);
	}

	for (lbn = 0; lbn < BLOCKS_PER_DEVICE * PAGES_PER_BLOCK; lbn++) {

		printf("spare%d:	%d	\n", lbn, spare[lbn].lsn);


	}
}

int repeat(int *a, int len, int lsn) {
	int tag = 0;
	for (int i = 0; i < len; i++) {
//		printf("%d\n", i);
		if (a[i] == lsn) {
			tag = 1;
			break;
		}
	}
	if (tag == 1) {
		return 1;
	}
	else return 0;

}