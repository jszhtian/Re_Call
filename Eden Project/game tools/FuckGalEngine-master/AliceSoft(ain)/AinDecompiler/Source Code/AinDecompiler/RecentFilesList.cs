using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Collections.ObjectModel;

namespace AinDecompiler
{
    public class RecentFilesList
    {
        static string[] dummy = new string[0];
        public static RecentFilesList FilesList = new RecentFilesList();
        public int MaxSize = 16;
        string[] filesList;

        public RecentFilesList()
        {
            ReadFromRegistry();
        }

        private void ReadFromRegistry()
        {
            if (!RegistryUtility.TryGetSetting("RecentFiles", "", out filesList))
            {
                filesList = dummy;
            }
            if (filesList == null)
            {
                filesList = dummy;
            }
        }
        public void Add(string fileName)
        {
            ReadFromRegistry();
            int index = Array.IndexOf(filesList, fileName);
            if (index == -1)
            {
                filesList = Enumerable.Repeat(fileName, 1).Concat(filesList).ToArray();
                if (filesList.Length > MaxSize)
                {
                    filesList = filesList.Take(MaxSize).ToArray();
                }
            }
            else
            {
                filesList = Enumerable.Repeat(fileName, 1).Concat(filesList.Take(index).Concat(filesList.Skip(index + 1).Take(filesList.Length - (index + 1)))).ToArray();
            }
            SaveToRegistry();
        }

        private void SaveToRegistry()
        {
            RegistryUtility.SaveSetting("RecentFiles", "", filesList);
        }

        public string[] GetList()
        {
            return filesList;
        }
    }
}
