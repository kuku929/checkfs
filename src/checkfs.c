#include <unistd.h>
#include <stdio.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include "checkfs.h"

/**
 * @brief Dumps information from the superblock.
 * 
 * This function takes the superblock structure as input and prints its 
 * contents.
 *
 * @param dsb Pointer to the disk superblock structure to be printed.
 */
void sb_dump_info(const struct disk_super_block *dsb)
{
	printf("%-20s %s\n", "name", dsb->name);
	// super block size
	printf("%-20s %lu\n", "superblock size", sizeof(struct disk_super_block));
	// magic1 
	printf("%-20s %d\n", "magic1", dsb->magic1);
	// fs_byte_order;
	printf("%-20s %d\n", "byte order", dsb->fs_byte_order);
	// block_size;
	printf("%-20s %d\n", "block size", dsb->block_size);
	// num_blocks;
	printf("%-20s %ld\n", "number of blocks", dsb->num_blocks);
	// block_shift;
	printf("%-20s %d\n", "block shift", dsb->block_shift);
	// used_blocks;
	printf("%-20s %ld\n", "used blocks", dsb->used_blocks);
	// inode size;
	printf("%-20s %d\n", "inode size", dsb->inode_size);
	// blocks_per_ag;
	printf("%-20s %d\n", "blocks per ag", dsb->blocks_per_ag);
	// ag_shift;
	printf("%-20s %d\n", "allocation shift", dsb->ag_shift);
	// num_ags;
	printf("%-20s %d\n", "number of ag", dsb->num_ags);
}

/**
 * @brief Placeholder function for inode checking.
 * 
 * This function is a placeholder for checking the integrity of inodes.
 * The current implementation always returns success (1).
 *
 * @param fd File descriptor of the device.
 * @param dsb Pointer to the disk superblock structure.
 * 
 * @return Always returns 1.
 */
int inode_check(int fd, const struct disk_super_block *dsb)
{
	return 1;
}

/**
 * @brief Checks the integrity of the file system.
 * 
 * This function performs sanity checks on the superblock. If the superblock 
 * is dirty, it checks if the inodes are also corrupted. If corruption is 
 * detected, an error message is printed.
 *
 * @param fd File descriptor of the device.
 * @param dsb Pointer to the disk superblock structure.
 */
void fs_check(int fd, const struct disk_super_block *dsb)
{
	int err = sb_check(dsb);
	if(err == -1){
		// something is wrong
		printf("Superblock is dirty!\n");
		// let's see if the inodes are dirty
		err = inode_check(fd, dsb);
		if(err == -1){
			printf("Data may be corrupted!\n");
			return;
		}
	}
	printf("Filesystem is clean!\n");
	return;
}

/**
 * @brief Performs a sanity check of the superblock.
 * 
 * This function checks whether the magic numbers, block sizes, and flags 
 * in the superblock are correct and consistent. If any of these checks 
 * fail, it returns -1 indicating that the superblock is not valid.
 *
 * @param dsb Pointer to the disk superblock structure to be checked.
 * 
 * @return Returns 1 if the superblock is valid, otherwise returns -1.
 */
int sb_check(const struct disk_super_block *dsb)
{
	if( dsb->magic1 != SUPER_BLOCK_MAGIC1										||
		dsb->magic2 != SUPER_BLOCK_MAGIC2									||
		dsb->magic3 != SUPER_BLOCK_MAGIC3									||
		(dsb->block_size != (1<<(dsb->block_shift)))								||
		(dsb->blocks_per_ag << (dsb->block_shift + 3)!= (1<<dsb->ag_shift))			||
		(dsb->flags == SUPER_BLOCK_DISK_DIRTY)								||
		(dsb->flags != SUPER_BLOCK_DISK_CLEAN)
	)
	{
		printf("flag: %d\n", dsb->flags);
		return -1;
	}
	return 1;
}


int main(int argc, char **argv)
{
	const char *dev_name;
	if(argc > 1){
		dev_name = argv[1];
	}else{
		printf("specify a device!\n");
		return -1;
	}
	int fd;
	if((fd = open(dev_name, O_RDONLY)) == -1){
		err(errno, "cannot open device %s", dev_name);
	}
	char buffer[1024];
	if(read(fd, buffer, sizeof(buffer)) != sizeof(buffer)){
		err(errno, "superblock read failed! ");
	}
	const struct disk_super_block *dsb = (struct disk_super_block *)(buffer + 512);
	sb_dump_info(dsb);
	fs_check(fd, dsb);
}
