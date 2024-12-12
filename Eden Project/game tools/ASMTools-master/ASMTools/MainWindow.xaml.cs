using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Gee.External.Capstone;
using Gee.External.Capstone.X86;
using Keystone;


namespace ASMTools
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public static byte[] StrToByteArray(string str)
        {
            Dictionary<string, byte> hexindex = new Dictionary<string, byte>();
            for (int i = 0; i <= 255; i++)
                hexindex.Add(i.ToString("X2"), (byte)i);

            List<byte> hexres = new List<byte>();
            for (int i = 0; i < str.Length; i += 3)
                hexres.Add(hexindex[str.Substring(i, 2)]);

            return hexres.ToArray();
        }
        public MainWindow()
        {
            InitializeComponent();
        }
        private void ASM2HEX_CanExec(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }
        private void ASM2HEX_Exec(object sender, ExecutedRoutedEventArgs e)
        {
            var AsmStr = this.ASMBox.Text;
            var ins = AsmStr.Replace("\n",";");
            dynamic disassembleMode;
            if (this.Arch.SelectedIndex == 0)
            {
                disassembleMode = Mode.X32;
            }
            else if (this.Arch.SelectedIndex == 1)
            {
                disassembleMode = Mode.X64;
            }
            else
            {
                disassembleMode = Mode.X32;
            }
            try
            {
                using (Engine keystone = new Engine(Architecture.X86, disassembleMode) { ThrowOnError = true })
                {
                    ulong address = 0;
                    EncodedData enc = keystone.Assemble(ins, address);
                    var hexCode = BitConverter.ToString(enc.Buffer).Replace("-", " ");
                    this.HEXBox.Text = hexCode;
                }
            }
            catch (Exception ex){
                MessageBox.Show(ex.ToString(), "Exception");
            }
           

        }
        private void HEX2ASM_CanExec(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }
        private void HEX2ASM_Exec(object sender, ExecutedRoutedEventArgs e)
        {
            var HexText = this.HEXBox.Text.ToUpper();
            var BinaryCode = StrToByteArray(HexText);
            dynamic disassembleMode;
            if (this.Arch.SelectedIndex == 0)
            {
                disassembleMode = X86DisassembleMode.Bit32;
            }
            else if (this.Arch.SelectedIndex == 1)
            {
                disassembleMode = X86DisassembleMode.Bit64;
            }
            else
            {
                disassembleMode = X86DisassembleMode.Bit32;
            }
            string disasmText="";
            try
            {
                using (var disassembler = CapstoneDisassembler.CreateX86Disassembler(disassembleMode))
                {
                    disassembler.EnableInstructionDetails = true;
                    disassembler.DisassembleSyntax = DisassembleSyntax.Intel;
                    dynamic instructions = disassembler.Disassemble(BinaryCode);
                    foreach (var instruction in instructions)
                    {
                        var address = instruction.Address;
                        var id = instruction.Id;
                        var mnemonic = instruction.Mnemonic;
                        var operand = instruction.Operand;
                        disasmText += String.Format("{0} \t {1} \n", mnemonic, operand);
                    }

                }
                this.ASMBox.Text = disasmText;
            }
            catch (Exception ex) {
                MessageBox.Show(ex.ToString(), "Exception");
            }
            
            
        }
    }


}
