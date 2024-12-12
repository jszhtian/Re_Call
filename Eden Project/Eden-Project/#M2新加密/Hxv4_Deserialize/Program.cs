
namespace Hxv4_Deserialize
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var data = File.ReadAllBytes(@"C:\Users\Shiroha\Desktop\花鐘カナデグラム\MEM_03052170_0002B3D3.mem");
            var deserializer = new Deserializer();
            var stream = new MemoryStream(data);
            deserializer.Deserialize(stream);
        }
    }
}
