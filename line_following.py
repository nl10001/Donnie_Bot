#!/usr/bin/env python3

import numpy as np
import rospy
from std_msgs.msg import Float32MultiArray
from geometry_msgs.msg import Twist
class Line_Following:
	def __init__(self):
		#initialising the nodes, publishers and subscribers for the motor values and the LiDAR data
		rospy.init_node('ir_lesson_node')
		rospy.on_shutdown(self.shutdownhook)#runs shutdownhook code when the scripts is ended
		self.pub = rospy.Publisher('cmd_vel', Twist, queue_size=10)
		self.ir_sub=rospy.Subscriber('/ir',Float32MultiArray,self.get_ir_values)
		self.motorTwist=Twist()
		self.irArray=Float32MultiArray()
		self.ctrl_c = False
		print("starting line following...")
		self.rate = rospy.Rate(20) # 20hz
        
	#subscribing to the IR topic to get the IR data
	def get_ir_values(self,data):
		self.irArray=data


	def update_dir(self):	
		#setting the threshold of the ir data, if the ir is sensing the tape it
		#should be above 300
		self.threshold=300
		thresholdArray=np.zeros(3)
		self.motorTwist.angular.z=0
		self.motorTwist.linear.x=0.7
		while not rospy.is_shutdown():
			#setting the elements in the new array to either 1 or 0 depending if
			#that IR sensor is sensing the black tape
			for i in range(len(self.irArray.data)):
				if self.irArray.data[i]>=self.threshold:
					thresholdArray[i]=1
				else:
					thresholdArray[i]=0
			#comparing elements in the thresholdarray then setting the angular velocity
			#all off line do inverse last action
			if thresholdArray[0]==0 and thresholdArray[1]==0 and thresholdArray[2]==0:
				self.motorTwist.angular.z=self.motorTwist.angular.z
			#left on line turn hard left
			elif thresholdArray[0]==1 and thresholdArray[1]==0 and thresholdArray[2]==0:
				self.motorTwist.angular.z=0.3
			#left and middle on line turn left
			elif thresholdArray[0]==1 and thresholdArray[1]==1 and thresholdArray[2]==0:
				self.motorTwist.angular.z=0.15
			#right on line turn hard right
			elif thresholdArray[0]==0 and thresholdArray[1]==0 and thresholdArray[2]==1:
				self.motorTwist.angular.z=-0.3
			#right and middle on line turn right
			elif thresholdArray[0]==0 and thresholdArray[1]==1 and thresholdArray[2]==1:
				self.motorTwist.angular.z=-0.15
			#middle only on line drive straight
			elif thresholdArray[0]==0 and thresholdArray[1]==1 and thresholdArray[2]==0:
				self.motorTwist.angular.z=0
			#all on line drive straight
			elif thresholdArray[0]==1 and thresholdArray[1]==1 and thresholdArray[2]==1:
				self.motorTwist.angular.z=0
			else:
				self.motorTwist.angular.z=0
			self.pub.publish(self.motorTwist)
			self.rate.sleep()
		
	def shutdownhook(self):
		print("ending...")
		self.ctrl_c = True
		self.motorTwist.angular.z = 0
		self.motorTwist.linear.x = 0
		self.pub.publish(self.motorTwist)
		self.rate.sleep()

if __name__ == '__main__':
	x = Line_Following()
	try:
		x.update_dir()
	except rospy.ROSInterruptException:
		pass
