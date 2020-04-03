#!/usr/bin/env python

import serial, sys, time
import rospy
# import rosutill
import threading

from geometry_msgs.msg import Twist


class RCcontroller:

    def __init__(self, port):
        ## For reading serial device
        #self._serialData = serial.Serial(port, 9600)
        self.port = port
        self.lock = threading.Lock()
        self._cleanCMD = {}

        ##For executing ros msgs
        self._steeringChannel = "2"
        self._driveChannel = "1"
        thread = threading.Thread(target=self.recieverReadingloop)
        thread.daemon = True
        thread.start()

        ## ros implementation here: self.publisher = rospy.Publiser(topic, message, queue size)
        self.publisher = rospy.Publisher("cmd_vel", Twist, queue_size=10)
        self.vel_msg = Twist()
        

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

    def steering(self):
        _steeringInput = self.serialCommand()

        try:
            _steeringCMD = _steeringInput.get(self._steeringChannel)

            if _steeringCMD < 1450 or _steeringCMD > 1550:
            ## add approproiate scaling factor here
                _steeringOutput = (int(_steeringCMD) - 1500)/500.0
            else:
                _steeringOutput = 0

            return _steeringOutput

        except TypeError:
            pass
    
    def drive(self):
        _driveInput = self.serialCommand()
        _driveOutput = 0

        try:
            _driveCMD = int(_driveInput.get(self._driveChannel))
            #modify _drive Output to have appropriate scaling factor

            #print("DriveCMD: {}".format(_driveCMD))

            if  _driveCMD > 1550 or _driveCMD < 1450:
                _driveOutput = (_driveCMD - 1550) / 510.0

            else:
                _driveOutput = 0

            #print("Drive Output: {}".format(_driveOutput))
            return _driveOutput

        except TypeError:
            pass

    def execution(self):
        self.vel_msg.linear.x = self.cmdOutputs()[1]
        self.vel_msg.angular.z = self.cmdOutputs()[0]
        self.publisher.publish(self.vel_msg)

       # print("Steering: {}".format(output[0]))
       # print("Drive: {}".format(output[1]))


    def cmdOutputs(self):
        try:
            ## scaling factor implementation
            _steering = (self.steering())*2    ##* scalling factor *
            _drive = self.drive()   ##* scaling factor *

        except TypeError:
            _steering = 0
            _drive = 0
            pass

	print("Steering: {}".format(_steering))
	print("Drive: {}".format(_drive))


        if (_steering is not None and _drive is not None):
            return [_steering, _drive]

        else:
            return[0, 0]

         

    
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
