LAMP
=====

I'm building an internet-connected lamp with some [ws2801 leds on a string](https://www.adafruit.com/products/738)
and an Arduino Yún.

Eventually there will be a [website](http://lamp.cordandruss.com) that allows you to queue up modes of interaction
to the lamp.  The first version is just mode changes.

Hardware
========

The hardware setup is simple, just follow the instructions at the adafruit page above, and you'll be good to go.
I'll have a schematic at some point, but right now I'm waiting for the Yún to come in, so that will need to wait.

The LEDs are held in place with a custom printed adapter that fit onto the end of
[brass tubes of different lengths](https://www.amazon.com/gp/css/order-history/?ie=UTF8&camp=1789&creative=390957&linkCode=ur2&ref=TE_on&search=115-6442997-6629809&tag=loggerblogger-20&linkId=B76FXZP637BABTSB).
These are then stuck into a piece of hemispherical floral foam.  The Yún sits in a carrier board on top of that,
and a rotary encoder and button are wired out to the base of the light.

It's all strung up and hanging from a boom arm on a tripod, suspended from 550 paracord and some 3d printed pulleys.

Software
========

There are two pieces to this system: LampQ, and LampY.

LampQ
-----

LampQ is a python program that sits on a publicly facing server that has ssh enabled on it.  It handles taking
requests from the outside world (via a webpage), and makes those requests available to LampY.  It handles keeping
track of the queue of requests and providing them to LampY when needed.

For information on the communication protocol, please see the README in the lampq directory

LampY
-------
LampY handles animating the LEDs, and communicating with LampQ over ssh.  It uses the Yún's bridge library to
start an async process call to ssh into the server that is running LampQ.  LampQ only runs when LampY has the
connection open.  In my setup, I just have an account that has LampQ set as the shell for the account, so it's
the only thing you can run.

LampY can be modified by the end user to add whatever modes they would like to have in their lamp, but I have a
number of useful and fun modes available.

For information on the modes, please see the README in the lampy directory