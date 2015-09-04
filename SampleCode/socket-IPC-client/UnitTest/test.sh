#/bin/sh

AMOUNT=10000
count=1
echo "Test start!"
CMD="../Debug/socket-IPC-client"
while [ 1 ]; do
	echo "*** count=$count ***"
	"$CMD"
	if [ "$count" = "$AMOUNT" ];  then
		echo "Test end!"
		break;
	fi 
	count=$((count+1))
	sleep 0.5
done

exit 0