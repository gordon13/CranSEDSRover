# !/usr/bin/python3
# import socket
# import sys
import tkinter as tk
from tkinter import *
from tkinter.scrolledtext import ScrolledText
from tkinter import messagebox
from client import gs_client
from threading import Thread
import time
from queue import Queue
import os
import socket
import sys
import subprocess
import common.message_definitions as md


class Application(tk.Frame):
    def __init__(self,master = None):
        super().__init__(master)
        self.width = master.winfo_screenwidth()
        self.height = master.winfo_screenheight()
        self.master.geometry((str(self.width) + 'x' + str(self.height)))
        self.left_arrow_img = tk.PhotoImage(file="left_arrow.png")
        self.right_arrow_img = tk.PhotoImage(file="right_arrow.png")
        self.up_arrow_img = tk.PhotoImage(file="up_arrow.png")
        self.down_arrow_img = tk.PhotoImage(file="down_arrow.png")
        self.stop_button_img = tk.PhotoImage(file="stop_button.png")
        self.rover_speed = 0
        self.master.bind('<Left>', lambda m="left": self.start_command_thread(m))
        self.master.bind('<Right>', lambda m="right": self.start_command_thread(m))
        self.master.bind('<Up>', lambda m="up": self.start_command_thread(m))
        self.master.bind('<Down>', lambda m="down": self.start_command_thread(m))
        self.master.bind('<m>', lambda m="mechanism": self.start_command_thread(m))
        self.master.bind('<b>', lambda m="bucket": self.start_command_thread(m))

        # self.master.bind('<Left>', self.leftKey)
        self.master.focus_set()
        # self.pack()
        # self.create_widgets()
        self.create_panes(master)
        self.pack()

    def create_panes(self,master):
        #create first window
        self.video_panel = tk.PanedWindow(master, orient = HORIZONTAL)
        self.video_panel.pack(expand = 1)

        #VIDEO BUTTON  : add video stream button to first window
        self.stream_button = tk.Button(self.video_panel, text = "Start Stream", command= self.start_stream_thread)
        self.video_panel.add(self.stream_button)

        #split first window vertically
        self.window2 = tk.PanedWindow(self.video_panel, orient = VERTICAL)
        self.video_panel.add(self.window2)

        #add control panel to right window - top
        self.control_panel = tk.PanedWindow(self.window2, orient = VERTICAL)
        self.window2.add(self.control_panel)

        #add telemtry panel to right window - bottom
        self.telemetry_panel = tk.PanedWindow(self.window2, orient= VERTICAL)
        self.window2.add(self.telemetry_panel)


        # CONTROL BUTTONS: add buttons to control panel
        #LEFT
        self.button_left = tk.Button(self.control_panel,text ="L", image =self.left_arrow_img, height= 20,width=20,
                                     command=lambda m="left": self.start_command_thread(m), relief=GROOVE, padx=2, pady =2)
        self.control_panel.add(self.button_left)

        #RIGHT
        self.button_right = tk.Button(self.control_panel, text="R",image =self.right_arrow_img, height= 20,width=20,
                                      command=lambda m="right": self.start_command_thread(m), relief=GROOVE, padx=2, pady =2)
        self.control_panel.add(self.button_right)

        #FORWARD
        self.button_forward = tk.Button(self.control_panel, text="F", image =self.up_arrow_img, height= 20,width=20,
                                        command=lambda m="forward": self.start_command_thread(m), relief=GROOVE, padx=2, pady =2)
        self.control_panel.add(self.button_forward)

        #REVERSE
        self.button_reverse = tk.Button(self.control_panel, text="B", image =self.down_arrow_img, height= 20,width=20,
                                        command=lambda m="reverse": self.start_command_thread(m), relief=GROOVE, padx=2, pady =2)
        self.control_panel.add(self.button_reverse)

        #STOP
        self.button_stop = tk.Button(self.control_panel, text="S",  image =self.stop_button_img, height= 20,width=20,
                                     command=lambda m="stop": self.start_command_thread(m), relief=GROOVE, padx=2, pady =2)
        # self.button_stop.bind('<Button-1>', self.start_command_thread)
        self.control_panel.add(self.button_stop)

        # ADD MECHANISM CONTROL BUTTON

        #MOVE MECHANISM
        self.button_move_mech = tk.Button(self.control_panel, text="Collect Sample", command=lambda m="collect": self.start_command_thread(m),padx = 2, relief= GROOVE)
        self.control_panel.add(self.button_move_mech)
        #STOP MECHANISM
        self.button_stop_mech = tk.Button(self.control_panel, text="Stop Mechanism", command=lambda m="stopmech": self.start_command_thread(m),padx=2, relief=GROOVE)
        self.control_panel.add(self.button_stop_mech)

        # ADD COMMAND CONNECTION BUTTON
        self.button_command_connect = tk.Button(self.control_panel, text="Start Control", command = self.start_command_session,padx = 2, relief= GROOVE)
        self.control_panel.add(self.button_command_connect)

        #ADD CONNECT BUTTON
        self.connect_button = tk.Button(self.control_panel, text="Connect to Rover", command=self.start_telemetry_thread, padx = 2, relief= GROOVE)
        self.control_panel.add(self.connect_button)
        #ADD DISCONNECT BUTTON
        self.disconnect_button = tk.Button(self.control_panel, text="Disconnect from Rover",command=self.stop_telemetry_port, padx = 2, relief= GROOVE)
        self.control_panel.add(self.disconnect_button)

        #place the buttons in control panel where we want them - Using Grid
        self.button_stop.grid(column= 1, row =1)
        self.button_forward.grid(column=1, row=0)
        self.button_reverse.grid(column=1, row=2)
        self.button_left.grid(column=0, row=1)
        self.button_right.grid(column=2, row=1)
        self.button_move_mech.grid(column = 5, row = 1)
        self.button_stop_mech.grid(column = 6, row = 1)
        self.button_command_connect.grid(column = 7, row = 1)
        self.connect_button.grid(column = 5, row = 2)
        self.disconnect_button.grid(column=6, row=2)



        #MOTOR SLIDER LABEL
        self.speed_slider_label = Label(self.telemetry_panel, text="Set Duty Cycle : ", anchor='w')
        self.telemetry_panel.add(self.speed_slider_label)

        # TELEMETRY : add slider option for command to  telemetry (bad space management atm but no time)
        self.speed_slider = tk.Scale(self.telemetry_panel, orient=HORIZONTAL, from_=-100, to_=100, tickinterval=20, resolution=1, highlightbackground="black")
        self.speed_slider.bind("<ButtonRelease-1>", self.set_speed)
        self.telemetry_panel.add(self.speed_slider)

        # MECHANISM SLIDER LABEL
        self.mechanism_slider_label = Label(self.telemetry_panel, text="Set Mechanism Angle : ", anchor='w')
        self.telemetry_panel.add(self.mechanism_slider_label)

        # ADD SLIDER FOR MECHANISM CONTROL
        self.mechanism_slider = tk.Scale(self.telemetry_panel, orient=HORIZONTAL, from_=0, to_=100, tickinterval=10,
                                         resolution=1, highlightbackground="blue")
        self.mechanism_slider.bind("<ButtonRelease-1>", self.set_mechanism_angle)
        self.telemetry_panel.add(self.mechanism_slider)

        # BUCKET SLIDER LABEL
        self.bucket_slider_label = Label(self.telemetry_panel, text="Set Bucket Angle : ", anchor='w')
        self.telemetry_panel.add(self.bucket_slider_label)

        # ADD SLIDER FOR BUCKET CONTROL
        self.bucket_slider = tk.Scale(self.telemetry_panel, orient=HORIZONTAL, from_=0, to_=100, tickinterval=10,
                                         resolution=1, highlightbackground="red")
        self.bucket_slider.bind("<ButtonRelease-1>", self.set_bucket_angle)
        self.telemetry_panel.add(self.bucket_slider)


        # add display option for telemetry and command
        # #CONNECT BUTTON
        # self.connect_button = tk.Button(self.telemetry_panel, text="Connect to Rover", command=self.start_telemetry_thread)
        # self.telemetry_panel.add(self.connect_button)
        # #DISCONNECT BUTTON
        # self.disconnect_button = tk.Button(self.telemetry_panel, text="Disconnect from Rover",command=self.stop_telemetry_port)
        # self.telemetry_panel.add(self.disconnect_button)
        #LABEL TM WINDOW
        self.telemetry_label = Label(self.telemetry_panel, text="Telemetry window", anchor = 'w')
        self.telemetry_panel.add(self.telemetry_label)
        #TELEMETRY DISPLAY
        self.telemetry_text = ScrolledText(self.telemetry_panel, background="black", foreground="yellow", height =10, width = 20, padx=5, pady =5)
        self.telemetry_panel.add(self.telemetry_text)
        #COMMAND DISPLAY LABEL
        self.command_label = Label(self.telemetry_panel, text="Command window", anchor = 'w')
        self.telemetry_panel.add(self.command_label)
        #COMMAND DISPLAY
        self.command_text = ScrolledText(self.telemetry_panel, background="black", foreground="green", height =10, width = 20, padx=5, pady =5)
        self.telemetry_panel.add(self.command_text)


    def start_stream_thread(self):
        self.stream_thread = Thread(target = self.start_vlc)
        self.stream_thread.setDaemon(True)
        self.stream_thread.start()

    def start_vlc(self):
        subprocess.call(["C:\Program Files (x86)\VideoLAN\VLC\\vlc.exe"])


    def start_telemetry_thread(self):
        self.telemetry_thread = Thread(target=self.connect_to_pi)
        self.telemetry_thread.setDaemon(True)
        self.telemetry_thread.start()
        self.write_telemetry = Thread(target=self.start_telemetry_queue, daemon=True)
        self.write_telemetry.start()
        print(self.telemetry_queue.get())


    def start_telemetry_queue(self):
        self.telemetry_queue = Queue()
        while True:
            self.telemetry_text.insert('insert', self.telemetry_queue.get())
            # self.telemetry_text.delete(1.0, END)

    def stop_telemetry_port(self):
        print("disconnecting from rover")
        self.telemetry_text.insert('insert',"disconnecting from rover \n")

    def start_command_queue(self):
        print("started queue")
        self.command_queue = Queue()
        print(self.command_queue)
        self.command_text.insert('insert',self.command_queue.get())

    def start_command_thread(self,key_name):
        # print(button_name)
        self.command_thread = Thread(target=self.command_pi(key_name))
        self.command_thread.setDaemon(True)
        self.command_thread.start()

    def set_speed(self, event):
        self.rover_speed = self.speed_slider.get()*10
        # print(self.rover_speed)

    def set_mechanism_angle(self, event):
        self.mechanism_angle = self.mechanism_slider.get()
        print(self.mechanism_angle)

    def set_bucket_angle(self, event):
        self.bucket_angle = self.bucket_slider.get()
        print(self.bucket_angle)
    # @staticmethod
    # def leftKey(event):
    #     # if event == 37:
    #     print(event.keysym)

    def command_pi(self, key_name):
        self.write_command = Thread(target=self.start_command_queue, daemon=True)
        self.write_command.start()
        print("hey started queue thread")
        # print(self.command_queue.get())

        if key_name.keysym=="Left":
            # self.mode = "left"
            # while self.mode =="left"
            command = []
            for i in range(4):
                message_name = "set_motor_speed"  # message_name = "emergency_stop"
                motor_data = [i, self.rover_speed]
                data_send = md.encode_message(message_name, motor_data)

                #decoded_data = md.decode_message(encoded_data)
                # print(list(decoded_data))
                # command.append(encoded_data)
                try:
                    self.command_sock.sendall(data_send)
                    time.sleep(0.1)
                except socket.error:
                    print("Failed to send command")
                    sys.exit()
                self.command_received_data = self.command_sock.recv(1024)
                self.command_queue.put(self.command_received_data)

            # data_send = command
            # self.command_text.insert('insert',"moving left, command: " + str(command) + '\n')
            print("this is left button {}".format(command))
        elif  key_name.keysym=="Right":
            # self.mode = "right"
            command = [1, self.rover_speed]
            data_send = md.encode_message("set_motor_speed", command)
            try:
                self.command_sock.sendall(data_send)
                time.sleep(0.1)
            except socket.error:
                print("Failed to send command")
                sys.exit()
            self.command_received_data = self.command_sock.recv(1024)
            self.command_queue.put(self.command_received_data)
            # self.command_text.insert('insert', "moving right, command: " + str(command) + '\n')
            print("this is right button {}".format(command))
        elif  key_name.keysym=="Up":
            command = []
            for i in range(4):
                message_name = "set_motor_speed"  # message_name = "emergency_stop"
                motor_data = [i, self.rover_speed]
                data_send = md.encode_message(message_name, motor_data)

                #decoded_data = md.decode_message(encoded_data)
                # print(list(decoded_data))
                # command.append(encoded_data)
                try:
                    self.command_sock.sendall(data_send)
                    time.sleep(0.1)
                except socket.error:
                    print("Failed to send command")
                    sys.exit()
                self.command_received_data = self.command_sock.recv(1024)
                self.command_queue.put(self.command_received_data)
            # self.command_text.insert('insert', "moving forward, command: " + str(command) + '\n')
            print("this is forward button {}".format(command))
        elif  key_name.keysym=="Down":
            # self.mode = "reverse"
            command = [1, -self.rover_speed]
            data_send = md.encode_message("set_motor_speed", command)
            # self.command_text.insert('insert', "moving reverse, commmand: " + str(command) + '\n')
            print("this is reverse button {}".format(command))
        elif key_name.keysym == "s":
            # self.mode = "stop"
            command = [0]
            data_send = md.encode_message("emergency_stop", command)
            # self.command_text.insert('insert', "That's too much let's STOP, command: " + str(command) + '\n')
            print("this is stop button {}".format(command))
        elif key_name.keysym == "m":
            # self.mode = "collect" MECHANISM SERVO
            print("starting mechanism")
            command = [1,self.mechanism_angle]
            data_send = md.encode_message("set_mechanism_angle", command)
            try:
                self.command_sock.sendall(data_send)
                time.sleep(0.1)
            except socket.error:
                print("Failed to send command")
                sys.exit()
            self.command_received_data = self.command_sock.recv(1024)
            self.command_queue.put(self.command_received_data)
            # self.command_text.insert('insert', "Let's collect samples, command: " + str(command) + '\n')
            print("this is collection button {}".format(command))
        elif key_name.keysym == "b":
            # self.mode = "bucket" BUCKET SERVO
            command = [1,self.bucket_angle]
            data_send = md.encode_message("set_mechanism_angle", command)
            try:
                self.command_sock.sendall(data_send)
                time.sleep(0.1)
            except socket.error:
                print("Failed to send command")
                sys.exit()
            self.command_received_data = self.command_sock.recv(1024)
            self.command_queue.put(self.command_received_data)
            # self.command_text.insert('insert', "Sample collection done, command: " + str(command) + '\n')
            print("this is mechanism stop button {}".format(command))
        else:
            print("No such command. Please check !")

        # try:
        #     self.command_sock.sendall(data_send)
        # except socket.error:
        #     print("Failed to send command")
        #     sys.exit()
        # self.command_received_data = self.command_sock.recv(1024)
        # self.command_queue.put(self.command_received_data)

        # self.command_port = 12340
        # # self.command_port = 80
        # try:
        #     self.command_sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        # except socket.error:
        #     print("Failed to create command socket")
        #     sys.exit() #quit program
        # try:
        #     command_host = socket.gethostbyname("192.168.137.108") #www.google.com or IP of rover
        # except socket.gaierror:
        #     print("Failed to get command host")
        #     sys.exit()
        # self.command_sock.connect((command_host,self.command_port))
        #
        # # while True:
        # # print("sending request")
        # try:
        #     self.command_sock.sendall(b"1")
        # except socket.error:
        #     print("Failed to send command")
        #     sys.exit()
        # self.command_received_data = self.command_sock.recv(1024)
        # self.command_queue.put(self.command_received_data)
        # print(self.command_queue.get())
        # time.sleep(1)

    def start_command_session(self):
        self.command_port = 12341 #12340 is default
        # self.command_port = 80
        try:
            self.command_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except socket.error:
            print("Failed to create command socket")
            sys.exit()  # quit program
        try:
            command_host = socket.gethostbyname("192.168.137.33")  # www.google.com or IP of rover
        except socket.gaierror:
            print("Failed to get command host")
            sys.exit()
        self.command_sock.connect((command_host, self.command_port))

        # while True:
        # print("sending request")


    def connect_to_pi(self):
        # self.port = 80
        self.port = 12345
        # self.message = b"GET / HTTP/1.1\r\n\r\n"
        self.message = b"1"
        try:
            self.telemetry_sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        except socket.error:
            print("Failed to create socket")
            sys.exit() #quit program
        try:
            host = socket.gethostbyname("192.168.137.33")  # www.google.com or IP
            # host = socket.gethostbyname("www.google.com") #www.google.com or IP
        except socket.gaierror:
            print("Failed to get host")
            sys.exit()

        self.telemetry_sock.connect((host,self.port))
        while True:
            # print("sending request")
            try:
                self.telemetry_sock.sendall(self.message)
            except socket.error:
                print("Failed to send")
                sys.exit()

            self.telemetry_received_data = self.telemetry_sock.recv(1024) #wait until receive data (blocking type)
            self.telemetry_queue.put(self.telemetry_received_data)
            time.sleep(1)
        # mysock.close() #to free port



root = tk.Tk()
app = Application(master=root)
app.master.title('Leviathans of Cranfield')
app.mainloop()

