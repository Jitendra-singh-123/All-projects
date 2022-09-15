
#Tkinter is the standard GUI library for Python. Python when combined with Tkinter provides a fast and easy way to create GUI applications.
import tkinter as tk
import datetime

#for windowsound
import winsound as ws


class Countdown(tk.Frame):
    def __init__(self,master):
        super().__init__(master)
        self.create_widgets()
        self.show_widgets()
        self.seconds_left=0
        self._timer_on=False

#showing the widgets like all buttons
    def show_widgets(self):
        self.label.pack()
        self.entry.pack()
        self.start.pack()
        self.stop.pack()
        self.reset.pack()

#creating all buttons
    def create_widgets(self):
        self.label=tk.Label(self,text="Enter the time in seconds.")
        self.entry=tk.Entry(self,justify="center")
        self.entry.focus_set()
        self.reset=tk.Button(self,text="Reset", command=self.reset_button)

        self.stop=tk.Button(self,text="stop",command=self.stop_button)
        self.start=tk.Button(self,text="Start", command=self.start_button)

        
#main logic of countdown timer
    def countdown(self):
        self.label["text"]=self.convert_seconds_left_to_time()

        if self.seconds_left:
            #decreasing the time by 1
            self.seconds_left-=1

            #Recursively decrease the time by 1 till seconds left==0
            self._timer_on=self.after(1000,self.countdown)

        else:
            self._timer_on=False
            #playing sound after time is over
            ws.PlaySound('mp.wav',ws.SND_FILENAME)

#set 0 to time and forget all the buttons and pack all the buttons again for new timer
    def reset_button(self):
        self.seconds_left=0
        self.stop_timer()
        self._timer_on=False
        self.label["text"]="Enter the time in seconds"
        self.start.forget()
        self.stop.forget()
        self.reset.forget()
        self.start.pack()
        self.stop.pack()
        self.reset.pack()

#Halt the timer and ask to set times again
    def stop_button(self):
        self.seconds_left=int(self.entry.get())
        self.stop_timer()

#start the time by asking the countdown time from user
    def start_button(self):
        self.seconds_left=int(self.entry.get())
        self.stop_timer()
        self.countdown()
        self.start.forget()
        self.stop.forget()
        self.reset.forget()
        self.start.pack()
        self.stop.pack()
        self.reset.pack()

    def stop_timer(self):
        if self._timer_on:
            self.after_cancel(self._timer_on)
            self._timer_on=False

    def convert_seconds_left_to_time(self):
        return datetime.timedelta(seconds=self.seconds_left)


#Main function
if __name__=="__main__":
    root=tk.Tk()
    #can't resize the window
    root.resizable(False,False)

    countdown=Countdown(root)
    countdown.pack()

    root.mainloop()

     

    
        

        


    


    
