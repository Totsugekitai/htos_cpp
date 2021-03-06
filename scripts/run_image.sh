#!/bin/bash -eux

if [ $# -lt 1 ]
then
    echo "Usage: $0 <image name>"
    exit 1
fi

DISK_IMG=$1

if [ ! -f $DISK_IMG ]
then
    echo "No such file: $DISK_IMG"
    exit 1
fi

qemu-system-x86_64 \
    -m 1G \
    -drive if=pflash,format=raw,readonly,file=$HTOS_ROOT/third_party/OVMF_CODE.fd \
    -drive if=pflash,format=raw,file=$HTOS_ROOT/third_party/OVMF_VARS.fd \
    -drive if=ide,index=0,media=disk,format=raw,file=$DISK_IMG \
    -device nec-usb-xhci,id=xhci \
    -device usb-kbd \
    -gdb tcp::1234 \
    -chardev stdio,mux=on,id=com1 \
    -serial chardev:com1 \
    -monitor telnet::5555,server,nowait \
