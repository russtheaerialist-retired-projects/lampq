import Queue

from lampq.cmd import Command


class YunModes(object):
    OFF = "off"
    WHITE = "white"


class Yun(object):
    def __init__(self, reader, registry):
        self._mode_queue = Queue.Queue()
        self._current_mode = "off"
        self._reader = reader
        self._mode_list = set()
        self._restricted_modes = set()
        self._configured = False
        COMMANDS = (
            Command(
                "queue", self.cmd_queue, has_parameters=False, secure=False
            ),
            Command(
                "setmode", self.cmd_setmode, has_parameters=True, secure=False
            ),
            Command(
                "popmode", self.cmd_popmode, has_parameters=False, secure=True
            ),
            Command(
                "hello", self.cmd_hello, has_parameters=True, secure=True
            ),
            Command(
                "modes", self.cmd_modes, has_parameters=False, secure=False
            )
        )
        map(registry.register, COMMANDS)

    def _get_available_modes_for(self, file):
        if Command.is_secure(file):
            available_modes = self._mode_list.union(self._restricted_modes)
        else:
            available_modes = self._mode_list

        return available_modes

    def cmd_setmode(self, file, _, mode):
        if not self._configured:
            file.write("ERROR:Lamp did not send preamble\n")
            return False

        available_modes = self._get_available_modes_for(file)

        if mode in available_modes:
            self._mode_queue.put(mode)
            file.write("OK\n")
        else:
            file.write("ERROR:Unknown Mode\n")

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

    def cmd_hello(self, file, cmd, args):
        """ The hello command is issued by the YUN at first start up to provide information about
            the available modes, which are formatted as pipe separated values in the args.

            If the mode starts with a -, it means this mode is not provided in the list of modes to the web client
        """
        for mode in args.split("|"):
            if mode.startswith('-'):
                self._restricted_modes.add(mode[1:])
            else:
                self._mode_list.add(mode)
        file.write("OK\n")
        self._configured = True
        return False

    def cmd_modes(self, file, cmd, args):
        if not self._configured:
            file.write("ERROR:Lamp did not send preamble\n")
            return False

        available_modes = self._get_available_modes_for(file)

        file.write("OK:{0}\n".format("|".join(available_modes)))