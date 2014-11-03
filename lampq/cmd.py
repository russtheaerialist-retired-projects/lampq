class CommandError(Exception): pass

class UnknownCommandError(CommandError):
    def __init__(self, cmd):
        super(UnknownCommandError, self).__init__("Unknown Command {0}".format(cmd))

class ArgumentRequiredError(CommandError):
    def __init__(self, cmd):
        super(ArgumentRequiredError, self).__init__("Command {0} Requires Arguments".format(cmd))

class _Registry(object):
    def __init__(self):
        self._cmds = {}

    def register(self, cmd, callback, require_arguments=False):
        self._cmds[cmd] = (callback, require_arguments)

    def _raise_unknown_command(self, cmd, *args, **kwargs):
        raise UnknownCommandError(cmd)

    def run_command_script(self, line):
        if ":" in line:
            cmd, arguments = line.split(":", 1)
        else:
            cmd = line
            arguments = ""

        callback, require_arguments = self._cmds.get(cmd, (self._raise_unknown_command, False))
        if require_arguments and not arguments:
            raise ArgumentRequiredError(cmd)

        return callback(cmd, arguments)

Registry = _Registry()