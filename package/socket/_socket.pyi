from PikaObj import *


class socket:
    def _gethostname() -> str: ...
    def _bind(host: str, port: int): ...
    def _listen(num: int): ...
    def _accept(): ...
    def _send(data: any): ...
    def _close(): ...
    def _connect(host: str, port: int): ...
    def _recv(num: int) -> str: ...
    def _init(): ...
