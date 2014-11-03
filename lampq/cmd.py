class UnknownCommandError(Exception): pass

class _Registry(object):
    def __init__(self):
        self._cmds = {}

    def register(self, cmd, callback):
        self._cmds[cmd] = callback

    def _raise_unknown_command(self, cmd, *args, **kwargs):
        raise UnknownCommandError(cmd)

    def run_command_script(self, line):
        if ":" in line:
            cmd, arguments = line.split(":", 1)
        else:
            cmd = line
            arguments = ""

        callback = self._cmds.get(cmd, self._raise_unknown_command)

        return callback(cmd, arguments)

Registry = _Registry()