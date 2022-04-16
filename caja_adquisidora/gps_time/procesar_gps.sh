egrep -B1  -e "GNRMC" gps.txt | cut -f 4,6 -d, | while read linea ; do 
	echo -n $(echo -n $linea |  cut -f 1 -d\ ); echo -n "," 
	read l 
	echo $l 
	read l 
done
