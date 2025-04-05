/**
 * @brief Super block struct
 *
 * magic1,magic2,magic3 are used for consistency check and 
 * need to be placed at regular intervals
 */
#include <stdint.h>
#define int32	int32_t
#define int64	int64_t
#define uint32	uint32_t
#define uint16	uint16_t
#define SUPER_BLOCK_MAGIC1			0x42465331		/* BFS1 */
#define SUPER_BLOCK_MAGIC2			0xdd121031
#define SUPER_BLOCK_MAGIC3			0x15b6830e
#define SUPER_BLOCK_DISK_CLEAN		0x434C454E		/* CLEN */
#define SUPER_BLOCK_DISK_DIRTY		0x44495254		/* DIRT */

struct block_run {
	int32		allocation_group; /**< Allocation group these blocks belong to*/
	uint16		start; /**< Starting block number */
	uint16		length; /**<Length of extent */
} __attribute__((packed)); 

typedef struct block_run block_run;
typedef block_run inode_addr;
struct disk_super_block {
	char		name[32];
	int32		magic1; /**!value "BFS1"*/
	int32		fs_byte_order;
	uint32		block_size;
	uint32		block_shift;	/**<Exponent of block_size. used for additional checking*/
	int64		num_blocks; 	/**<Total usable blocks*/
	int64		used_blocks;
	int32		inode_size; 	/**<Multiple of block_size. used for checking as well*/
	int32		magic2;
	int32		blocks_per_ag;	/**<Number of bitmap blocks. total should not exceed 65536*/
	int32		ag_shift;		/**<Bits to shift allocation number to get byte offset*/
	int32		num_ags; 		/**<Total allocation groups. used as a check for block_run::allocation_group*/
	int32		flags;			/**<BFS_CLEAN or BFS_DIRTY. used to check for consistency*/
	block_run	log_blocks;		/**<Location of journal. circular buffer of fixed size*/
	int64		log_start;
	int64		log_end;
	int32		magic3;
	inode_addr	root_dir;		/**<Location of root directory*/
	inode_addr	indices;		/**<Location of index directory*/
	int32		_reserved[8];
	int32		pad_to_block[87]; /**<This also contains parts of the boot block*/
} __attribute__((packed));

void sb_dump_info(const struct disk_super_block *dsb);
int inode_check(int fd, const struct disk_super_block *dsb);
void fs_check(int fd, const struct disk_super_block *dsb);
int sb_check(const struct disk_super_block *dsb);
