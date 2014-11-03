import sys
import select

class Yun(object):
    def __init__(self):
        self._buffer = []

    def _dispatch(self, line):
        print(">>>{0}".format(line))

    @property
    def _input_waiting(self):
        retval = select.select([sys.stdin,],[],[],0.0)
        return retval[0]

    def _read_input(self):
        if not self._input_waiting:
            raise StopIteration()

        while self._input_waiting:
            data = sys.stdin.read(1)
            self._buffer.append(data)
            print(self._buffer)

        buffer = "".join(self._buffer)
        first_newline = buffer.find('\n')
        while first_newline >= 0:
            line = buffer[:first_newline]

            buffer = buffer[first_newline+1:]
            print("+{0}|{1}|".format(line, buffer))

            yield line
            first_newline = buffer.find('\n')

        self._buffer = [ buffer ]

    def process_input(self):
        for line in self._read_input():
            if self._dispatch(line):
                return True

        return False