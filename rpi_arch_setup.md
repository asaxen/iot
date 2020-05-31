# Set up arch linux on RPI 4

https://archlinuxarm.org/platforms/armv8/broadcom/raspberry-pi-4

## Default users and password
root - root
alarm - alarm

Login as root:
## Initilize pacman
```sh
pacman-key --init
pacman-key --populate archlinuxarm
```

## Set up WIFI
```sh
/etc/netctl# install -m640 examples/wireless-wpa wireless-home
/etc/netctl# cat wireless-home

nano wireless-home

# start wifi
netctl start wireless-home

# Enable wifi at start up
netctl enable wireless-home
```
