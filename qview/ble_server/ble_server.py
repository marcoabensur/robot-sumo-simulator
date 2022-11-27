from gi.repository import GLib

# Bluezero modules
from bluezero import adapter
from bluezero import peripheral
from bluezero import device
import socket
import threading

# constants
BLE_SERVICE = '0000FFE0-0000-1000-8000-00805F9B34FB'
BLE_CHARACTERISTIC = '0000FFE1-0000-1000-8000-00805F9B34FB'
BLE_RECEIVE_SIZE = 20

class UARTDevice:
    tx_obj = None

    @classmethod
    def on_connect(cls, ble_device: device.Device):
        print("Connected to " + str(ble_device.address))

    @classmethod
    def on_disconnect(cls, adapter_address, device_address):
        print("Disconnected from " + device_address)

    @classmethod
    def uart_notify(cls, notifying, characteristic):
        if notifying:
            cls.tx_obj = characteristic
        else:
            cls.tx_obj = None

    @classmethod
    def update_tx(cls, value):
        if cls.tx_obj:
            print("Sending")
            cls.tx_obj.set_value(value)

    @classmethod
    def uart_write(cls, value, options):
        print('raw bytes:', value)
        print('With options:', options)
        # print('Text value:', bytes(value).decode('utf-8'))
        cls.update_tx(value)

        # Send ble data to tcp server
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_address = ('localhost', 10000)
        print ('connecting to port' + str(server_address))
        sock.connect(server_address)
        try:
            message = bytearray(value)
            hex_string = "".join("0x%02x, " % b for b in message)
            print ('BLE sending ' + hex_string)
            sock.sendall(message)

        finally:
            print('closing socket')
            sock.close()


def server_thread():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    server_address = ('localhost', 10001)
    print ('starting up on %s port %s' % server_address)

    sock.bind(server_address)
    sock.listen(1)

    while True:
        # Wait for a connection
        print ('waiting for a connection')
        connection, client_address = sock.accept()
        try:
            print ('connection from + ' + str(client_address))
            data = connection.recv(BLE_RECEIVE_SIZE)
            ble_data_from_server = bytearray(data)
            hex_string = "".join("0x%02x, " % b for b in ble_data_from_server)
            print ('received ' + hex_string)
            UARTDevice.update_tx(ble_data_from_server)
            
        finally:
            # Clean up the connection
            connection.close()

def main(adapter_address):
    thread2 = threading.Thread(target=server_thread)
    thread2.daemon = True
    thread2.start()


    ble_uart = peripheral.Peripheral(adapter_address, local_name='JDY-10-V2.5')
    ble_uart.add_service(srv_id=1, uuid=BLE_SERVICE, primary=True)
    
    ble_uart.add_characteristic(srv_id=1, chr_id=1, uuid=BLE_CHARACTERISTIC,
                                value=[], notifying=False,
                                flags=['write-without-response', 'notify'],
                                notify_callback=UARTDevice.uart_notify,
                                read_callback=None,
                                write_callback=UARTDevice.uart_write)

    ble_uart.on_connect = UARTDevice.on_connect
    ble_uart.on_disconnect = UARTDevice.on_disconnect

    ble_uart.publish()


if __name__ == '__main__':
    main(list(adapter.Adapter.available())[0].address)