#!/bin/bash
# Print the PID of this script
echo "PID: $$"

# Sleep for 30 seconds
#sleep 30

rm -f testfile.img
dd if=/dev/zero of=testfile.img bs=1M seek=1000 count=1
DEVICE=$(losetup --show -f testfile.img)
mkfs.btrfs -f $DEVICE
mkdir -p tmpmnt

FAILTYPE=fail_function
FAILFUNC=__x64_sys_read
echo $FAILFUNC > /sys/kernel/debug/$FAILTYPE/inject
echo 100 > /sys/kernel/debug/$FAILTYPE/$FAILFUNC/retval
echo Y > /sys/kernel/debug/$FAILTYPE/task-filter
echo 100 > /sys/kernel/debug/$FAILTYPE/probability
echo 0 > /sys/kernel/debug/$FAILTYPE/interval
echo 9999999999999999999 > /sys/kernel/debug/$FAILTYPE/times
echo 0 > /sys/kernel/debug/$FAILTYPE/space
echo 1 > /sys/kernel/debug/$FAILTYPE/verbose

#read -p "Digite o PID do processo que sera impactado pela injecao: " pid
#command="echo 1 | sudo tee /proc/$pid/make-it-fail"
#eval $command

echo "Pressione uma tecla finalizar a injecao de falha."
read

echo > /sys/kernel/debug/$FAILTYPE/inject

rmdir tmpmnt
losetup -d $DEVICE
rm testfile.img
