# checkfs: A filesystem check tool for OpenBFS

This tool performs sanity checks on an OpenBFS partition and is 
developed for FreeBSD. As of now, only the superblock is read to
determine if the filesystem is corrupted.

## Building

Clone this repo in a directory and run the following command in the terminal.
```bash
cd <checkfs-dir>
make
```

## Running
The partition name may look something like /dev/ndaXpY if it is in disk or /dev/daXpsY if it is in a usb flash drive.
```bash
$ sudo ./checkfs <name-of-partition>
```
## TODO
1. [] fill in the inode_check() function.
2. [] add journal replay
