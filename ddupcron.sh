#!/bin/sh

# Define the host to be updated as 1st arguement to script
if [ -z $1 ]; then
	echo "Usage: ddupcron.sh hostname"
	exit
else
	HOST=$1
fi
# Define interface to grep address from
IFACE="eth0"
IFCHECK=$(/sbin/ifconfig $IFACE|grep ask|awk '{print $2}'|cut -d ':' -f2)
# Define where we should store last IP
IPFILE="/tmp/ddupip"
IPCHECK=$(cat $IPFILE)
# Define path to ddup and ddup arguments (except --host)
DDUP_PATH="/home/ddup/ddup"
DDUP_ARGS="--debug"


if [ "$IFCHECK" = "$IPCHECK" ]; then
	echo "looks like we are still the same ip"
else
	$DDUP_PATH --host $HOST $DDUP_ARGS
	echo "$IFCHECK" > $IPFILE
fi

