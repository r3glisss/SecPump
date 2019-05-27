# BLE Commmand Line Interface

Before Running the command line BLE interface on a linux machine you need to install a couple of package

__ubuntu:__
~~~bash
sudo apt-get install python-pip libglib2.0-dev
sudo pip install bluepy
~~~

__fedora:__
~~~bash
sudo dnf install python-pip glib2-devel
sudo pip install bluepy
~~~

Then you can launch the interface:

~~~python
sudo python bluCmd.py
~~~
