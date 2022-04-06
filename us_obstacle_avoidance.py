#!/usr/bin/env python3


import rospy
import time
import math
import numpy as np
from std_msgs.msg import Float32
from geometry_msgs.msg import Twist

class Donnie_Bot:
	def __init__(self):
		rospy.init_node('us_obstacle_avoidance_node')
		rospy.on_shutdown(self.shutdownhook)
		self.ultrasonic_sub=rospy.Subscriber('/sonar', Float32, self.get_sonar)
		self.pub=rospy.Publisher('/cmd_vel',Twist, queue_size=10)
		self.sonar = Float32()
		self.ctrl_c = False
		print("obstacle avoidance using ultrasonic sensor is starting")
		self.rate = rospy.Rate(10)

	def get_sonar(self, msg):
		self.sonar=msg

	def obstacleAvoidance(self):
		self.var=Twist()
		try:
			while not self.ctrl_c:
				if self.sonar.data <= 20:
					self.var.angular.z=1
				else:
					self.var.angular.z=0
				self.var.linear.x=0.7
				self.pub.publish(self.var)
				self.rate.sleep()
		except rospy.ROSInterruptException:
			pass

		rospy.spin()

	def shutdownhook(self):
		print("ending...")
		self.ctrl_c = True
		self.var.angular.z = 0
		self.var.linear.x = 0
		self.pub.publish(self.var)
		self.rate.sleep()

if __name__ == '__main__':
	x = Donnie_Bot()
	try:
		x.obstacleAvoidance()
	except rospy.ROSInterruptException:
		pass

