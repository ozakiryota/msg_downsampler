#!/bin/bash

xhost +

image="msg_downsampler"
tag="latest"
exec_pwd=$(cd $(dirname $0); pwd)
home_dir="/home/user"

docker run \
	-it \
	--rm \
	-e local_uid=$(id -u $USER) \
	-e local_gid=$(id -g $USER) \
	--net=host \
	--privileged \
	-e "DISPLAY" \
	-v "/tmp/.X11-unix:/tmp/.X11-unix" \
	-v $HOME/rosbag:$home_dir/rosbag:ro \
	$image:$tag