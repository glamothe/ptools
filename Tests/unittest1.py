from ptools import *
import random
import unittest


class TestCoord3D(unittest.TestCase):
	def setUp(self):
		self.coo1 = Coord3D(3.0, 4.0, 5.0)
		self.coo2 = Coord3D(1.0, 2.0, 7.5)
	
	def testPlusOperator(self):
		coo3 = self.coo1 + self.coo2
		self.assertEqual(coo3.x, 4.0)
		self.assertEqual(coo3.y, 6.0)
		self.assertEqual(coo3.z, 12.5)
	def testMinusOperator(self):
		coo3 = self.coo1 - self.coo2
		self.assertEqual(coo3.x, 2.0)
		self.assertEqual(coo3.y, 2.0)
		self.assertEqual(coo3.z, -2.5)
	def testPlusEqualOperator(self):
		coo3 = Coord3D(self.coo1)
		coo3 += self.coo2
		self.assertEqual(coo3.x, 4.0)
		self.assertEqual(coo3.y, 6.0)
		self.assertEqual(coo3.z, 12.5)
		
unittest.main()