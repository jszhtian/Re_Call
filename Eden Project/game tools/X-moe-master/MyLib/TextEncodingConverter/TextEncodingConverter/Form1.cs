using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TextEncodingConverter
{
    public partial class Form1 : Form
    {

        private List<string> ItemList = null;
        public Form1()
        {
            InitializeComponent();

            textBox1.Text = "*.*";
            comboBox1.Items.Add("MBS");
            comboBox1.Items.Add("UTF-16LE");
            comboBox1.Items.Add("UTF-16BE");
            comboBox1.Items.Add("UTF-8");

            comboBox1.SelectedIndex = 3;
            //listView1.ShowGroups = true;

        }

        //在运行的时候测试cp
        private Encoding GetEncode(byte[] buffer)
        {
            if (buffer.Length <= 0 || buffer[0] < 239)
            {
                return Encoding.Default;
            }
            if (buffer[0] == 239 && buffer[1] == 187 && buffer[2] == 191)
            {
                return Encoding.UTF8;
            }
            if (buffer[0] == 254 && buffer[1] == byte.MaxValue)
            {
                return Encoding.BigEndianUnicode;
            }
            if (buffer[0] == byte.MaxValue && buffer[1] == 254)
            {
                return Encoding.Unicode;
            }
            return Encoding.Default;
        }


        private string GetTextData(byte[] buffer, Encoding encoding)
        {
            if (Equals(encoding, Encoding.UTF8))
            {
                return encoding.GetString(buffer, 3, buffer.Length - 3);
            }
            if (Equals(encoding, Encoding.BigEndianUnicode) || Equals(encoding, Encoding.Unicode))
            {
                return encoding.GetString(buffer, 2, buffer.Length - 2);
            }
            return encoding.GetString(buffer);
        }

        private void AutoGetEncoding()
        {
            string pattern = textBox1.Text.Trim();
            string folder = textBox2.Text.Trim();
            if (string.IsNullOrEmpty(pattern))
            {
                MessageBox.Show("请填写文件Filter", "提示");
            }
            else if (string.IsNullOrEmpty(folder))
            {
                MessageBox.Show("请选择目标文件夹。", "提示");
            }
            else
            {
                try
                {
                    List<string> FileLists = GetFileList(folder, pattern);
                    for (int i = 0; i < FileLists.Count; ++i)
                    {
                        //MessageBox.Show(FileLists[i]);
                        AddFileItem(i, FileLists[i], folder, Path.GetExtension(FileLists[i]));
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("错误");
                }
            }
        }

        public string convEncode(string write, int fromEncode, int toEncode)
        {
            Encoding From, To;
            From = Encoding.GetEncoding(fromEncode);
            To = Encoding.GetEncoding(toEncode);
            byte[] temp = From.GetBytes(write);
            byte[] temp1 = Encoding.Convert(From, To, temp);
            return To.GetString(temp1);
        }

        private void AddFileItem(int i, string path, string folder, string extension)
        {

            FileStream fileStream = File.OpenRead(path);
            var buffer = new byte[fileStream.Length];
            fileStream.Read(buffer, 0, buffer.Length);
            fileStream.Close();
            fileStream.Dispose();
            Encoding encode = GetEncode(buffer);

            var item = new ListViewItem();

            item.Text = path.Replace(folder, "").TrimStart('\\');
            //MessageBox.Show(item.Text);
            item.SubItems.Add(encode.BodyName);
            item.SubItems.Add("未转换");

            listView1.Items.Add(item);
            listView1.Items[listView1.Items.Count - 1].EnsureVisible();

        }

        private void button1_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog bDialog = new FolderBrowserDialog();
            bDialog.ShowDialog(this);
            string selectedPath = bDialog.SelectedPath;
            if (string.IsNullOrEmpty(selectedPath))
                return;
            textBox2.Text = selectedPath;
        }

        private List<string> GetFileList(string Folder, string Filter)
        {
            string[] strArray = null;
            listView1.Items.Clear();
            try
            {
                strArray = Directory.GetFiles(Folder, Filter, checkBox1.Checked ?
                    SearchOption.AllDirectories : SearchOption.TopDirectoryOnly);
            }
            catch
            {
                MessageBox.Show("错误:无法打开文件夹");
            }
            if (strArray != null)
            {
                ItemList = new List<string>(strArray);
                return ItemList;
            }
            return new List<string>();
        }

        private void WriteTextFile(string filepath, string body, Encoding encoding)
        {
            if (File.Exists(filepath))
            {
                File.Delete(filepath);
            }

            byte[] bytes = encoding.GetBytes(body);
            FileStream fileStream = File.Open(filepath, FileMode.CreateNew, FileAccess.Write);
            if (Equals(encoding, Encoding.UTF8))
            {
                fileStream.WriteByte(239);
                fileStream.WriteByte(187);
                fileStream.WriteByte(191);
            }
            else if (Equals(encoding, Encoding.BigEndianUnicode))
            {
                fileStream.WriteByte(254);
                fileStream.WriteByte(byte.MaxValue);
            }
            else if (Equals(encoding, Encoding.Unicode))
            {
                fileStream.WriteByte(byte.MaxValue);
                fileStream.WriteByte(254);
            }
            fileStream.Write(bytes, 0, bytes.Length);
            fileStream.Flush();
            fileStream.Close();
            fileStream.Dispose();
        }

        //start
        private void button2_Click(object sender, EventArgs e)
        {
            string UserCP = textBox3.Text;
            Encoding encoding;

            listView1.BeginUpdate();
            switch (comboBox1.SelectedIndex)
            {
                case 0:
                    encoding = Encoding.Default;
                    break;
                case 1:
                    encoding = Encoding.Unicode;
                    break;
                case 2:
                    encoding = Encoding.BigEndianUnicode;
                    break;
                case 3:
                    encoding = Encoding.UTF8;
                    break;
                default:
                    MessageBox.Show("编码选择错误");
                    return;
            }
            var folder = textBox2.Text.Trim();

            if (ItemList != null)
            {
                var sucress = 0;
                for (int i = 0; i < ItemList.Count; ++i)
                {
                    string extension = Path.GetExtension(ItemList[i]);
                    FileStream fileStream = File.Open(ItemList[i], FileMode.Open, FileAccess.ReadWrite);
                    var buffer = new byte[fileStream.Length];
                    fileStream.Read(buffer, 0, buffer.Length);
                    fileStream.Close();
                    fileStream.Dispose();
                    var fileEncoding = GetEncode(buffer);
                    string result = "无需转换";
                    if (!Equals(fileEncoding, encoding))
                    {
                        try
                        {
                            string txt = null;
                            if((!string.IsNullOrEmpty(textBox3.Text.Trim())) &&
                                encoding == Encoding.Default)
                            {
                                int srcEncoding = Convert.ToInt32(textBox3.Text.Trim());
                                string strBuffer = System.Text.Encoding.ASCII.GetString(buffer);
                                txt = convEncode(strBuffer, encoding.CodePage, srcEncoding);
                            }
                            else
                            {
                                txt = GetTextData(buffer, encoding);
                            }
                            WriteTextFile(ItemList[i], txt, encoding);
                            sucress++;
                            result = "转换成功";
                        }
                        catch
                        {
                            result = "转换失败";
                        }
                    }

                    var item = new ListViewItem();

                    item.Text = ItemList[i].Replace(folder, "").TrimStart('\\');
                    item.SubItems.Add(fileEncoding.BodyName);
                    item.SubItems.Add(result);

                    listView1.Items[i] = item;
                }
            }
            listView1.EndUpdate();
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e)
        {
            listView1.Items.Clear();

            listView1.BeginUpdate();
            try
            {
                AutoGetEncoding();
            }
            catch(Exception error)
            {
                MessageBox.Show(error.ToString());
            }
            listView1.EndUpdate();
        }

    }
}
