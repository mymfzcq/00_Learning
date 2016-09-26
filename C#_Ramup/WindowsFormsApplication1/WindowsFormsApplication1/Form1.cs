using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        static string S1 = "";
        static int S2 = 222;

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
    }
}
