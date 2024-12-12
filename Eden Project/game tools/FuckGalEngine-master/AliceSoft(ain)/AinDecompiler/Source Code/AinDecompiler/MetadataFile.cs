using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Xml.Linq;

namespace AinDecompiler
{
    public static class XmlExtensions
    {
        public static bool IsNamed(this XElement element, string localName)
        {
            return element.Name.LocalName == localName;
        }

        public static string GetAttribute(this XElement element, string attributeName)
        {
            var attribute = element.Attribute(XNamespace.None + attributeName);
            if (attribute != null)
            {
                return attribute.Value;
            }
            return null;
        }

        public static void SetAttribute(this XElement element, string attributeName, string value)
        {
            element.SetAttributeValue(XNamespace.None + attributeName, value);
        }
    }

    public class MetadataFile
    {
        AinFile ainFile;
        public Dictionary<IVariable, Metadata> Metadata = new Dictionary<IVariable, Metadata>();
        public Dictionary<string, EnumerationType> EnumerationTypes = new Dictionary<string, EnumerationType>();
        public Dictionary<int, Metadata> GlobalGroupMetadata = new Dictionary<int, Metadata>();

        public MetadataFile(AinFile ainFile)
        {
            this.ainFile = ainFile;
        }

        public void SaveFile(string fileName)
        {
            var document = new XDocument();
            var rootElement = new XElement(XNamespace.None + "root");
            foreach (var pair in Metadata)
            {
                var variable = pair.Key;
                var metaData = pair.Value;

                var element = new XElement(XNamespace.None + "variable");

                var parent = variable.Parent;
                var parentFunction = parent as Function;
                var parentStruct = parent as Struct;
                element.SetAttribute("name", variable.Name);
                if (parentFunction != null)
                {
                    element.SetAttribute("function", parent.Name);
                    element.SetAttribute("name", variable.Index.ToString());
                }
                else if (parentStruct != null)
                {
                    element.SetAttribute("struct", parent.Name);
                }
                WriteMetadata(element, metaData);
                rootElement.Add(element);
            }

            foreach (var pair in this.EnumerationTypes)
            {
                var enumerationType = pair.Value;
                var name = enumerationType.Name;
                var element = new XElement(XNamespace.None + "enumeration");
                element.SetAttribute("name", name);
                foreach (var item in enumerationType)
                {
                    var childElement = new XElement(XNamespace.None + "item");
                    childElement.SetAttribute("name", item.Value);
                    childElement.SetAttribute("value", item.Key.ToString());
                    element.Add(childElement);
                }
                rootElement.Add(element);
            }

            foreach (var pair in this.GlobalGroupMetadata)
            {
                int index = pair.Key;
                var metaData = pair.Value;

                var element = new XElement(XNamespace.None + "globalgroup");
                element.SetAttribute("index", index.ToString());
                WriteMetadata(element, metaData);
                rootElement.Add(element);
            }
            document.Add(rootElement);
            document.Save(fileName);
        }

        public void LoadFile(string fileName)
        {
            var text = File.ReadAllText(fileName);
            var document = XDocument.Parse(text);
            var rootElement = document.Element(XNamespace.None + "root");
            if (rootElement == null)
            {
                return;
            }

            foreach (var element in rootElement.Elements())
            {
                if (element.IsNamed("variable"))
                {
                    var metaData = TryReadMetadata(element);

                    string name = element.GetAttribute("name");
                    if (name != null && metaData != null)
                    {
                        IVariable variable = null;
                        string parentFunctionName = element.GetAttribute("function");
                        string parentStructName = element.GetAttribute("struct");

                        var parentFunction = ainFile.GetFunction(parentFunctionName);
                        var parentStruct = ainFile.GetStruct(parentStructName);
                        var function = ainFile.GetFunction(name);
                        var structInfo = ainFile.GetStruct(name);
                        var global = ainFile.GetGlobal(name);

                        if (parentFunction != null)
                        {
                            int index;
                            if (!int.TryParse(name, out index))
                            {
                                variable = ainFile.GetFunctionParameter(parentFunction, name);
                            }
                            else
                            {
                                variable = ainFile.GetFunctionParameter(parentFunction, index);
                            }
                        }
                        else if (parentStruct != null)
                        {
                            variable = ainFile.GetStructMember(parentStruct, name);
                        }
                        else if (function != null)
                        {
                            variable = function;
                        }
                        else if (structInfo != null)
                        {
                            variable = structInfo;
                        }
                        else if (global != null)
                        {
                            variable = global;
                        }

                        if (variable != null)
                        {
                            Metadata[variable] = metaData;
                        }
                    }
                }
                else if (element.IsNamed("enumeration"))
                {
                    string name = element.GetAttribute("name");
                    if (name != null)
                    {
                        var enumeration = this.EnumerationTypes.GetOrAddNew(name);
                        enumeration.Name = name;
                        foreach (var enumerationElement in element.Elements())
                        {
                            if (enumerationElement.IsNamed("item"))
                            {
                                string itemName = enumerationElement.GetAttribute("name");
                                string itemValueString = enumerationElement.GetAttribute("value");
                                if (itemName != null && itemValueString != null)
                                {
                                    int itemValue;
                                    if (int.TryParse(itemValueString, out itemValue))
                                    {
                                        enumeration.Add(itemValue, itemName);
                                    }
                                }
                            }
                        }
                    }
                }
                else if (element.IsNamed("globalgroup"))
                {
                    string indexString = element.GetAttribute("index");
                    int index;
                    if (IntUtil.TryParse(indexString, out index))
                    {
                        var metaData = TryReadMetadata(element);
                        if (metaData != null)
                        {
                            this.GlobalGroupMetadata.Set(index, metaData);
                        }
                    }
                }
            }
        }

        private void WriteMetadata(XElement parentElement, Metadata metaData)
        {
            if (metaData.ReplacementName != null)
            {
                var element = new XElement(XNamespace.None + "ReplacementName");
                element.Add(new XText(metaData.ReplacementName));
                parentElement.Add(element);
            }
            if (metaData.Description != null)
            {
                var element = new XElement(XNamespace.None + "Description");
                element.Add(new XText(metaData.Description));
                parentElement.Add(element);
            }
            if (metaData.EnumerationType != null)
            {
                var element = new XElement(XNamespace.None + "EnumerationType");
                element.Add(new XText(metaData.EnumerationType.Name));
                parentElement.Add(element);
            }
            if (metaData.EnumerationTypeForArrayIndex != null)
            {
                var element = new XElement(XNamespace.None + "EnumerationTypeForArrayIndex");
                element.Add(new XText(metaData.EnumerationTypeForArrayIndex.Name));
                parentElement.Add(element);
            }
            if (metaData.DefaultValue != null)
            {
                var element = new XElement(XNamespace.None + "DefaultValue");
                var defaultValue = metaData.DefaultValue;
                var dataType = defaultValue.DataType;
                string value;
                switch (dataType)
                {
                    default:
                    case DataType.Int:
                    case DataType.RefInt:
                    case DataType.Lint:
                    case DataType.RefLint:
                        value = defaultValue.IntValue.ToString();
                        element.SetAttribute("DataType", "int");
                        break;
                    case DataType.Float:
                    case DataType.RefFloat:
                        value = defaultValue.FloatValue.FloatToString();
                        element.SetAttribute("DataType", "float");
                        break;
                    case DataType.String:
                    case DataType.RefString:
                        value = defaultValue.StringValue ?? "";
                        element.SetAttribute("DataType", "string");
                        break;
                    case DataType.Bool:
                    case DataType.RefBool:
                        value = (defaultValue.IntValue != 0) ? "true" : "false";
                        element.SetAttribute("DataType", "bool");
                        break;
                }
                element.Add(new XText(value));
                parentElement.Add(element);
            }
        }

        private Metadata TryReadMetadata(XElement parentElement)
        {
            Metadata metaData = null;
            foreach (var element in parentElement.Elements())
            {
                if (element.IsNamed("ReplacementName"))
                {
                    if (metaData == null) metaData = new Metadata();
                    metaData.ReplacementName = element.Value;
                }
                else if (element.IsNamed("Description"))
                {
                    if (metaData == null) metaData = new Metadata();
                    metaData.Description = element.Value;
                }
                else if (element.IsNamed("EnumerationType"))
                {
                    if (metaData == null) metaData = new Metadata();
                    string enumerationTypeName = element.Value;
                    var enumerationType = EnumerationTypes.GetOrAddNew(enumerationTypeName);
                    metaData.EnumerationType = enumerationType;
                }
                else if (element.IsNamed("EnumerationTypeForArrayIndex"))
                {
                    if (metaData == null) metaData = new Metadata();
                    string enumerationTypeName = element.Value;
                    var enumerationType = EnumerationTypes.GetOrAddNew(enumerationTypeName);
                    metaData.EnumerationTypeForArrayIndex = enumerationType;
                }
                else if (element.IsNamed("DefaultValue"))
                {
                    if (metaData == null) metaData = new Metadata();
                    string defaultValueString = element.Value;
                    string dataType = element.GetAttribute("DataType");
                    var defaultValue = new InitialValue();
                    switch (dataType)
                    {
                        default:
                        case "int":
                            defaultValue.DataType = DataType.Int;
                            int defaultValueInt = 0;
                            int.TryParse(defaultValueString, out defaultValueInt);
                            defaultValue.IntValue = defaultValueInt;
                            break;
                        case "float":
                            defaultValue.DataType = DataType.Float;
                            double defaultValueDouble = 0;
                            double.TryParse(defaultValueString, out defaultValueDouble);
                            defaultValue.FloatValue = (float)defaultValueDouble;
                            break;
                        case "string":
                            defaultValue.DataType = DataType.String;
                            defaultValue.StringValue = defaultValueString;
                            break;
                        case "bool":
                            bool defaultValueBool = false;
                            bool.TryParse(defaultValueString, out defaultValueBool);
                            defaultValue.DataType = DataType.Bool;
                            defaultValue.IntValue = defaultValueBool ? 1 : 0;
                            break;
                    }
                    metaData.DefaultValue = defaultValue;
                }
            }
            return metaData;
        }

        public MetadataFile Clone(AinFile ainFile)
        {
            var clone = new MetadataFile(ainFile);
            foreach (var pair in this.EnumerationTypes)
            {
                clone.EnumerationTypes.Add(pair.Key, pair.Value.Clone());
            }

            foreach (var pair in this.Metadata)
            {
                var variable = pair.Key.Canonicalize(ainFile);
                var metaData = pair.Value.Clone();
                clone.Metadata.Add(variable, metaData);
            }
            return clone;
        }
    }

}
