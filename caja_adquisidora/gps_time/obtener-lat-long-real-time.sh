stdbuf -o0 /root/gps_time | stdbuf -o0 egrep  -e "GNRMC" | stdbuf -o0 cut -f 4,6 -d, 
