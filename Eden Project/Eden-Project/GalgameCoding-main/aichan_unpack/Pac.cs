using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace aichan_unpack
{
    static class Pac
    {
        public static void Unpack(string filePath, string outputPath)
        {
            // 打开封包文件
            using (var stream = File.OpenRead(filePath))
            using (var reader = new BinaryReader(stream))
            {
                // 读取文件名区块
                var name_chunk = reader.ReadBytes(0x4000);
                // 读取偏移量区块
                var offset_chunk = reader.ReadBytes(0x1000);

                // 文件名数组
                var name_tbl = new List<string>();
                // 偏移量数组
                var offset_tbl = new List<int>();

                // 文件名编码
                Encoding encoding = Encoding.GetEncoding("shift_jis");

                // 解析文件名
                for (int i = 0; i < name_chunk.Length; i += 16)
                {
                    int j;

                    // 计算文件名长度
                    for (j = i; j < name_chunk.Length; j++)
                        if (name_chunk[j] == 0)
                            break;

                    // 如果文件名长度为零则已读完
                    if (j == i)
                        break;

                    // 根据指定编码把字节转换为字符串
                    var name = encoding.GetString(name_chunk, i, j - i);

                    // 把文件名存到数组
                    name_tbl.Add(name);
                }

                // 解析偏移量
                for (int i = 0; i < offset_chunk.Length; i += 4)
                {
                    // 取小端四字节整数
                    var val = BitConverter.ToInt32(offset_chunk, i);

                    // 如果值为零则已读完
                    if (val == 0)
                        break;

                    // 把偏移量存到数组
                    offset_tbl.Add(val);
                }

                // 检查文件名数量和偏移数量是否匹配，偏移数量比文件名数量多一个。
                if (name_tbl.Count != offset_tbl.Count - 1)
                    throw new Exception("failed to parse the index.");

                // 创建输出目录
                Directory.CreateDirectory(outputPath);

                // 解包所有文件
                for (int i = 0; i < name_tbl.Count; i++)
                {
                    // 文件指针移动到数据所在位置
                    stream.Position = offset_tbl[i];
                    // 计算文件数据长度
                    var length = offset_tbl[i + 1] - offset_tbl[i];
                    // 从封包中读取文件数据
                    var data = reader.ReadBytes(length);

                    // 拼接输出文件路径
                    var name = name_tbl[i];
                    var path = Path.Combine(outputPath, name);
                    Console.WriteLine($"Extract: {name}");
                    // 写出文件数据
                    File.WriteAllBytes(path, data);
                }

                // 解包完成
            }
        }
    }
}
