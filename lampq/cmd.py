class CommandError(Exception): pass

class UnknownCommandError(CommandError):
    def __init__(self, cmd):
        super(UnknownCommandError, self).__init__("Unknown Command {0}".format(cmd))

class ArgumentRequiredError(CommandError):
    def __init__(self, cmd):
        super(ArgumentRequiredError, self).__init__("Command {0} Requires Arguments".format(cmd))

class Command(object):
    def __init__(self, name, callback, has_parameters, secure):
        self.name = name
        self.callback = callback
        self.has_parameters = has_parameters
        self.secure = secure

    def __repr__(self):
        return "CMD:{0}".format(self.name)


class _Registry(object):
    def __init__(self):
        self._cmds = {}
        self._UNKNOWN = Command(
            "UNKNOWN",
            self._raise_unknown_command,
            False,
            False
        )

    def register(self, cmd):
        self._cmds[cmd.name] = cmd

    def _raise_unknown_command(self, cmd, *args, **kwargs):
        raise UnknownCommandError(cmd)

    def run_command_script(self, file, line):
        if ":" in line:
            cmd, arguments = line.split(":", 1)
        else:
            cmd = line
            arguments = ""

        cmdobj = self._cmds.get(cmd, self._UNKNOWN)
        if cmdobj.has_parameters and not arguments:
            print(cmdobj, cmdobj.has_parameters, cmdobj.secure)
            raise ArgumentRequiredError(cmd)

        if cmdobj.secure and not (hasattr(file, "is_secure") and file.is_secure):
            # Secure commands are only available to secure connections
            raise UnknownCommandError(cmd)

        return cmdobj.callback(file, cmd, arguments)

Registry = _Registry()