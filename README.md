# checkfs: a filesystem check tool for OpenBFS

This tool performs sanity checks on a OpenBFS partition and is 
developed for FreeBSD.

## Building

clone this repo in a directory. and run the following command in the terminal.
```bash
cd <work-dir>
make
```

## Running
sudo access is required to open the partition. The partition name may
look something like /dev/ndaXpY if it is in disk or /dev/daXpsY if it is
in a usb flash drive.
```bash
$ sudo ./checkfs <name-of-partition>
```
## TODO
[] fill in the inode_check() function.
[] add journal replay
