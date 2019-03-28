using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

// Based on the excellent reply by Servy at:
// https://stackoverflow.com/questions/18726852/redirecting-console-writeline-to-textbox

namespace howto_capture_console
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        // Install a new output TextWriter for the Console window.
        private void Form1_Load(object sender, EventArgs e)
        {
            TextBoxWriter writer = new TextBoxWriter(txtConsole);
            Console.SetOut(writer);
        }

        // Write to the Console window.
        private void btnWrite_Click(object sender, EventArgs e)
        {
            Console.WriteLine(txtOutput.Text);
            txtOutput.Clear();
        }
    }
}
