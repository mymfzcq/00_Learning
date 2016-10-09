using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using System.Net;
namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        private Thread newthread;
        static string S1 = "";
        static int S2 = 222;
        static int temp = 0;
        MethodInvoker method;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            MessageBox.Show("form load event");
        }

        private void Form1_Click(object sender, EventArgs e)
        {
            MessageBox.Show(string.Format("pos x is {0},pos y is {1}", ((MouseEventArgs)e).X.ToString(), ((MouseEventArgs)e).Y.ToString()));
        }

        private void my_button_Click(object sender, EventArgs e)
        {
            my_button.Visible = false;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Thread thread1 = Thread.CurrentThread;
            MessageBox.Show(thread1.ManagedThreadId.ToString());
            //temp = (temp + 1) % 2;
            //if (temp == 1)
            //{
            //    label1.Text = "button1 clicked";
            //}
            //else if (temp == 0)
            //{
            //    label1.Text = "button1";
            //}
            //CheckForIllegalCrossThreadCalls = false;
            newthread = new Thread(new ParameterizedThreadStart(calc));
            newthread.Start(this);

        }
        public void sub_calc()
                    {
                        Thread thread3 = Thread.CurrentThread;
                        //MessageBox.Show(thread3.ManagedThreadId.ToString());

                        this.Text = thread3.ManagedThreadId.ToString();
                    }
        public void calc(object o)
        {
            method = new MethodInvoker(sub_calc);
            Thread thread2 = Thread.CurrentThread;
            MessageBox.Show(newthread.ManagedThreadId.ToString());
            for (int i = 0; i < 100000; i++)
            {

                if (((Form1)o).InvokeRequired)
                {
                    //MessageBox.Show(thread2.ManagedThreadId.ToString());

                    this.Invoke(method);
                    //this.Invoke(new MethodInvoker(() => ((Form1)o).Text = i.ToString()));

                }
                else
                {
                    ((Form1)o).Text = i.ToString();
                }
                if (i == 80000)
                {
                    newthread.Suspend();
                }
            }
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            MessageBox.Show(listBox1.SelectedItem.ToString());
            using (WebClient client = new WebClient())
            {
                string s = client.DownloadString(url);
            }
        }
    }
}
