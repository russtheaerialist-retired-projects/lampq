import socket
import logging

from lampq.line_reader import LineIncomplete

logger = logging.getLogger(__name__)

class ReadlineSocketWrapper(object):
    def __init__(self, sock):
        self._sock = sock
        self._buffer = []

    def __getattr__(self, item):
        return getattr(self._sock, item)

    def readline(self):
        try:
            data = self.recv(1024)
        except socket.IOError:
            logging.info("Received socket Error", exc_info=True)
            raise LineIncomplete()

        self._buffer.append(data)
        data = "".join(self._buffer)

        newline = data.find('\n')
        if newline <= 0:
            # No New Line exists, incomplete line
            self._buffer = [ data ]
            raise LineIncomplete()

        line = data[:newline-1]
        self._buffer = [ data[newline+1:] ]

        return line

class ListeningServer(object):
    def __init__(self, reader, port):
        self._socket = socket.socket(socket.AF_INET)
        self._socket.bind(('', port))  # Find to all addresses
        self._socket.listen(5)
        self._connections = []
        reader.add(self._socket, self._accept_new_connection, self._remove_connection)

    def _accept_new_connection(self, reader, _):
        new_socket, address = self._socket.accept()
        new_socket.setblocking(0)  # Non-blocking
        logger.info("New Connection from: {0}".format(address))

        wrapper = ReadlineSocketWrapper(new_socket)

        self._connections.append(wrapper)
        reader.add(wrapper, self._process_command, self._remove_connection)

    def _process_command(self, reader, line):
        print(line)

    def _remove_connection(self, reader, f):
        f.close()