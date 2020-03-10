# sudont

Does what `sudo` does, but runs the command as your user instead of as root.

## Installing

```
cd sudont
autoreconf
cd build
../configure
make
sudo make install
```

## Uses

There are two primary uses in which `sudont` may actually be helpful.

The first is if you are working on your system and might mess it up, but don't want to be running everything as root:

```
zach@zach-pc:~$ sudo -i
root@zach-pc:~# sudont -i
zach@zach-pc:~$ # do the work on the system
zach@zach-pc:~$ exit
logout
root@zach-pc:~# sudont -i
sudo: unable to stat /etc/sudoers: Permission denied
sudo: no valid sudoers sources found, quitting
sudo: unable to initialize policy plugin
root@zach-pc:~# # oops, I broke something
root@zach-pc:~# # good thing I'm logged in as root now and can fix it :)
```

The second is is you have an unprivileged user and need to switch back to your privileged user for something without destroying your environment.
Note: `sudont` by default only works for an unprivileged user if it is typed directly into a shell that was created with `sudo -i`.

```
zach@zach-pc:~$ sudo -i -u unprivileged-user
unprivileged-user@zach-pc:~$ SOMEVAR=somethingICanNeverRemember
unprivileged-user@zach-pc:~$ # on no, I needed to do something else as 'zach' before I got here,
unprivileged-user@zach-pc:~$ # but I don't want to lose my environment...
unprivileged-user@zach-pc:~$ sudont -i
zach@zach-pc:~$ # Do whatever I need to fix
zach@zach-pc:~$ exit
logout
unprivileged-user@zach-pc:~$ echo $SOMEVAR
somethingICanNeverRemember
unprivileged-user@zach-pc:~$ # yay, I don't have to set up my environment again :)
```
