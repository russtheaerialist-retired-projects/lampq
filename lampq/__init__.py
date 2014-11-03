from __future__ import print_function

import sys
import time
import logging

from lampq.yun import Yun
from lampq.line_reader import LineReader
from lampq.server import ListeningServer
from lampq.cmd import Registry, UnknownCommandError

logger = logging.getLogger(__name__)

def main(args=sys.argv[1:]):

    logging.basicConfig(level=logging.DEBUG)

    def exit_on_close(reader, connection):
        logger.info("Received EOF from Console, Shutting Down")
        return True

    def run_command(_, cmd):
        try:
            Registry.run_command_script(cmd)
        except UnknownCommandError:
            return False
        except:
            logger.error("Unknown Error", exc_info=True)

    def quit_command(cmd, *args, **kwargs):
        return True

    Registry.register("quit", quit_command)
    Registry.register("echo", lambda cmd,args: args)

    reader = LineReader({
        sys.stdin: (run_command, exit_on_close),
    })

    server = ListeningServer(reader, 9090)

    reader.start()

    try:
        while reader.process():
            time.sleep(1)
    except KeyboardInterrupt:
        reader.stop()

    server.shutdown()

if __name__ == "__main__":
    main()