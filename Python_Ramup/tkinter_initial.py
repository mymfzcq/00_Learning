import tkinter as tk
import sys
import argparse


class win32_app(tk.Frame):
    """docstring for ClassName"""

    def __init__(self, baseframe):
        tk.Frame.__init__(self, baseframe)
        self.parent_frame = baseframe
        self.config(height=200, width=100, background="grey")
        self.pack(fill=None, expand=0)
        self.var1 = tk.StringVar()
        #self.place(relx=.5, rely=.5, anchor="c")
        # super(ClassName, self).__init__()
        self.initUI()

    def initUI(self):
        self.button1 = tk.Button(
            master=self, height=1, bg="#eeeeee", text="button1", fg="red",command=self.button_callback)
        self.button1.pack()
        self.create_label1()
        self.create_entry1()

    def create_label1(self):
        self.label1 = tk.Label(self, text="label1", bg="#888888")
        self.label1.pack()
    def create_entry1(self):
        self.var1.set("1231231231")
        #self.var1.trace("w", self.entry1_callback)
        self.entry1 = tk.Entry(
            self, justify=tk.CENTER, textvariable=self.var1)
        self.entry1.pack()
        pass

    def entry1_callback(self):
        print(self.var1.get())
        pass
    def button_callback(self):
        print (self.var1.get())
        pass

def main(mode):

    if mode == "test":
        tk_root = tk.Tk()
        tk_root.title("itis a function")
        # tk_root.geometry("800x480+300+300")
        my_win32_app = win32_app(tk_root)
        tk_root.mainloop()
    pass

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="tkiner py pragram")
    parser.add_argument("-m", "--mode", type=str,
                        help="the mode of tkinter win32 application")
    arg = parser.parse_args()

    sys.exit(main(arg.mode))
