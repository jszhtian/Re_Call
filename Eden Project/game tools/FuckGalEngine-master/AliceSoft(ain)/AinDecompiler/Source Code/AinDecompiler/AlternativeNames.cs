using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    public class AlternativeNames : Dictionary<string, string>
    {
        public Dictionary<string, string> Inverse;
        public AlternativeNames()
            : base()
        {

        }
        public AlternativeNames(string fileName)
            : this()
        {
            LoadFile(fileName);
        }

        public bool LoadFile(string fileName)
        {
            string extension = Path.GetExtension(fileName);
            if (extension.Equals(".ain", StringComparison.InvariantCultureIgnoreCase) ||
                extension.Equals(".ain_", StringComparison.InvariantCultureIgnoreCase))
            {
                string directoryName = Path.GetDirectoryName(fileName);
                string baseName = Path.GetFileNameWithoutExtension(fileName);
                fileName = Path.Combine(directoryName, baseName + "_names.txt");
            }
            if (File.Exists(fileName))
            {
                try
                {
                    var lines = File.ReadAllLines(fileName);
                    foreach (var line in lines)
                    {
                        var split = line.Split('\t');
                        if (split.Length > 1)
                        {
                            string a = split[0];
                            string b = split[1];
                            this.Set(a, b);
                        }
                    }
                    return true;
                }
                catch (IOException ex)
                {

                }
                catch (UnauthorizedAccessException ex)
                {

                }
            }
            return false;
        }

        public void GetInverse()
        {
            Inverse.Clear();
            foreach (var pair in this)
            {
                Inverse.Set(pair.Value, pair.Key);
            }
        }

        public string GetAlternativeFunctionDeclaration(IFunction function)
        {
            return GetAlternativeFunctionDeclaration(function, false);
        }

        public string GetAlternativeFunctionDeclaration(IFunction function, bool includeLocalVariables)
        {
            if (function == null)
            {
                return "";
            }
            string functionName = function.GetAlternativeName(); 

            StringBuilder sb = new StringBuilder();
            var ainFile = function.Root;
            var func = function as Function;
            var libf = function as HllFunction;

            if (func != null && func.IsLabel != 0)
            {
                sb.Append("#");
            }

            var split = functionName.Split('@');
            if (split.Length > 1)
            {
                string className = split[0];
                functionName = split[1];

                if (functionName == "0")
                {
                    //constructor
                    sb.Append(className + "::" + className);
                }
                else if (functionName == "1")
                {
                    //destructor
                    sb.Append(className + "::~" + className);
                }
                else
                {
                    if (!(function.DataType == DataType.Void && func != null && func.IsLabel != 0))
                    {
                        sb.Append(GetAlternativeDataTypeName(function) + " ");
                    }
                    if (libf != null)
                    {
                        string libraryName = libf.ParentLibrary.GetAlternativeName();
                        sb.Append(libraryName + ".");
                    }
                    sb.Append(className + "::" + functionName);
                }
            }
            else
            {
                if (!(function.DataType == DataType.Void && func != null && func.IsLabel != 0))
                {
                    sb.Append(GetAlternativeDataTypeName(function) + " ");
                }
                if (libf != null)
                {
                    string libraryName = libf.ParentLibrary.GetAlternativeName();
                    sb.Append(libraryName + ".");
                }
                sb.Append(functionName);
            }

            sb.Append("(");
            bool needComma = false;
            int i;
            for (i = 0; i < function.ParameterCount; i++)
            {
                if (function.Parameters[i].DataType != DataType.Void)
                {
                    Util.PrintComma(sb, ref needComma);
                    var arg = function.Parameters[i];
                    sb.Append(GetAlternativeVariableDeclaration(arg));
                }
            }
            sb.Append(")");
            if (includeLocalVariables)
            {
                needComma = true;
                for (; i < function.Parameters.Count; i++)
                {
                    if (function.Parameters[i].DataType != DataType.Void)
                    {
                        Util.PrintComma(sb, ref needComma);
                        var arg = function.Parameters[i];
                        sb.Append(GetAlternativeVariableDeclaration(arg));
                    }
                }
            }
            return sb.ToString();
        }

        public string GetAlternativeDataTypeName(IVariable variable)
        {
            if (variable == null)
            {
                return "";
            }
            var ainFile = variable.Root;
            string dataTypeName = ExpressionDisplayer.GetDataTypeNameReal(variable);
            if (variable.StructType != -1)
            {
                string structName = ainFile.Structs[variable.StructType].Name;
                string alternativeStructName = ainFile.Structs[variable.StructType].GetAlternativeName();
                dataTypeName = dataTypeName.Replace(structName, alternativeStructName);
            }
            return dataTypeName;
        }

        public string GetAlternativeVariableDeclaration(IVariable variable)
        {
            if (variable == null)
            {
                return "";
            }
            string alternativeDataTypeName = GetAlternativeDataTypeName(variable);
            string alternativeVariableName = variable.GetAlternativeName();
            return alternativeDataTypeName + " " + alternativeVariableName;
        }
    }
}
