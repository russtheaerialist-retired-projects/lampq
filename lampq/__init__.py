from __future__ import print_function

import sys
import time
import logging

from lampq.yun import Yun
from lampq.line_reader import LineReader
from lampq.server import ListeningServer
from lampq.cmd import Registry, CommandError, Command

logger = logging.getLogger(__name__)

class ConsoleWrapper(object):
    def __getattr__(self, item):
        return getattr(sys.stdin, item)

    @property
    def is_secure(self):
        return True

    def write(self, *args, **kwargs):
        return sys.stdout.write(*args, **kwargs)

def main(args=sys.argv[1:]):

    logging.basicConfig(level=logging.DEBUG)

    def exit_on_close(reader, connection):
        logger.info("Received EOF from Console, Shutting Down")
        return True

    def run_command(_, file, cmd):
        try:
            Registry.run_command_script(file, cmd)
        except CommandError, ex:
            logger.error("{0}: {1}".format(ex.message, cmd))

            return False

        except:
            logger.error("Unknown Error", exc_info=True)

    ECHO = Command("echo", lambda file, cmd, args: file.write("{0} = {1}".format(cmd, args)),
                   has_parameters=True, secure=False)

    def quit_command(file, cmd, *args, **kwargs):
        return True

    def echo(file, cmd, args):
        return "{0} = {1}".format(cmd, args)



    reader = LineReader({
        ConsoleWrapper(): (run_command, exit_on_close),
    })
    yun = Yun(reader, Registry)

    Registry.register(ECHO)

    server = ListeningServer(reader, 9090)

    reader.start()
    sys.stdout.write("HELLO:1.0.0\n")

    try:
        while reader.process():
            time.sleep(0.5)
    except KeyboardInterrupt:
        reader.stop()
    except:
        reader.stop()

    server.shutdown()

if __name__ == "__main__":
    main()