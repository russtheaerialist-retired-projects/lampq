import threading
import select
import collections
import Queue

class LineIncomplete(Exception): pass

class LineReader(object):
    def __init__(self, input_map):
        self._map = input_map
        self._thread = threading.Thread(target=self._run)
        self._stop_signal = threading.Event()
        self._stop_signal.clear()
        self._queues = collections.defaultdict(Queue.Queue)

    def start(self):
        self._thread.start()

    def stop(self):
        self._stop_signal.set()
        self._thread.join()

    def _run(self):
        while not self._stop_signal.is_set() and len(self._map):
            ready = select.select(self._map,[],[],0.0)[0]
            for f in ready:
                callback, on_remove = self._get_entry(f)
                if not hasattr(f, "readline"):
                    # This is server socket that's accepting a connection, so we won't read a line from it
                    callback(self, f)
                    continue

                try:
                    line = f.readline().rstrip()
                except LineIncomplete:
                    continue

                if line:
                    self._queues[f].put_nowait(line.rstrip())
                else:
                    _, on_remove = self._get_entry(f)
                    del self._map[f]
                    on_remove(self, f)

    def add(self, f, callback, on_remove):
        self._map[f] = (callback, on_remove)

    def _get_entry(self, f):
        return self._map.get(f, (lambda x, y: False, lambda x, y: False))

    def process(self):
        for f, q in self._queues.items():
            try:
                item = q.get_nowait()
                callback, _ = self._get_entry(f)
                should_exit = callback(self, item)
                if should_exit:
                    self.stop()
                    return False
            except Queue.Empty:
                pass

        return self._thread.is_alive()