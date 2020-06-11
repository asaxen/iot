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

## Set up WIFI (prbly do this with systemd.network instead)
```sh
/etc/netctl# install -m640 examples/wireless-wpa wireless-home
/etc/netctl# cat wireless-home

nano wireless-home

# start wifi
netctl start wireless-home

# Enable wifi at start up
netctl enable wireless-home
```

## Temporarly allow root ssh access
```sh
nano /etc/ssh/sshd_config
# Authentication:
#LoginGraceTime 2m
PermitRootLogin yes
#StrictModes yes
#MaxAuthTries 6
#MaxSessions 10

# reboot
```

## Change root user password, add new user and groups, install sudo
```sh
# Change root password
passwd root
# Add new user
useradd -m asaxen
passwd asaxen
# Check what groups exist
getent group
# Install sudo
pacman -Sy sudo
# Create group sudo
groupadd sudo
# Add user to sudo group
usermod -aG sudo asaxen
# Create group docker
groupadd docker
usermod -aG docker asaxen
# Uncomment line in sudoers file allowing users that are a member in sudo group to execute "sudo"
 nano /etc/sudoers
```

