device="sdb"
sgdisk -n 1:2048:+512M -t 1:ef00 -c 1:"EFI System" $device
sgdisk -n 2:0:0 -t 2:8300 -c 2:"Linux Filesystem" $device

bootuuid=$(blkid /dev/${device}1 -sUUID -ovalue)
rootuuid=$(blkid /dev/${device}2 -sUUID -ovalue)

cat << EOF > test.txt
UUID=$rootuuid  /  ext4  defaults,errors=remount-ro  0  1
UUID=$bootuuid /boot/efi       vfat    umask=0077      0       1
EOF