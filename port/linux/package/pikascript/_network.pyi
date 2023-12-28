STA_IF: int
AP_IF: int
STAT_IDLE: int
STAT_CONNECTING: int
STAT_WRONG_PASSWORD: int
STAT_NO_AP_FOUND: int
STAT_CONNECT_FAIL: int
STAT_GOT_IP: int

class NET:
    def __init__(self, interface_id: int): ...

    def active(self, is_active: int): ...

    def checkActive(self) -> int: ...

    def status(self) -> int: ...

    def statusWithParam(self, param: str) -> int: ...

    def ifconfig(self, ip: str, mask: str, gateway: str, dns: str): ...

    def checkIfconfig(self) -> list: ...

    def close(self): ...


class WLAN(NET):
    def __init__(self, interface_id: int): ...

    def connect(self, ssid: str, key: str): ...

    def isconnected(self) -> int: ...

    def config(self, **kwargs): ...

    def checkConfig(self, param: str) -> any: ...

    def connectWithBssid(self, ssid: str, key: str, bssid: str): ...

    def scan(self) -> list: ...


class LAN(NET):
    def __init__(self, interface_id: int): ...

    def isconnected(self) -> int: ...


def __init__(): ...
