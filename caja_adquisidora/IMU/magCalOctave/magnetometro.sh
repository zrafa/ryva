
ssh rafa@10.0.50.94 "cd /home/rafa/ryva/caja_adquisidora/IMU && sudo stdbuf -o0 python magnetometro.py" | stdbuf -o0 grep "\[" | stdbuf -o0 sed -e "s/, /./g" -e "s/\[/MAG./" -e "s/\]//"
