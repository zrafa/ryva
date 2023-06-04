v4l2-ctl -c focus_auto=0
v4l2-ctl -c focus_absolute=0

if [[ $# -ne 1 ]] ; then
	echo "falta un argumento DIR"
	exit 1
fi

DIR=/home/rafa/fotos/$1
mkdir $DIR

./cliente_v4l2 --640 --save ${DIR}   --server 10.0.50.100 --port 6000 
