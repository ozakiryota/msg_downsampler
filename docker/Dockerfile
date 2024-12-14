########## Pull ##########
FROM ros:noetic
########## User ##########
ARG home_dir="/home/user"
COPY entrypoint.sh /usr/local/bin/entrypoint.sh
RUN apt-get update && \
	apt-get install -y \
		gosu \
		sudo && \
	chmod +x /usr/local/bin/entrypoint.sh && \
	mkdir -p $home_dir
ENTRYPOINT ["/usr/local/bin/entrypoint.sh"]
########## Non-interactive ##########
ENV DEBIAN_FRONTEND=noninteractive
########## ROS Setup ##########
RUN mkdir -p $home_dir/catkin_ws/src && \
	cd $home_dir/catkin_ws && \
	/bin/bash -c "source /opt/ros/noetic/setup.bash; catkin_make" && \
	echo "source /opt/ros/noetic/setup.bash" >> $home_dir/.bashrc && \
	echo "source ~/catkin_ws/devel/setup.bash" >> $home_dir/.bashrc && \
	echo "export ROS_WORKSPACE=~/catkin_ws" >> $home_dir/.bashrc
## cmk
RUN echo 'function cmk(){(cd $ROS_WORKSPACE; catkin_make $@)}' >> $home_dir/.bashrc
########## Common Tool ##########
RUN apt-get update && \
	apt-get install -y git
########## msg_downsampler ##########
RUN cd $home_dir/catkin_ws/src && \
	git clone https://github.com/ozakiryota/msg_downsampler.git && \
	cd .. && \
	/bin/bash -c "source /opt/ros/noetic/setup.bash; catkin_make"
########## Debug ##########
RUN apt-get update && \
	apt-get install -y \
		ros-noetic-rviz \
		ros-noetic-image-transport-plugins
########## Initial position ##########
WORKDIR $home_dir
CMD ["bash"]