import rospy
import sensor_msgs.point_cloud2 as pc2

if __name__ == "__main__":

	f = open("oneshot.txt","r")

	points_list = []

	for data in pc2.read_points(f.read(), skip_nans=True):
    		points_list.append([data[0], data[1], data[2], data[3]])

	print(points_list)
