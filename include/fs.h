#ifndef FS_H
#define FS_H

/* Modes for file creation*/
#define O_CREAT 11

/* Flags of file*/
#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR 2

#define FILENAMELEN 16
#define INODEBLOCKS 12
#define INODEDIRECTBLOCKS (INODEBLOCKS - 2)
#define DIRECTORY_SIZE 16

#define MDEV_BLOCK_SIZE 512
#define MDEV_NUM_BLOCKS 512
#define DEFAULT_NUM_INODES (MDEV_NUM_BLOCKS / 4)

#define INODE_TYPE_FILE 1
#define INODE_TYPE_DIR 2

/* Structure of inode*/
struct inode
{
    int id;
    short int type;
    short int nlink;
    int device;
    int size;
    int blocks[INODEBLOCKS];
};

/*File states to check if file is open or closed*/
#define FSTATE_CLOSED 0
#define FSTATE_OPEN 1

/*Struct to store file details like state, fileptr*/
struct filetable
{
    int state;
    int fileptr;
    struct dirent *de;
    struct inode in;
    int flag; /* contains the permission */
};

/*Struct to store directory entry*/
struct dirent
{
    int inode_num;
    char name[FILENAMELEN];
};

/*Struct to store directory details*/
struct directory
{
    int numentries;
    struct dirent entry[DIRECTORY_SIZE];
};

/*Struct to file system details*/
struct fsystem
{
    int nblocks;
    int blocksz;
    int ninodes;
    int inodes_used;
    int freemaskbytes;
    char *freemask;
    struct directory root_dir;
};

/* file and directory functions */
int fs_open(char *filename, int flags);              // open file - read/write/read and write; return fd
int fs_close(int fd);                                // close file pointed by fd
int fs_create(char *filename, int mode);             // create file in given mode
int fs_seek(int fd, int offset);                     // change file pointer position within given file
int fs_read(int fd, void *buf, int nbytes);          // read file contents
int fs_write(int fd, void *buf, int nbytes);         // write to file
int fs_link(char *src_filename, char *dst_filename); // hardlink creation
int fs_unlink(char *filename);                       // remove given hard link

/* filesystem functions */
int fs_mkfs(int dev, int num_inodes);
int fs_mount(int dev);

/* filesystem internal functions */
int fs_get_inode_by_num(int dev, int inode_number, struct inode *in);
int fs_put_inode_by_num(int dev, int inode_number, struct inode *in);
int fs_setmaskbit(int b);
int fs_clearmaskbit(int b);
int fs_getmaskbit(int b);

/*
  Block Store functions
  bread, bwrite,
  bput, bget write entire blocks (macro with offset=0, len=blocksize)
 */
int bs_mkdev(int dev, int blocksize, int numblocks);
int bs_bread(int bsdev, int block, int offset, void *buf, int len);
int bs_bwrite(int bsdev, int block, int offset, void *buf, int len);

/* debugging functions */
void fs_printfreemask(void);
void fs_print_fsd(void);

#endif /* FS_H */
