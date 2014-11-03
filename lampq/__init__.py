from __future__ import print_function

import sys
import time
import logging

from lampq.yun import Yun
from lampq.line_reader import LineReader
from lampq.server import ListeningServer

logger = logging.getLogger(__name__)

def main(args=sys.argv[1:]):
    def exit_on_close(reader, connection):
        logger.info("Received EOF from Console, Shutting Down")
        return True

    reader = LineReader({
        sys.stdin: (lambda x,y: print(x,y), exit_on_close),
    })
    server = ListeningServer(reader, 9090)

    reader.start()

    try:
        while reader.process():
            time.sleep(1)
    except KeyboardInterrupt:
        reader.stop()

if __name__ == "__main__":
    main()