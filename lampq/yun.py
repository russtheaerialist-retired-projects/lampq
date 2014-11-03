import sys
import select
import Queue

from lampq.cmd import Command

class YunModes(object):
    OFF = "off"
    WHITE = "white"

class Yun(object):
    def __init__(self, reader):
        self._mode_queue = Queue.Queue()
        self._current_mode = "off"
        self._reader = reader
        self.CMD_QUEUE = Command(
            "queue", self.cmd_queue, False, False
        )
        self.CMD_SETMODE = Command(
            "setmode", self.cmd_setmode, True, False
        )
        self.CMD_POPMODE = Command(
            "popmode", self.cmd_popmode, False, True
        )

    def cmd_setmode(self, file, _, mode):
        self._mode_queue.put(mode)
        file.write("OK\n")
        return False

    def cmd_queue(self, file, cmd, _):
        file.write("OK:{0}\n".format("|".join(self._mode_queue.queue)))

        return False

    def cmd_popmode(self, file, cmd, _):
        try:
            self._current_mode = self._mode_queue.get_nowait()
        except Queue.Empty:
            pass
        self._reader.send_all("SETMODE:{0}".format(self._current_mode))

        return False