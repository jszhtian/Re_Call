using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using Microsoft.Win32;
using System.Reflection;
using System.Windows.Forms;

namespace scriptthingy
{
    public static class RegistryUtility
    {
        static string GetKeyName(string path)
        {
            string appName = Application.ProductName;

            string keyName = "Software\\" + appName;
            if (!String.IsNullOrEmpty(path))
            {
                keyName += "\\" + path;
            }
            return keyName;
        }

        static RegistryKey OpenSubKey(string fullPath, bool writable)
        {
            string[] dirs = fullPath.Split('\\');
            RegistryKey key = Registry.CurrentUser;
            foreach (var dir in dirs)
            {
                var nextKey = key.OpenSubKey(dir, writable);
                if (nextKey == null)
                {
                    if (!writable)
                    {
                        return null;
                    }
                    else
                    {
                        nextKey = key.CreateSubKey(dir);
                    }
                }
                key.Close();
                key = nextKey;
            }
            return key;
        }

        public static string GetSetting(string settingName, string path, string defaultValue)
        {
            string value;
            if (TryGetSetting(settingName, path, out value))
            {
                return value;
            }
            return defaultValue;
        }

        public static bool TryGetSetting(string settingName, string path, out string value)
        {
            string keyName = GetKeyName(path);
            using (var k1 = OpenSubKey(keyName, false))
            {
                if (k1 != null)
                {
                    if (k1.GetValueNames().Contains(settingName))
                    {
                        var kind = k1.GetValueKind(settingName);
                        if (kind == RegistryValueKind.String)
                        {
                            value = (string)k1.GetValue(settingName);
                            return true;
                        }
                    }
                }
            }
            value = null;
            return false;
        }

        public static int GetSetting(string settingName, string path, int defaultValue)
        {
            int value;
            if (TryGetSetting(settingName, path, out value))
            {
                return value;
            }
            return defaultValue;
        }
        public static bool TryGetSetting(string settingName, string path, out int value)
        {
            string keyName = GetKeyName(path);
            using (var k1 = OpenSubKey(keyName, false))
            {
                if (k1 != null)
                {
                    if (k1.GetValueNames().Contains(settingName))
                    {
                        var kind = k1.GetValueKind(settingName);
                        if (kind == RegistryValueKind.DWord)
                        {
                            value = (int)k1.GetValue(settingName);
                            return true;
                        }
                        if (kind == RegistryValueKind.String)
                        {
                            int intValue = 0;
                            if (Int32.TryParse((string)k1.GetValue(settingName), out intValue))
                            {
                                value = intValue;
                                return true;
                            }
                        }
                    }
                }
            }
            value = 0;
            return false;
        }

        public static bool GetSetting(string settingName, string path, bool defaultValue)
        {
            bool value;
            if (TryGetSetting(settingName, path, out value))
            {
                return value;
            }
            return defaultValue;
        }
        public static bool TryGetSetting(string settingName, string path, out bool value)
        {
            string keyName = GetKeyName(path);
            using (var k1 = OpenSubKey(keyName, false))
            {
                if (k1 != null)
                {
                    if (k1.GetValueNames().Contains(settingName))
                    {
                        var kind = k1.GetValueKind(settingName);
                        if (kind == RegistryValueKind.DWord)
                        {
                            value = ((int)k1.GetValue(settingName)) != 0;
                            return true;
                        }
                        if (kind == RegistryValueKind.String)
                        {
                            int intValue = 0;
                            if (Int32.TryParse((string)k1.GetValue(settingName), out intValue))
                            {
                                value = intValue != 0;
                                return true;
                            }
                            bool boolValue;
                            if (bool.TryParse((string)k1.GetValue(settingName), out boolValue))
                            {
                                value = boolValue;
                                return true;
                            }
                        }
                    }
                }
            }
            value = false;
            return false;
        }

        public static string GetSetting(string settingName, string defaultValue)
        {
            return GetSetting(settingName, "", defaultValue);
        }
        public static int GetSetting(string settingName, int defaultValue)
        {
            return GetSetting(settingName, "", defaultValue);
        }
        public static bool GetSetting(string settingName, bool defaultValue)
        {
            return GetSetting(settingName, "", defaultValue);
        }

        public static void SaveSetting(string settingName, string path, string value)
        {
            string keyName = GetKeyName(path);
            using (var k1 = OpenSubKey(keyName, true))
            {
                k1.SetValue(settingName, value, RegistryValueKind.String);
            }
        }
        public static void SaveSetting(string settingName, string path, int value)
        {
            string keyName = GetKeyName(path);
            using (var k1 = OpenSubKey(keyName, true))
            {
                k1.SetValue(settingName, value, RegistryValueKind.DWord);
            }
        }
        public static void SaveSetting(string settingName, string path, bool value)
        {
            string keyName = GetKeyName(path);
            using (var k1 = OpenSubKey(keyName, true))
            {
                k1.SetValue(settingName, value, RegistryValueKind.DWord);
            }
        }
        public static void SaveSetting(string settingName, string value)
        {
            SaveSetting(settingName, "", value);
        }
        public static void SaveSetting(string settingName, int value)
        {
            SaveSetting(settingName, "", value);
        }
        public static void SaveSetting(string settingName, bool value)
        {
            SaveSetting(settingName, "", value);
        }

        public static string[] ListKeys(string path)
        {
            string keyName = GetKeyName(path);
            using (var k1 = OpenSubKey(keyName, true))
            {
                if (k1 == null)
                {
                    return new string[0];
                }
                return k1.GetSubKeyNames();
            }
        }

        public static void GetObject(string path, object obj)
        {
            var type = obj.GetType();
            var properties = type.GetProperties(BindingFlags.Public | BindingFlags.Instance | BindingFlags.FlattenHierarchy);
            foreach (var property in properties)
            {
                if (property.PropertyType == typeof(string))
                {
                    string stringValue;
                    if (TryGetSetting(property.Name, path, out stringValue))
                    {
                        property.SetValue(obj, stringValue, null);
                    }
                }
                if (property.PropertyType == typeof(int))
                {
                    int intValue;
                    if (TryGetSetting(property.Name, path, out intValue))
                    {
                        property.SetValue(obj, intValue, null);
                    }
                }
                if (property.PropertyType == typeof(bool))
                {
                    bool boolValue;
                    if (TryGetSetting(property.Name, path, out boolValue))
                    {
                        property.SetValue(obj, boolValue, null);
                    }
                }
            }
        }

        public static void SaveObject(string path, object obj)
        {
            var type = obj.GetType();
            var properties = type.GetProperties(BindingFlags.Public | BindingFlags.Instance | BindingFlags.FlattenHierarchy);
            foreach (var property in properties)
            {
                if (property.PropertyType == typeof(string))
                {
                    string stringValue = (string)property.GetValue(obj, null);
                    SaveSetting(property.Name, path, stringValue);
                }
                if (property.PropertyType == typeof(int))
                {
                    int intValue = (int)property.GetValue(obj, null);
                    SaveSetting(property.Name, path, intValue);
                }
                if (property.PropertyType == typeof(bool))
                {
                    bool boolValue = (bool)property.GetValue(obj, null);
                    SaveSetting(property.Name, path, boolValue);
                }
            }
        }


        public static void DeleteKey(string path)
        {
            string fullPath = GetKeyName(path);

            string[] dirs = fullPath.Split('\\');
            RegistryKey key = Registry.CurrentUser;

            for (int i = 0; i < dirs.Length; i++)
            {
                var dir = dirs[i];
                if (i == dirs.Length - 1)
                {
                    key.DeleteSubKeyTree(dir);
                    key.Close();
                }
                else
                {
                    var nextKey = key.OpenSubKey(dir, true);
                    if (nextKey == null)
                    {
                        return;
                    }

                    key.Close();
                    key = nextKey;
                }
            }
        }
    }
}
