LampQ Protocol
==============

The LampQ protocol is a line-oriented, text-based (for now…) protocol for communication between LampY, LampQ, and
the external-facing webpage.

All commands to lampq are lowercase, and all responses from lampq are uppercase

A single packet consists of a single line of text, delimited with a single newline character `\\n`.

The packet is defined as: `command:arg1|arg2|arg3|…`

The response back from the server is defined as: `RESPONSE:Text string suitable for displaying to end-user`

Responses
---------

There are three available responses: `OK`, `ERROR`, `SETMODE`, and `HELLO`.

`OK` and `ERROR` are responses to requests that you make.  Every command (except `hello`) will return
either `OK` or `ERROR`.

`HELLO:<version>` - the preample message from LampQ to LampY only.  No preamble is given to tcp connections. Version is
a version number to allow LampY to know if it's compatible.  Version is currently 1.0.0.

`SETMODE:<mode>` - this is **broadcasted** to all open connections when the LampY requests as popmode.

Commands
--------

`hello:<list|modes>` - this command is sent from LampY to LampQ in response to the `HELLO` preample.  LampY must provide
a pipe separated list of modes available on LampY.  This allows us to configure the available modes each time we start.

If a mode begins with a '-', it is considered a private mode.  Private modes are not available to the web interface,
but is available to LampY, and is used to represent modes that are available via the dial, but not the web interface.

`modes` - this command sends a list of the modes that are available.  If it's a tcp connection, it only lists the modes
available to the web interface. If it's a console connection, it lists all the modes.

`setmode:<modename>` - this command queues up a mode change.  Because I want the lamp to not go spastic with requests,
LampY will only change modes every X seconds, so setmode will queue up a mode change.

`queue` - this lists the current queue of modes.  This is mainly used to show the queue on the web interface.

`popmode` - *LampY Connection only* this command will get the next mode in the queue.  This command is special because
it **broadcasts** the `SETMODE` response to all connections so that all connections know that the lamp mode has changed.
If there are no messages in the queue, the `SETMODE` response will be the same mode as was previously running.

`echo:<anything>` - this command simply returns an `OK` response with a copy of the arguments that were sent.
