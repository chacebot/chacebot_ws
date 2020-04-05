#!/usr/bin/env python

import serial, sys, time
import rospy
# import rosutill
import threading

from local_controller_node.msg import RControllerCommands


class RCcontroller:

    def __init__(self, port):
        ## For reading serial device
        #self._serialData = serial.Serial(port, 9600)
        self.port = port
        self.lock = threading.Lock()
        self._cleanCMD = {}

        ##For executing ros msgs
        self._enableChannel = "3"
        self._steeringChannel = "2"
        self._driveChannel = "1"
        thread = threading.Thread(target=self.recieverReadingloop)
        thread.daemon = True
        thread.start()

        self._enableStatus = True

        ## ros implementation here: self.publisher = rospy.Publiser(topic, message, queue size)
        self.publisher = rospy.Publisher("local_cmd", RControllerCommands, queue_size=10)
        

    def serialInterpreter(self):
        _serialData = serial.Serial(self.port, 9600)
        _data = (_serialData.readline().strip())
        _decodedData = _data.decode('utf-8')
        _decodedData.split(" ")
        _splitData = _decodedData.split()
        #print("Split data: {}".format(_splitData[1]))
        try:
            _signal = _splitData[0]
            _value = _splitData[1]
            _filteredData = [_signal, int(_value)]
            return _filteredData

        except (serial.serialutil.SerialException, TypeError, IndexError, ValueError) as e:
            print("Passed at Serial Interpreter because: {}" .format(e))
            pass
        
    
    def read(self):
        
        try:
            _rawCMD = self.serialInterpreter()
            with self.lock:
                self._cleanCMD[_rawCMD[0]] = int(_rawCMD[1])

            #print("Raw CMD0: {}".format(_rawCMD[0]))
        
        except TypeError:
            print("Passed at Read")
            pass

    def serialCommand(self):
        with self.lock:
            return self._cleanCMD

    def recieverReadingloop(self):
        while True:
            self.read()
            time.sleep(0.01)

    def execution(self):

        self.publisher.publish(self.cmdOutputs())

    def cmdOutputs(self):
        if (_steering is not None and _drive is not None):
            local_cmd = RControllerCommands(steering = self.serialCommand()[self._steeringChannel], drive = self.serialCommand()[self._driveChannel], status = self._enableStatus)
            return local_cmd
        else:
            local_cmd = RControllerCommands(steering = 1500, drive = 1500, status = False)
            return local_cmd        

    
def main():

    _devicePort = '/dev/ttyACM0'

    node_name = 'rc_controller'
    rospy.init_node(node_name)
    node = RCcontroller(_devicePort)
    rate = rospy.Rate(10)


    while not rospy.is_shutdown():
        node.execution()
        rate.sleep()



if __name__ == '__main__':
    main()
