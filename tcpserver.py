import socket
import csv 
"""with open('C:/Users/danby/Desktop/cgl5_10.csv', 'r') as csvfile:
	reader = csv.reader(csvfile)
	for row in reader:
		print row
csvfile.close()
"""

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('0.0.0.0', 8787))
s.listen(5)
print 'Waiting for connection'
while True:
	sock, addr = s.accept()
	print 'connection to', addr, 'success!'
	#while(1):
	csvfile = open('C:/Users/danby/Desktop/cgl5_10.csv', 'r')
	reader = csv.reader(csvfile)
	count = 0
	data = []
	for row in reader:
		#print type(row)
		#print row
		
		if count <=  2:
			count = count +1
		else:
			#row.append('#')
			data.append(row)
			#data.append('#')
			#sock.send(str(row))
			#sock.send('#')
		
	if(len(data) < 10):
		sock.send(str(data))
	else:
		for i in range(10):
			sock.send(str(data[-i]))
			sock.send('#')
		#sock.send(str(data[-11:]))
	csvfile.close()
		#s.send(str)

s.close()