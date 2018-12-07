#!/bin/bash

# ENSURE VPN IS ON

sudo mount -t cifs -o user=mbcx4ae3,domain=ds.man.ac.uk,sec=ntlmsspi //nask.man.ac.uk/home$ /mnt/p_drive
sudo rsync -rtvu --delete . /mnt/p_drive/public_html/
# sudo cp -v -r -u . /mnt/p_drive/public_html/
sudo umount /mnt/p_drive
