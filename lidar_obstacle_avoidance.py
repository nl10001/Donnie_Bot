#! /usr/bin/env python3

import rospy 
import time 
import math
import numpy as np
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Twist

class DonnieBot(object):
	#initialising the nodes, publishers and subscribers for the motor values and the LiDAR data
	def __init__(self):
		rospy.init_node('LiDAR_obstacle_avoidance')
		rospy.on_shutdown(self.shutdownhook)#runs shutdownhook code when the scripts is ended
		self.ultrasonic_sub=rospy.Subscriber('/scan', LaserScan, self.get_lasers)
		self.pub=rospy.Publisher('/cmd_vel',Twist, queue_size=10)
		self.lasers = LaserScan()
		self.ctrl_c = False
		print("LiDAR obstacle avoidance starting now...")
		self.rate = rospy.Rate(10)
	#subscribing to the LiDAR data
	def get_lasers(self,msg):
		self.lasers=msg

	def obstacleAvoidance(self):
		time.sleep(1)
		self.var=Twist()
		#main loop of the code
		while not rospy.is_shutdown():
			range=self.lasers.ranges
			#getting the data from 3 elements of the LiDAR array
			left=range[1000]
			middle=range[0]
			right=range[134]
			#calculates the movement depending on the data recieved from hte LiDAR
			if left < right and left <middle:
				self.var.angular.z = -0.5
			elif right<left and right<middle:
				self.var.angular.z = 0.5
			elif middle <0.2:
				self.var.angular.z = 0.5
			else:
				self.var.angular.z =0
				self.var.linear.x=0.4
			self.pub.publish(self.var)
			self.rate.sleep()
			
	#stops the motors when the scrips is ended
	def shutdownhook(self):
		print("ending...")
		self.ctrl_c = True
		self.var.angular.z = 0
		self.var.linear.x = 0
		self.pub.publish(self.var)
		self.rate.sleep()

if __name__ == '__main__':
	x = DonnieBot()
	try:
		x.obstacleAvoidance()
	except rospy.ROSInterruptException:
		pass
