1. make
2. sudo insmod chardev.ko
3. sudo dmseg
4. cat /home/ucsp/dev
5. **FUNDAMENTAL ->sudo chmod -R 777 /dev/chardev** 
6. echo "HOLA UCSP" > /dev/chardev
7. cat dev/chardev
8. sudo rmmod chardev
