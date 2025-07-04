using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Sum
{
    public partial class Form1 : Form
    {
        private readonly HttpClient client;

        public Form1()
        {
            InitializeComponent();
            client = new HttpClient { BaseAddress = new Uri("http://localhost:5039") };
        }

        private async void button1_Click(object sender, EventArgs e)
        {
            if (int.TryParse(textBox1.Text, out int X) && int.TryParse(textBox2.Text, out int Y))
            {
                try
                {
                    string url = $"/sum?X={X}&Y={Y}";
                    var response = await client.PostAsync(url, null);
                    response.EnsureSuccessStatusCode();
                    textBox3.Text = await response.Content.ReadAsStringAsync();
                }
                catch (HttpRequestException ex)
                {
                    MessageBox.Show("Ошибка при выполнении запроса: " + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Неизвестная ошибка: " + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
            else
            {
                MessageBox.Show("Введите корректные числа", "Ошибка ввода", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!char.IsDigit(e.KeyChar) && e.KeyChar != '\b')
            {
                e.Handled = true;
            }
        }

        private void textBox2_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!char.IsDigit(e.KeyChar) && e.KeyChar != '\b')
            {
                e.Handled = true;
            }
        }
    }
}
