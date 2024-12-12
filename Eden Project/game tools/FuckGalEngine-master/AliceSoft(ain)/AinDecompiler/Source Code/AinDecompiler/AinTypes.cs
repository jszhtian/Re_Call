using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.ComponentModel;
using System.Collections.ObjectModel;
using System.Globalization;

namespace AinDecompiler
{
    public class ListWrapper<T1, T2> : IList<T2> where T1 : class, T2
    {
        public ListWrapper(IList<T1> list)
        {
            this.list = list;
        }

        private IList<T1> list;

        #region IList<T2> Members

        public int IndexOf(T2 item)
        {
            var variableItem = item as T1;
            if (variableItem == null)
            {
                return -1;
            }
            return list.IndexOf(variableItem);
        }

        public void Insert(int index, T2 item)
        {
            var variableItem = item as T1;
            if (variableItem == null)
            {
                throw new ArgumentOutOfRangeException("item");
            }
            else
            {
                list.Insert(index, variableItem);
            }
        }

        public void RemoveAt(int index)
        {
            list.RemoveAt(index);
        }

        public T2 this[int index]
        {
            get
            {
                return list[index];
            }
            set
            {
                var variableItem = value as T1;
                if (variableItem == null)
                {
                    throw new ArgumentOutOfRangeException();
                }
                list[index] = variableItem;
            }
        }

        #endregion

        #region ICollection<T2> Members

        public void Add(T2 item)
        {
            var variableItem = item as T1;
            if (variableItem == null)
            {
                throw new ArgumentOutOfRangeException();
            }
            list.Add(variableItem);
        }

        public void Clear()
        {
            list.Clear();
        }

        public bool Contains(T2 item)
        {
            var variableItem = item as T1;
            if (variableItem == null)
            {
                return false;
            }
            return list.Contains(variableItem);
        }

        public void CopyTo(T2[] array, int arrayIndex)
        {
            for (int i = 0; i < list.Count; i++)
            {
                array[i + arrayIndex] = list[i];
            }
        }

        public int Count
        {
            get
            {
                return list.Count;
            }
        }

        public bool IsReadOnly
        {
            get
            {
                return list.IsReadOnly;
            }
        }

        public bool Remove(T2 item)
        {
            var variableItem = item as T1;
            if (variableItem == null)
            {
                return false;
            }
            return list.Remove(variableItem);
        }

        #endregion

        #region IEnumerable<T2> Members

        public IEnumerator<T2> GetEnumerator()
        {
            return list.Cast<T2>().GetEnumerator();
        }

        #endregion

        #region IEnumerable Members

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return this.GetEnumerator();
        }

        #endregion
    }

    public class MyCollection<T> : Collection<T>
    {
        public MyCollection()
            : base()
        {

        }
        public MyCollection(IEnumerable<T> sequence)
            : this()
        {
            AddRange(sequence);
        }
        public void AddRange(IEnumerable<T> sequence)
        {
            foreach (var item in sequence)
            {
                this.Add(item);
            }
        }
    }


    public class VariableCollection : MyCollection<Variable>
    {
        public VariableCollection() : base() { }
        public VariableCollection(IEnumerable<Variable> sequence) : base(sequence) { }
    }
    public class FunctionCollection : MyCollection<Function>
    {
        public FunctionCollection() : base() { }
        public FunctionCollection(IEnumerable<Function> sequence) : base(sequence) { }
    }
    public class FuncTypeCollection : MyCollection<FunctionType>
    {
        public FuncTypeCollection() : base() { }
        public FuncTypeCollection(IEnumerable<FunctionType> sequence) : base(sequence) { }
    }
    public class GlobalCollection : MyCollection<Global>
    {
        public GlobalCollection() : base() { }
        public GlobalCollection(IEnumerable<Global> sequence) : base(sequence) { }
    }
    public class HllLibraryCollection : MyCollection<HllLibrary>
    {
        public HllLibraryCollection() : base() { }
        public HllLibraryCollection(IEnumerable<HllLibrary> sequence) : base(sequence) { }
    }
    public class HllFunctionCollection : MyCollection<HllFunction>
    {
        public HllFunctionCollection() : base() { }
        public HllFunctionCollection(IEnumerable<HllFunction> sequence) : base(sequence) { }
    }
    public class HllParameterCollection : MyCollection<HllFunctionParameter>
    {
        public HllParameterCollection() : base() { }
        public HllParameterCollection(IEnumerable<HllFunctionParameter> sequence) : base(sequence) { }
    }
    public class StringCollection : List<string>
    {
        public StringCollection() : base() { }
        public StringCollection(IEnumerable<string> sequence) : base(sequence) { }
    }
    public class GlobalInitialValueCollection : MyCollection<GlobalInitialValue>
    {
        public GlobalInitialValueCollection() : base() { }
        public GlobalInitialValueCollection(IEnumerable<GlobalInitialValue> sequence) : base(sequence) { }
    }
    public class StructCollection : MyCollection<Struct>
    {
        public StructCollection() : base() { }
        public StructCollection(IEnumerable<Struct> sequence) : base(sequence) { }
    }
    public class SwitchBlockCollection : MyCollection<SwitchBlock>
    {
        public SwitchBlockCollection() : base() { }
        public SwitchBlockCollection(IEnumerable<SwitchBlock> sequence) : base(sequence) { }
    }



    /// <summary>
    /// Contains the functions for reading arrays of objects
    /// </summary>
    public static partial class Util
    {
        /// <summary>
        /// Reads an array of objects that implement a Read() method.
        /// </summary>
        /// <typeparam name="T">A type that implements Read()</typeparam>
        /// <param name="br">The binary reader to read from</param>
        /// <param name="count">The number of objects to read</param>
        /// <returns>An array of objects</returns>
        public static T[] Read<T>(BinaryReader br, int count) where T : IReadable, new()
        {
            bool hasIndex = typeof(IWithIndex).IsAssignableFrom(typeof(T));
            List<T> list = new List<T>();
            for (int i = 0; i < count; i++)
            {
                var obj = new T();
                obj.Read(br);
                list.Add(obj);
            }
            return list.ToArray();
        }

        /// <summary>
        /// Reads an array of objects that implement a Read() method, and require the Version parameter.
        /// </summary>
        /// <typeparam name="T">A type that implements Read()</typeparam>
        /// <param name="br">The binary reader to read from</param>
        /// <param name="count">The number of objects to read</param>
        /// <param name="version">The version number of the AIN file</param>
        /// <returns>An array of objects</returns>
        public static T[] Read<T>(BinaryReader br, int count, int version) where T : IReadable2, new()
        {
            List<T> list = new List<T>();
            for (int i = 0; i < count; i++)
            {
                var obj = new T();
                obj.Read(br, version);
                list.Add(obj);
            }
            return list.ToArray();
        }

        /// <summary>
        /// Reads an array of null-terminated strings.
        /// </summary>
        /// <param name="br">The binary reader to read from</param>
        /// <param name="count">The number of strings to read</param>
        /// <returns>An array of strings</returns>
        public static string[] ReadStrings(BinaryReader br, int count)
        {
            List<string> list = new List<string>();
            for (int i = 0; i < count; i++)
            {
                var str = br.ReadStringNullTerminated();
                list.Add(str);
            }
            return list.ToArray();
        }

        /// <summary>
        /// Writes an array of null-terminated strings.
        /// </summary>
        /// <param name="bw">The binary writer to write to.</param>
        /// <param name="strings">The array of strings to write</param>
        public static void WriteStrings(BinaryWriter bw, IList<string> strings)
        {
            for (int i = 0; i < strings.Count; i++)
            {
                string str = strings[i];
                bw.WriteStringNullTerminated(str);
            }
        }

        /// <summary>
        /// Writes an array of objects that implement a Write() method
        /// </summary>
        /// <typeparam name="T">The type that implements a Write() method</typeparam>
        /// <param name="bw">The binary writer to write to</param>
        /// <param name="list">The list of objects to write</param>
        public static void Write<T>(BinaryWriter bw, IList<T> list) where T : IReadable
        {
            for (int i = 0; i < list.Count; i++)
            {
                var obj = list[i];
                obj.Write(bw);
            }
        }
        /// <summary>
        /// Writes an array of objects that implement a Write() method, and require the Version parameter.
        /// </summary>
        /// <typeparam name="T">The type that implements a Write() method</typeparam>
        /// <param name="bw">The binary writer to write to</param>
        /// <param name="list">The list of objects to write</param>
        /// <param name="version">The version of the AIN file</param>
        public static void Write<T>(BinaryWriter bw, IList<T> list, int version) where T : IReadable2
        {
            for (int i = 0; i < list.Count; i++)
            {
                var obj = list[i];
                obj.Write(bw, version);
            }
        }

        /// <summary>
        /// Prints a comma and space if needed, otherwise changes the bool to indicate that a comma is needed.
        /// </summary>
        /// <param name="stringBuilder">The string builder to write to</param>
        /// <param name="needComma">The bool indicating that a comma is needed</param>
        public static void PrintComma(StringBuilder stringBuilder, ref bool needComma)
        {
            if (needComma)
            {
                stringBuilder.Append(", ");
            }
            else
            {
                needComma = true;
            }
        }

        /// <summary>
        /// Prints a comma and space if needed, otherwise changes the bool to indicate that a comma is needed.
        /// </summary>
        /// <param name="stringBuilder">The text writer to write to</param>
        /// <param name="needComma">The bool indicating that a comma is needed</param>
        public static void PrintComma(TextWriter textWriter, ref bool needComma)
        {
            if (needComma)
            {
                textWriter.Write(", ");
            }
            else
            {
                needComma = true;
            }
        }
    }

    /// <summary>
    /// A variable (Function parameter, local variable, or struct member)
    /// </summary>
    [TypeConverter(typeof(MyTypeConverter))]
    public class Variable : IReadable, IVariable, IWithRoot<AinFile>
    {
        /// <summary>
        /// The name of the variable
        /// </summary>
        public string Name;
        /// <summary>
        /// The data type of the variable.  For a struct type, use StructType to determine which type it is.
        /// </summary>
        public DataType DataType;
        /// <summary>
        /// The struct type of the variabile.  -1 for variable types which are not structs, 0 for struct members of type void.
        /// </summary>
        public int StructType = -1;
        /// <summary>
        /// The number of dimensions of the array.  0 for variables which are not arrays.
        /// </summary>
        public int ArrayDimensions = 0;

        /// <summary>
        /// The array index of the variable
        /// </summary>
        [Browsable(false)]
        public int Index = -1;
        /// <summary>
        /// The function or struct that contains this variable
        /// </summary>
        [Browsable(false)]
        public IFunction Parent;
        /// <summary>
        /// The AIN file
        /// </summary>
        [Browsable(false)]
        public AinFile Root;



        /// <summary>
        /// Returns a copy of the variable.
        /// </summary>
        /// <returns>A copy of the variable.</returns>
        public Variable Clone()
        {
            var clone = (Variable)this.MemberwiseClone();
            clone.Parent = null;
            clone.Root = null;
            return clone;
        }

        /// <summary>
        /// Reads the variaible from a binary reader
        /// </summary>
        /// <param name="br">The binary reader to read from</param>
        public void Read(BinaryReader br)
        {
            Name = br.ReadStringNullTerminated();
            DataType = (DataType)br.ReadInt32();
            StructType = br.ReadInt32();
            ArrayDimensions = br.ReadInt32();
        }

        /// <summary>
        /// Writes the variaible to a binary writer
        /// </summary>
        /// <param name="bw">The binary writer to write to</param>
        public void Write(BinaryWriter bw)
        {
            bw.WriteStringNullTerminated(Name);
            bw.Write((int)DataType);
            //int structType = this.StructType;
            //if (structType != -1 && this.DataType.IsFunction())
            //{
            //    structType = -1;
            //}
            bw.Write(StructType);
            bw.Write(ArrayDimensions);
        }

        /// <summary>
        /// Returns the declaration string of a variable (typeName variableName)
        /// </summary>
        /// <returns>The declration string of a variable</returns>
        public override string ToString()
        {
            if (Root == null)
            {
                return ReflectionUtil.ToString(this);
            }
            return ExpressionDisplayer.GetDataTypeName(this) + " " + this.GetActiveName();
        }

        /// <summary>
        /// Creates an array of variables, used when creating system calls.
        /// </summary>
        /// <param name="typesAndNames">The data type of a parameter (DataType), followed by the name (string), repeating for each parameter</param>
        /// <returns>An array of new variables</returns>
        public static Variable[] Create(params object[] typesAndNames)
        {
            List<Variable> list = new List<Variable>();
            for (int i = 0; i < typesAndNames.Length; i += 2)
            {
                Variable newVariable = new Variable();
                DataType dataType = (DataType)typesAndNames[i];
                string name = (string)typesAndNames[i + 1];
                int arrayDimenions = 0;

                if (dataType.IsArray())
                {
                    arrayDimenions = 1;
                }
                newVariable.ArrayDimensions = arrayDimenions;
                newVariable.StructType = -1;
                newVariable.Name = name;
                newVariable.DataType = dataType;
                list.Add(newVariable);
            }
            return list.ToArray();
        }

        /// <summary>
        /// Creates a variable
        /// </summary>
        public Variable()
        {

        }

        /// <summary>
        /// Creates a variable.
        /// </summary>
        /// <param name="name">Name of the variable</param>
        /// <param name="dataType">Data type of the variable</param>
        /// <param name="structType">Struct type of the variable, or -1</param>
        /// <param name="arrayDimensions">Number of dimensions of an array, or 0</param>
        public Variable(string name, DataType dataType, int structType, int arrayDimensions)
        {
            this.Name = name;
            this.DataType = dataType;
            this.StructType = structType;
            this.ArrayDimensions = arrayDimensions;
        }

        #region IVariable Members

        string IVariable.Name
        {
            get
            {
                return this.Name;
            }
            set
            {
                this.Name = value;
            }
        }

        DataType IVariable.DataType
        {
            get
            {
                return this.DataType;
            }
            set
            {
                this.DataType = value;
            }
        }

        int IVariable.StructType
        {
            get
            {
                return this.StructType;
            }
            set
            {
                this.StructType = value;
            }
        }

        int IVariable.ArrayDimensions
        {
            get
            {
                return this.ArrayDimensions;
            }
            set
            {
                this.ArrayDimensions = value;
            }
        }

        int IVariable.GroupIndex
        {
            get
            {
                return -1;
            }
            set
            {

            }
        }

        #endregion

        #region IWithIndex Members

        int IWithIndex.Index
        {
            get
            {
                return this.Index;
            }
            set
            {
                this.Index = value;
            }
        }

        #endregion

        #region IWithParent Members

        object IWithParent.Parent
        {
            get
            {
                return this.Parent;
            }
            set
            {
                this.Parent = (IFunction)value;
            }
        }

        #endregion

        #region IWithParent<IFunction> Members

        IFunction IWithParent<IFunction>.Parent
        {
            get
            {
                return this.Parent;
            }
            set
            {
                this.Parent = value;
            }
        }

        #endregion

        #region IWithRoot<AinFile> Members

        AinFile IWithRoot<AinFile>.Root
        {
            get
            {
                return this.Root;
            }
            set
            {
                this.Root = value;
            }
        }

        #endregion
    }

    /// <summary>
    /// A switch block (conditionals)
    /// </summary>
    [TypeConverter(typeof(MyTypeConverter))]
    public class SwitchBlock : IReadable, IWithIndex, IWithRoot<AinFile>
    {
        /// <summary>
        /// Value of 2 if the cases are integers, value of 4 if the cases are strings.
        /// </summary>
        public int CaseType2or4 = 2;
        /// <summary>
        /// The address to jump to for a default switch case, or -1 for no default.
        /// </summary>
        public int DefaultCaseAddress = -1; //, CaseCount;
        /// <summary>
        /// The list of switch cases for this switch block
        /// </summary>
        public List<SwitchCase> SwitchCases = new List<SwitchCase>();
        /// <summary>
        /// The array index
        /// </summary>
        [Browsable(false)]
        public int Index = -1;
        /// <summary>
        /// The AIN file
        /// </summary>
        [Browsable(false)]
        public AinFile Root = null;

        /// <summary>
        /// Reads this switch block from a binary reader.
        /// </summary>
        /// <param name="br">The binary reader to read from</param>
        public void Read(BinaryReader br)
        {
            CaseType2or4 = br.ReadInt32();
            if (CaseType2or4 != 2 && CaseType2or4 != 4)
            {
                //put a breakpoint here for an unexpected switch case type
            }
            DefaultCaseAddress = br.ReadInt32();
            int caseCount = br.ReadInt32();
            SwitchCases.Clear();
            SwitchCases.AddRange(Util.Read<SwitchCase>(br, caseCount));
        }

        /// <summary>
        /// Writes this switch block to a binary writer.
        /// </summary>
        /// <param name="bw">The binary writer to write to</param>
        public void Write(BinaryWriter bw)
        {
            bw.Write(CaseType2or4);
            bw.Write(DefaultCaseAddress);
            bw.Write(SwitchCases.Count);
            Util.Write(bw, SwitchCases);
        }

        /// <summary>
        /// Returns a deep copy of this switch block
        /// </summary>
        /// <returns>A copy of this switch block</returns>
        public SwitchBlock Clone()
        {
            var clone = (SwitchBlock)this.MemberwiseClone();
            clone.Root = null;
            clone.SwitchCases = new List<SwitchCase>();
            clone.SwitchCases.Clear();
            clone.SwitchCases.AddRange(this.SwitchCases.Select(s => s.Clone()));
            return clone;
        }

        /// <summary>
        /// Returns a string representation of this switch block.
        /// </summary>
        /// <returns>A string representation of this switch block</returns>
        public override string ToString()
        {
            return ReflectionUtil.ToString(this);
        }

        #region IReadable Members

        int IWithIndex.Index
        {
            get
            {
                return this.Index;
            }
            set
            {
                this.Index = value;
            }
        }

        #endregion

        #region IWithRoot<AinFile> Members

        AinFile IWithRoot<AinFile>.Root
        {
            get
            {
                return this.Root;
            }
            set
            {
                this.Root = value;
            }
        }

        #endregion
    }

    /// <summary>
    /// A case item for a switch block
    /// </summary>
    [TypeConverter(typeof(MyTypeConverter))]
    public class SwitchCase : IReadable, IWithIndex, IWithParent<SwitchBlock>, IWithRoot<AinFile>
    {
        /// <summary>
        /// The value to compare against for this switch case.  For string cases, this is the index number of the string.
        /// </summary>
        public int Value;
        /// <summary>
        /// The address to jump to for this switch case.
        /// </summary>
        public int TargetAddress;
        /// <summary>
        /// The array index
        /// </summary>
        [Browsable(false)]
        public int Index = -1;
        /// <summary>
        /// The switch block that contains this item
        /// </summary>
        [Browsable(false)]
        public SwitchBlock Parent;
        /// <summary>
        /// The AIN file
        /// </summary>
        [Browsable(false)]
        public AinFile Root;

        /// <summary>
        /// Reads this switch case from a binary reader
        /// </summary>
        /// <param name="br">The binary reader to read from</param>
        public void Read(BinaryReader br)
        {
            Value = br.ReadInt32();
            TargetAddress = br.ReadInt32();
        }

        /// <summary>
        /// Writes this switch case to a binary writer
        /// </summary>
        /// <param name="bw">The binary writer to write to</param>
        public void Write(BinaryWriter bw)
        {
            bw.Write(Value);
            bw.Write(TargetAddress);
        }

        /// <summary>
        /// Returns a string representation of this switch case.
        /// </summary>
        /// <returns>A string representation of this switch case</returns>
        public override string ToString()
        {
            return ToString("0x" + this.TargetAddress.ToString("X"));
        }

        /// <summary>
        /// Returns a string representation of this object, given a label name.
        /// </summary>
        /// <param name="labelName">The text to use as the case destination address.</param>
        /// <returns>A string representation of this switch case</returns>
        public string ToString(string labelName)
        {
            if (Parent == null || Root == null)
            {
                return ReflectionUtil.ToString(this);
            }
            if (Parent.CaseType2or4 == 2)
            {
                return "case " + this.Value.ToString(CultureInfo.InvariantCulture) + ": " + labelName;
            }
            else
            {
                string stringValue;
                if (this.Value >= 0 && this.Value < Root.Strings.Count)
                {
                    stringValue = "\"" + AssemblerProjectWriter.EscapeString(Root.Strings[this.Value]) + "\"";
                }
                else
                {
                    stringValue = "0x" + this.Value.ToString("X");
                }
                return "case " + stringValue + ": " + labelName;
            }
        }

        /// <summary>
        /// Returns a copy of this switch case.
        /// </summary>
        /// <returns>A copy of this switch case</returns>
        public SwitchCase Clone()
        {
            var clone = (SwitchCase)this.MemberwiseClone();
            clone.Root = null;
            clone.Parent = null;
            return clone;
        }

        public string StringValue
        {
            get
            {
                if (Root != null && Parent != null && Parent.CaseType2or4 == 4)
                {
                    return Root.GetString(Value);
                }
                else
                {
                    return Value.ToString();
                }
            }
        }


        #region IReadable Members

        int IWithIndex.Index
        {
            get
            {
                return this.Index;
            }
            set
            {
                this.Index = value;
            }
        }

        #endregion

        #region IWithParent Members

        object IWithParent.Parent
        {
            get
            {
                return Parent;
            }
            set
            {
                this.Parent = (SwitchBlock)value;
            }
        }

        #endregion

        #region IWithParent<Switch> Members

        SwitchBlock IWithParent<SwitchBlock>.Parent
        {
            get
            {
                return Parent;
            }
            set
            {
                this.Parent = value;
            }
        }

        #endregion

        #region IWithRoot<AinFile> Members

        AinFile IWithRoot<AinFile>.Root
        {
            get
            {
                return this.Root;
            }
            set
            {
                this.Root = value;
            }
        }

        #endregion
    }


    /// <summary>
    /// A library that contains functions to call, called an "hll", but corresponds to a DLL file.
    /// </summary>
    [TypeConverter(typeof(MyTypeConverter))]
    public class HllLibrary : IReadable, IFunction, IWithRoot<AinFile>
    {
        public HllLibrary()
        {
            this._functions = new HllFunctionCollection();
            this._wrapper = new ListWrapper<HllFunction, IVariable>(_functions);

        }

        /// <summary>
        /// The name of the library, matches the base filename of the DLL
        /// </summary>
        public string LibraryName;
        /// <summary>
        /// The functions of the library
        /// </summary>
        public HllFunctionCollection Functions
        {
            get
            {
                return _functions;
            }
            set
            {
                _functions.Clear();
                _functions.AddRange(value);
            }
        }

        private HllFunctionCollection _functions;
        ListWrapper<HllFunction, IVariable> _wrapper;

        /// <summary>
        /// The array index
        /// </summary>
        [Browsable(false)]
        public int Index;
        /// <summary>
        /// The AIN file
        /// </summary>
        [Browsable(false)]
        public AinFile Root;

        /// <summary>
        /// Reads this from a binary reader
        /// </summary>
        /// <param name="br">The binary reader to read from</param>
        public void Read(BinaryReader br)
        {
            LibraryName = br.ReadStringNullTerminated();
            int numberOfFunctions = br.ReadInt32();
            Functions.Clear();
            Functions.AddRange(Util.Read<HllFunction>(br, numberOfFunctions));
        }

        /// <summary>
        /// Writes this to a binary writer
        /// </summary>
        /// <param name="bw">The binary writer to write to</param>
        public void Write(BinaryWriter bw)
        {
            bw.WriteStringNullTerminated(LibraryName);
            bw.Write(Functions.Count);
            Util.Write(bw, Functions);
        }

        #region IReadable Members

        int IWithIndex.Index
        {
            get
            {
                return this.Index;
            }
            set
            {
                this.Index = value;
            }
        }

        #endregion
        public override string ToString()
        {
            return ReflectionUtil.ToString(this);
        }

        /// <summary>
        /// Returns a deep copy of this object.
        /// </summary>
        /// <returns>A copy of this object</returns>
        public HllLibrary Clone()
        {
            var clone = (HllLibrary)this.MemberwiseClone();
            clone._functions = new HllFunctionCollection();
            clone._wrapper = new ListWrapper<HllFunction, IVariable>(clone._functions);
            clone.Functions.Clear();
            clone.Functions.AddRange(this.Functions.Select(f => f.Clone()));
            clone.Root = null;
            return clone;
        }

        #region IFunction Members

        string IFunction.Name
        {
            get
            {
                return this.LibraryName;
            }
            set
            {
                this.LibraryName = value;
            }
        }

        int IFunction.ParameterCount
        {
            get
            {
                return this.Functions.Count;
            }
            set
            {

            }
        }

        IList<IVariable> IFunction.Parameters
        {
            get
            {
                return this._wrapper;
            }
            set
            {
                this.Functions.Clear();
                this.Functions.AddRange(value.Cast<HllFunction>());
            }
        }

        #endregion

        #region IVariable Members

        string IVariable.Name
        {
            get
            {
                return this.LibraryName;
            }
            set
            {
                this.LibraryName = value;
            }
        }

        DataType IVariable.DataType
        {
            get
            {
                return DataType.Void;
            }
            set
            {

            }
        }

        int IVariable.StructType
        {
            get
            {
                return -1;
            }
            set
            {

            }
        }

        int IVariable.ArrayDimensions
        {
            get
            {
                return 0;
            }
            set
            {

            }
        }

        int IVariable.GroupIndex
        {
            get
            {
                return -1;
            }
            set
            {

            }
        }

        #endregion

        #region IWithParent<IFunction> Members

        IFunction IWithParent<IFunction>.Parent
        {
            get
            {
                return null;
            }
            set
            {

            }
        }

        #endregion

        #region IWithParent Members

        object IWithParent.Parent
        {
            get
            {
                return null;
            }
            set
            {

            }
        }

        #endregion

        #region IWithRoot<AinFile> Members

        AinFile IWithRoot<AinFile>.Root
        {
            get
            {
                return this.Root;
            }
            set
            {
                this.Root = value;
            }
        }

        #endregion
    }

    /// <summary>
    /// A library function from a DLL file.
    /// </summary>
    [TypeConverter(typeof(MyTypeConverter))]
    public class HllFunction : IReadable, IVariable, IFunction, IWithRoot<AinFile>
    {
        public HllFunction()
        {
            this._parameters = new HllParameterCollection();
            this._wrapper = new ListWrapper<HllFunctionParameter, IVariable>(this._parameters);
        }

        /// <summary>
        /// The name of the function
        /// </summary>
        public string Name;
        /// <summary>
        /// The return type of the function (cannot be a struct type or array)
        /// </summary>
        public DataType DataType;
        /// <summary>
        /// The number of parameters for the function
        /// </summary>
        public int ParameterCount;
        /// <summary>
        /// The library that contains this function
        /// </summary>
        [Browsable(false)]
        public HllLibrary ParentLibrary;
        /// <summary>
        /// The array index
        /// </summary>
        [Browsable(false)]
        public int Index = -1;
        /// <summary>
        /// The AIN file
        /// </summary>
        [Browsable(false)]
        public AinFile Root;
        /// <summary>
        /// The full name of the library function "libraryname.functionname"
        /// </summary>
        public string FullName
        {
            get
            {
                if (ParentLibrary != null)
                {
                    return ParentLibrary.LibraryName + "." + Name;
                }
                else
                {
                    return Name;
                }
            }
        }

        public HllParameterCollection Parameters
        {
            get
            {
                return _parameters;
            }
            set
            {
                _parameters.Clear();
                _parameters.AddRange(value);
            }
        }
        HllParameterCollection _parameters;
        ListWrapper<HllFunctionParameter, IVariable> _wrapper;

        public override string ToString()
        {
            if (this.Root == null)
            {
                return ReflectionUtil.ToString(this);
            }
            return ExpressionDisplayer.GetFunctionDeclaration(this, true);
        }

        public HllFunction Clone()
        {
            var clone = (HllFunction)this.MemberwiseClone();
            clone._parameters = new HllParameterCollection();
            clone._wrapper = new ListWrapper<HllFunctionParameter, IVariable>(_parameters);
            clone.ParentLibrary = null;
            clone.Root = null;
            clone.Parameters.Clear();
            clone.Parameters.AddRange(this.Parameters.Select(a => a.Clone()));
            return clone;
        }

        #region IReadable Members

        public void Read(BinaryReader br)
        {
            Name = br.ReadStringNullTerminated();
            DataType = (DataType)br.ReadInt32();
            ParameterCount = br.ReadInt32();
            Parameters.Clear();
            Parameters.AddRange(Util.Read<HllFunctionParameter>(br, ParameterCount));
        }

        public void Write(BinaryWriter bw)
        {
            bw.WriteStringNullTerminated(Name);
            bw.Write((int)DataType);
            bw.Write(Parameters.Count);
            Util.Write(bw, Parameters);
        }
        #endregion

        #region IWithIndex members

        int IWithIndex.Index
        {
            get
            {
                return this.Index;
            }
            set
            {
                this.Index = value;
            }
        }

        #endregion

        #region IVariable Members

        string IVariable.Name
        {
            get
            {
                return this.Name;
            }
            set
            {
                this.Name = value;
            }
        }

        DataType IVariable.DataType
        {
            get
            {
                return this.DataType;
            }
            set
            {
                this.DataType = value;
            }
        }

        int IVariable.StructType
        {
            get
            {
                return -1;
            }
            set
            {

            }
        }

        int IVariable.ArrayDimensions
        {
            get
            {
                return 0;
            }
            set
            {

            }
        }

        int IVariable.GroupIndex
        {
            get
            {
                return -1;
            }
            set
            {

            }
        }

        #endregion

        #region IFunction Members

        string IFunction.Name
        {
            get
            {
                return this.Name;
            }
            set
            {
                this.Name = value;
            }
        }

        int IFunction.ParameterCount
        {
            get
            {
                return this.ParameterCount;
            }
            set
            {
                this.ParameterCount = value;
            }
        }

        IList<IVariable> IFunction.Parameters
        {
            get
            {
                return this._wrapper;
            }
            set
            {
                this.Parameters.Clear();
                this.Parameters.AddRange(value.Cast<HllFunctionParameter>());
            }
        }

        #endregion

        #region IWithParent<IFunction> Members

        IFunction IWithParent<IFunction>.Parent
        {
            get
            {
                return this.ParentLibrary;
            }
            set
            {
                this.ParentLibrary = (HllLibrary)value;
            }
        }

        #endregion

        #region IWithParent Members

        object IWithParent.Parent
        {
            get
            {
                return this.ParentLibrary;
            }
            set
            {
                this.ParentLibrary = (HllLibrary)value;
            }
        }

        #endregion

        #region IWithRoot<AinFile> Members

        AinFile IWithRoot<AinFile>.Root
        {
            get
            {
                return this.Root;
            }
            set
            {
                this.Root = value;
            }
        }

        #endregion

    }

    /// <summary>
    /// A parameter for a HLL function
    /// </summary>
    [TypeConverter(typeof(MyTypeConverter))]
    public class HllFunctionParameter : IReadable, IVariable, IWithRoot<AinFile>
    {
        /// <summary>
        /// The name of the parameter
        /// </summary>
        public string Name;
        /// <summary>
        /// The data type of the parameter (cannot be a struct type or array)
        /// </summary>
        public DataType DataType;
        /// <summary>
        /// The array index
        /// </summary>
        [Browsable(false)]
        public int Index = -1;
        /// <summary>
        /// The function that contains this argumnet
        /// </summary>
        [Browsable(false)]
        public IFunction Parent;
        /// <summary>
        /// The AIN file
        /// </summary>
        [Browsable(false)]
        public AinFile Root;

        public override string ToString()
        {
            if (Root == null)
            {
                return ReflectionUtil.ToString(this);
            }
            return ExpressionDisplayer.GetDataTypeName(this) + " " + this.GetActiveName();
        }

        public void Read(BinaryReader br)
        {
            Name = br.ReadStringNullTerminated();
            DataType = (DataType)br.ReadInt32();
        }

        public void Write(BinaryWriter bw)
        {
            bw.WriteStringNullTerminated(Name);
            bw.Write((int)DataType);
        }

        public HllFunctionParameter Clone()
        {
            var clone = (HllFunctionParameter)this.MemberwiseClone();
            clone.Parent = null;
            clone.Root = null;
            return clone;
        }

        #region IReadable Members

        int IWithIndex.Index
        {
            get
            {
                return this.Index;
            }
            set
            {
                this.Index = value;
            }
        }

        #endregion

        #region IVariable Members

        string IVariable.Name
        {
            get
            {
                return this.Name;
            }
            set
            {
                this.Name = value;
            }
        }

        DataType IVariable.DataType
        {
            get
            {
                return this.DataType;
            }
            set
            {
                this.DataType = value;
            }
        }

        int IVariable.StructType
        {
            get
            {
                return -1;
            }
            set
            {

            }
        }

        int IVariable.ArrayDimensions
        {
            get
            {
                return 0;
            }
            set
            {

            }
        }

        int IVariable.GroupIndex
        {
            get
            {
                return -1;
            }
            set
            {

            }
        }

        #endregion

        #region IWithParent<IFunction> Members

        IFunction IWithParent<IFunction>.Parent
        {
            get
            {
                return this.Parent;
            }
            set
            {
                this.Parent = value;
            }
        }

        #endregion

        #region IWithParent Members

        object IWithParent.Parent
        {
            get
            {
                return this.Parent;
            }
            set
            {
                this.Parent = (IFunction)value;
            }
        }

        #endregion

        #region IWithRoot<AinFile> Members

        AinFile IWithRoot<AinFile>.Root
        {
            get
            {
                return this.Root;
            }
            set
            {
                this.Root = value;
            }
        }

        #endregion
    }

    /// <summary>
    /// A Function type (function poiner)
    /// </summary>
    [TypeConverter(typeof(MyTypeConverter))]
    public class FunctionType : IReadable, IVariable, IFunction, IWithRoot<AinFile>
    {
        public FunctionType()
        {
            _parameters = new VariableCollection();
            _wrapper = new ListWrapper<Variable, IVariable>(_parameters);
        }

        /// <summary>
        /// Name of the data type
        /// </summary>
        public string Name;
        /// <summary>
        /// Data Type of the return value
        /// </summary>
        public DataType DataType;
        /// <summary>
        /// Struct type of the return value, or -1
        /// </summary>
        public int StructType = -1;
        /// <summary>
        /// The number of parameters for the function
        /// </summary>
        public int ParameterCount;
        /// <summary>
        /// The parameters of the function type
        /// </summary>
        public VariableCollection Parameters
        {
            get
            {
                return _parameters;
            }
            set
            {
                _parameters.Clear();
                _parameters.AddRange(value);
            }
        }
        private VariableCollection _parameters;
        private ListWrapper<Variable, IVariable> _wrapper;

        /// <summary>
        /// The array index
        /// </summary>
        [Browsable(false)]
        public int Index = -1;
        /// <summary>
        /// The AIN file
        /// </summary>
        [Browsable(false)]
        public AinFile Root;

        /// <summary>
        /// Reads the function type from a binary reader
        /// </summary>
        /// <param name="br"></param>
        public void Read(BinaryReader br)
        {
            Name = br.ReadStringNullTerminated();
            DataType = (DataType)br.ReadInt32();
            StructType = br.ReadInt32();
            ParameterCount = br.ReadInt32();
            int totalVariables = br.ReadInt32();
            Parameters.Clear();
            Parameters.AddRange(Util.Read<Variable>(br, totalVariables));
        }

        /// <summary>
        /// Writes the function type to a binary writer
        /// </summary>
        /// <param name="bw"></param>
        public void Write(BinaryWriter bw)
        {
            bw.WriteStringNullTerminated(Name);
            bw.Write((int)DataType);
            bw.Write(StructType);
            bw.Write(ParameterCount);
            bw.Write(Parameters.Count);
            Util.Write(bw, Parameters);
        }

        public FunctionType Clone()
        {
            var clone = (FunctionType)this.MemberwiseClone();
            clone._parameters = new VariableCollection();
            clone._wrapper = new ListWrapper<Variable, IVariable>(clone._parameters);
            clone.Parameters.Clear();
            clone.Parameters.AddRange(this.Parameters.Select(a => a.Clone()));
            clone.Root = null;
            return clone;
        }

        #region IReadable Members

        int IWithIndex.Index
        {
            get
            {
                return this.Index;
            }
            set
            {
                this.Index = value;
            }
        }

        #endregion

        public override string ToString()
        {
            if (this.Root == null)
            {
                return ReflectionUtil.ToString(this);
            }
            return ExpressionDisplayer.GetFunctionDeclaration(this, true);
        }

        #region IVariable Members

        string IVariable.Name
        {
            get
            {
                return this.Name;
            }
            set
            {
                this.Name = value;
            }
        }

        DataType IVariable.DataType
        {
            get
            {
                return this.DataType;
            }
            set
            {
                this.DataType = value;
            }
        }

        int IVariable.StructType
        {
            get
            {
                return this.StructType;
            }
            set
            {
                this.StructType = value;
            }
        }

        int IVariable.ArrayDimensions
        {
            get
            {
                return 0;
            }
            set
            {

            }
        }

        int IVariable.GroupIndex
        {
            get
            {
                return -1;
            }
            set
            {

            }
        }
        #endregion

        #region IWithParent<IFunction> Members

        IFunction IWithParent<IFunction>.Parent
        {
            get
            {
                return null;
            }
            set
            {

            }
        }

        #endregion

        #region IWithParent Members

        object IWithParent.Parent
        {
            get
            {
                return null;
            }
            set
            {

            }
        }

        #endregion

        #region IFunction Members

        string IFunction.Name
        {
            get
            {
                return this.Name;
            }
            set
            {
                this.Name = value;
            }
        }

        int IFunction.ParameterCount
        {
            get
            {
                return this.ParameterCount;
            }
            set
            {

            }
        }

        IList<IVariable> IFunction.Parameters
        {
            get
            {
                return this._wrapper;
            }
            set
            {
                this.Parameters.Clear();
                this.Parameters.AddRange(value.Cast<Variable>());
            }
        }

        #endregion

        #region IWithRoot<AinFile> Members

        AinFile IWithRoot<AinFile>.Root
        {
            get
            {
                return this.Root;
            }
            set
            {
                this.Root = value;
            }
        }

        #endregion
    }

    /// <summary>
    /// A function or class method.
    /// </summary>
    [TypeConverter(typeof(MyTypeConverter))]
    public class Function : IReadable, IVariable, IFunction, IWithRoot<AinFile>
    {
        public Function()
        {
            this._parameters = new VariableCollection();
            this._wrapper = new ListWrapper<Variable, IVariable>(_parameters);
        }

        /// <summary>
        /// The address of the code AFTER the FUNC instruction.  Subtract 6 to get the FUNC instruction that begins the function.
        /// </summary>
        public int Address;
        /// <summary>
        /// The name of the function.  For a class method, '@' separates the class name and method name.  0 is a constructor, 1 is a destructor, 2 is an initializer.
        /// </summary>
        public string Name;
        /// <summary>
        /// If this is nonzero, this function can be the target of a "jump" statement.
        /// </summary>
        public int IsLabel;
        /// <summary>
        /// The data type of the variable.  For a struct type, use StructType to determine which type it is.
        /// </summary>
        public DataType DataType;
        /// <summary>
        /// The data type of the struct, or -1 for data types which are not structs.
        /// </summary>
        public int StructType = -1;
        /// <summary>
        /// The number of parameters for the function.  Further items in the Parameters array are local variables.
        /// </summary>
        public int ParameterCount;
        //unknown
        public int Crc;
        /// <summary>
        /// The function parameters and local variables of the function.
        /// </summary>
        public VariableCollection Parameters
        {
            get
            {
                return _parameters;
            }
            set
            {
                _parameters.Clear();
                _parameters.AddRange(value);
            }
        }

        private VariableCollection _parameters;

        /// <summary>
        /// The array index
        /// </summary>
        [Browsable(false)]
        public int Index;
        /// <summary>
        /// The AIN file
        /// </summary>
        [Browsable(false)]
        public AinFile Root;

        /// <summary>
        /// Gets a string containing the declaration of the function, and a list of all local variables in that function.
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            if (this.Root == null)
            {
                return ReflectionUtil.ToString(this);
            }
            return ExpressionDisplayer.GetFunctionDeclaration(this, true);
        }

        /// <summary>
        /// Reads a function from a binary reader
        /// </summary>
        /// <param name="br"></param>
        public void Read(BinaryReader br)
        {
            Address = br.ReadInt32();
            Name = br.ReadStringNullTerminated();
            IsLabel = br.ReadInt32();
            DataType = (DataType)br.ReadInt32();
            StructType = br.ReadInt32();
            ParameterCount = br.ReadInt32();
            int totalVariables = br.ReadInt32();
            Crc = br.ReadInt32();
            Parameters.AddRange(Util.Read<Variable>(br, totalVariables));
        }

        /// <summary>
        /// Writes a function to a binary writer
        /// </summary>
        /// <param name="bw"></param>
        public void Write(BinaryWriter bw)
        {
            bw.Write(Address);
            bw.WriteStringNullTerminated(Name);
            bw.Write(IsLabel);
            bw.Write((int)DataType);
            bw.Write(StructType);
            bw.Write(ParameterCount);
            bw.Write(Parameters.Count);
            bw.Write(Crc);
            Util.Write(bw, Parameters);
        }

        /// <summary>
        /// Creates a function, used when creating the system calls array.
        /// </summary>
        /// <param name="dataType">The return type of the function</param>
        /// <param name="functionName">The name of the function</param>
        /// <param name="dataTypesAndNames">An array containing a data type (DataType), and a name (string) repeating.</param>
        /// <returns></returns>
        public static Function Create(DataType dataType, string functionName, params object[] dataTypesAndNames)
        {
            var newFunction = new Function();
            newFunction.DataType = dataType;
            newFunction.Name = functionName;
            newFunction.Parameters.Clear();
            newFunction.Parameters.AddRange(Variable.Create(dataTypesAndNames));
            //newFunction.totalVariables = newFunction.variables.Length;
            newFunction.ParameterCount = newFunction.Parameters.Count;
            return newFunction;
        }

        /// <summary>
        /// Returns the class name for a function if it exists.
        /// </summary>
        /// <returns></returns>
        public string GetClassName()
        {
            int atPosition = this.Name.IndexOf('@');
            if (atPosition >= 0)
            {
                return this.Name.Substring(0, atPosition);
            }
            else
            {
                return "";
            }
        }

        /// <summary>
        /// Returns the function name for a method, or the original function name.
        /// </summary>
        /// <returns></returns>
        public string GetFunctionName()
        {
            int atPosition = this.Name.IndexOf('@');
            if (atPosition >= 0)
            {
                return this.Name.Substring(atPosition + 1);
            }
            else
            {
                return this.Name;
            }
        }

        /// <summary>
        /// Returns the Struct object for a method.
        /// </summary>
        /// <returns></returns>
        public Struct GetClass()
        {
            if (Root == null)
            {
                return null;
            }
            var className = GetClassName();
            return Root.GetStruct(className);

            //if (Root.StructNameToIndex.ContainsKey(className))
            //{
            //    return Root.Structs[Root.StructNameToIndex[className]];
            //}
            //else
            //{
            //    return null;
            //}
        }

        public Function Clone()
        {
            var clone = (Function)this.MemberwiseClone();
            clone._parameters = new VariableCollection();
            clone._wrapper = new ListWrapper<Variable, IVariable>(clone._parameters);
            clone.Parameters.Clear();
            clone.Parameters.AddRange(this.Parameters.Select(a => a.Clone()));
            clone.Root = null;
            return clone;
        }


        #region IReadable Members

        int IWithIndex.Index
        {
            get
            {
                return this.Index;
            }
            set
            {
                this.Index = value;
            }
        }

        #endregion

        #region IVariable Members

        string IVariable.Name
        {
            get
            {
                return this.Name;
            }
            set
            {
                this.Name = value;
            }
        }

        DataType IVariable.DataType
        {
            get
            {
                return this.DataType;
            }
            set
            {
                this.DataType = value;
            }
        }

        int IVariable.StructType
        {
            get
            {
                return this.StructType;
            }
            set
            {
                this.StructType = value;
            }
        }

        int IVariable.ArrayDimensions
        {
            get
            {
                return 0;
            }
            set
            {

            }
        }

        int IVariable.GroupIndex
        {
            get
            {
                return -1;
            }
            set
            {

            }
        }

        #endregion

        #region IFunction Members

        string IFunction.Name
        {
            get
            {
                return Name;
            }
            set
            {
                Name = value;
            }
        }

        int IFunction.ParameterCount
        {
            get
            {
                return this.ParameterCount;
            }
            set
            {
                this.ParameterCount = value;
            }
        }

        ListWrapper<Variable, IVariable> _wrapper;

        IList<IVariable> IFunction.Parameters
        {
            get
            {
                return _wrapper;
            }
            set
            {
                this.Parameters.Clear();
                this.Parameters.AddRange(value.Cast<Variable>());
            }
        }

        #endregion

        #region IWithParent<IFunction> Members

        IFunction IWithParent<IFunction>.Parent
        {
            get
            {
                return null;
            }
            set
            {

            }
        }

        #endregion

        #region IWithParent Members

        object IWithParent.Parent
        {
            get
            {
                return null;
            }
            set
            {

            }
        }

        #endregion

        #region IWithRoot<AinFile> Members

        AinFile IWithRoot<AinFile>.Root
        {
            get
            {
                return this.Root;
            }
            set
            {
                this.Root = value;
            }
        }

        #endregion
    }

    /// <summary>
    /// A global variable, possibly with a group.
    /// </summary>
    [TypeConverter(typeof(MyTypeConverter))]
    public class Global : IReadable2, IVariable, IWithRoot<AinFile>
    {
        /// <summary>
        /// The name of the global variable
        /// </summary>
        public string Name;
        /// <summary>
        /// The data type of the variable.  For a struct type, use StructType to determine which type it is.
        /// </summary>
        public DataType DataType;
        /// <summary>
        /// The struct type of the variabile.  -1 for variable types which are not structs.
        /// </summary>
        public int StructType = -1;
        /// <summary>
        /// The number of dimensions of the array.  0 for variables which are not arrays.
        /// </summary>
        public int ArrayDimensions = 0;
        /// <summary>
        /// The group of a global variable.  -1 for variables which are not in a global group.
        /// </summary>
        public int GroupIndex = -1;
        /// <summary>
        /// The array index
        /// </summary>
        [Browsable(false)]
        public int Index = -1;
        /// <summary>
        /// The AIN file
        /// </summary>
        [Browsable(false)]
        public AinFile Root;

        public Global()
        {
            GroupIndex = -1;
        }
        public Global(BinaryReader br, int version)
        {
            Read(br, version);
        }

        public void Read(BinaryReader br, int version)
        {
            Name = br.ReadStringNullTerminated();
            DataType = (DataType)br.ReadInt32();
            StructType = br.ReadInt32();
            ArrayDimensions = br.ReadInt32();
            GroupIndex = -1;
            if (version >= 5)
            {
                GroupIndex = br.ReadInt32();
            }
        }
        public void Write(BinaryWriter bw, int version)
        {
            bw.WriteStringNullTerminated(Name);
            bw.Write((int)DataType);
            bw.Write(StructType);
            bw.Write(ArrayDimensions);
            if (version >= 5)
            {
                bw.Write(GroupIndex);
            }
        }

        public Global Clone()
        {
            var clone = (Global)this.MemberwiseClone();
            clone.Root = null;
            return clone;
        }

        #region IReadable Members

        int IWithIndex.Index
        {
            get
            {
                return this.Index;
            }
            set
            {
                this.Index = value;
            }
        }

        #endregion

        /// <summary>
        /// Gets the name of the group of this global variable
        /// </summary>
        public string GroupName
        {
            get
            {
                if (Root != null && GroupIndex >= 0 && GroupIndex < Root.GlobalGroupNames.Count)
                {
                    return Root.GlobalGroupNames[GroupIndex];
                }
                return null;
            }
            set
            {
                if (Root != null)
                {
                    int index = Root.GlobalGroupNames.IndexOf(value);
                    if (index >= 0)
                    {
                        GroupIndex = index;
                    }
                }

            }
        }

        public override string ToString()
        {
            if (this.Root == null)
            {
                return ReflectionUtil.ToString(this);
            }
            var groupName = this.GroupName;
            return ExpressionDisplayer.GetDataTypeName(this) + " " + ((!String.IsNullOrEmpty(groupName)) ? (groupName + "." + this.GetActiveName()) : this.GetActiveName());
        }

        //public Variable ToVariable()
        //{
        //    return new Variable() { dataType = dataType, name = name, arrayDimensions = arrayDimensions, structType = structType };
        //}

        #region IVariable Members

        string IVariable.Name
        {
            get
            {
                return this.Name;
            }
            set
            {
                this.Name = value;
            }
        }

        DataType IVariable.DataType
        {
            get
            {
                return this.DataType;
            }
            set
            {
                this.DataType = value;
            }
        }

        int IVariable.StructType
        {
            get
            {
                return this.StructType;
            }
            set
            {
                this.StructType = value;
            }
        }

        int IVariable.ArrayDimensions
        {
            get
            {
                return this.ArrayDimensions;
            }
            set
            {
                this.ArrayDimensions = value;
            }
        }

        int IVariable.GroupIndex
        {
            get
            {
                return this.GroupIndex;
            }
            set
            {
                this.GroupIndex = value;
            }
        }

        #endregion

        #region IWithParent<IFunction> Members

        IFunction IWithParent<IFunction>.Parent
        {
            get
            {
                return null;
            }
            set
            {

            }
        }

        #endregion

        #region IWithParent Members

        object IWithParent.Parent
        {
            get
            {
                return null;
            }
            set
            {

            }
        }

        #endregion

        #region IWithRoot<AinFile> Members

        AinFile IWithRoot<AinFile>.Root
        {
            get
            {
                return this.Root;
            }
            set
            {
                this.Root = value;
            }
        }

        #endregion
    }

    /// <summary>
    /// A struct or class
    /// </summary>
    [TypeConverter(typeof(MyTypeConverter))]
    public class Struct : IReadable, IFunction, IWithRoot<AinFile>
    {
        public Struct()
        {
            _members = new VariableCollection();
            _wrapper = new ListWrapper<Variable, IVariable>(_members);
        }


        /// <summary>
        /// The name of the struct
        /// </summary>
        public string Name;
        /// <summary>
        /// The function ID of the constructor function
        /// </summary>
        public int Constructor = -1;
        /// <summary>
        /// The function ID of the destructor function
        /// </summary>
        public int Destructor = -1;
        /// <summary>
        /// The number of members
        /// </summary>
        public int NumberOfMembers;
        /// <summary>
        /// The array index
        /// </summary>
        [Browsable(false)]
        public int Index = -1;
        /// <summary>
        /// The AIN file
        /// </summary>
        [Browsable(false)]
        public AinFile Root;

        public VariableCollection Members
        {
            get
            {
                return _members;
            }
            set
            {
                _members.Clear();
                _members.AddRange(value);
            }
        }
        VariableCollection _members;
        ListWrapper<Variable, IVariable> _wrapper;

        public void Read(BinaryReader br)
        {
            Name = br.ReadStringNullTerminated();
            Constructor = br.ReadInt32();
            Destructor = br.ReadInt32();
            NumberOfMembers = br.ReadInt32();
            Members.Clear();
            Members.AddRange(Util.Read<Variable>(br, NumberOfMembers));
        }

        public void Write(BinaryWriter bw)
        {
            bw.WriteStringNullTerminated(Name);
            bw.Write(Constructor);
            bw.Write(Destructor);
            bw.Write(Members.Count);
            Util.Write(bw, Members);
        }

        public override string ToString()
        {
            //if (Root == null)
            //{
            //    return ReflectionUtil.ToString(this);
            //}
            StringBuilder sb = new StringBuilder();
            sb.Append("struct " + this.GetActiveName());
            if (this.Members.Count > 0)
            {
                sb.AppendLine();
                sb.AppendLine("{");
                for (int i = 0; i < Members.Count; i++)
                {
                    sb.Append("\t" + Members[i].ToString());
                    if (i < Members.Count - 1)
                    {
                        sb.Append(",");
                    }
                    sb.AppendLine();
                }
                sb.Append("}");
            }
            return sb.ToString();
        }

        /// <summary>
        /// Returns the function that corresponds to the Constructor function ID
        /// </summary>
        /// <returns></returns>
        public Function GetConstructor()
        {
            int functionNumber = this.Constructor;
            if (functionNumber >= 0 && functionNumber < Root.Functions.Count)
            {
                return Root.Functions[functionNumber];
            }
            else
            {
                return null;
            }
        }

        /// <summary>
        /// Returns the function that corresponds to the Destructor function ID.
        /// </summary>
        /// <returns></returns>
        public Function GetDestructor()
        {
            int functionNumber = this.Destructor;
            if (functionNumber >= 0 && functionNumber < Root.Functions.Count)
            {
                return Root.Functions[functionNumber];
            }
            else
            {
                return null;
            }
        }

        public Struct Clone()
        {
            var clone = (Struct)this.MemberwiseClone();
            clone._members = new VariableCollection();
            clone._wrapper = new ListWrapper<Variable, IVariable>(clone._members);
            clone.Members.Clear();
            clone.Members.AddRange(this.Members.Select(m => m.Clone()));
            return clone;
        }

        #region IFunction Members

        string IFunction.Name
        {
            get
            {
                return this.Name;
            }
            set
            {
                this.Name = value;
            }
        }

        int IFunction.ParameterCount
        {
            get
            {
                return this.Members.Count;
            }
            set
            {

            }
        }

        IList<IVariable> IFunction.Parameters
        {
            get
            {
                return this._wrapper;
            }
            set
            {
                this.Members.Clear();
                this.Members.AddRange(value.Cast<Variable>());
            }
        }

        #endregion

        #region IVariable Members

        string IVariable.Name
        {
            get
            {
                return this.Name;
            }
            set
            {
                this.Name = value;
            }
        }

        DataType IVariable.DataType
        {
            get
            {
                return DataType.Struct;
            }
            set
            {

            }
        }

        int IVariable.StructType
        {
            get
            {
                return this.Index;
            }
            set
            {

            }
        }

        int IVariable.ArrayDimensions
        {
            get
            {
                return 0;
            }
            set
            {

            }
        }

        int IVariable.GroupIndex
        {
            get
            {
                return -1;
            }
            set
            {

            }
        }

        #endregion

        #region IWithIndex Members

        int IWithIndex.Index
        {
            get
            {
                return this.Index;
            }
            set
            {
                this.Index = value;
            }
        }

        #endregion

        #region IWithParent<IFunction> Members

        IFunction IWithParent<IFunction>.Parent
        {
            get
            {
                return null;
            }
            set
            {

            }
        }

        #endregion

        #region IWithParent Members

        object IWithParent.Parent
        {
            get
            {
                return null;
            }
            set
            {

            }
        }

        #endregion

        #region IWithRoot<AinFile> Members

        AinFile IWithRoot<AinFile>.Root
        {
            get
            {
                return this.Root;
            }
            set
            {
                this.Root = value;
            }
        }

        #endregion
    }

    /// <summary>
    /// The initial value for a variable
    /// </summary>
    [TypeConverter(typeof(MyTypeConverter))]
    public class InitialValue : IEquatable<InitialValue>
    {
        public InitialValue()
        {

        }

        public InitialValue(int intValue)
        {
            SetIntValue(intValue);
        }

        public InitialValue(string stringValue)
        {
            SetStringValue(stringValue);
        }

        public InitialValue(float floatValue)
        {
            SetFloatValue(floatValue);
        }

        public InitialValue(bool boolValue)
        {
            SetBoolValue(boolValue);
        }

        /// <summary>
        /// The data type of the variable
        /// </summary>
        public DataType DataType;
        /// <summary>
        /// The initial string value, or null
        /// </summary>
        public string StringValue;
        /// <summary>
        /// The initial value expressed as an integer
        /// </summary>
        public int IntValue;

        /// <summary>
        /// The initial value expressed as a float
        /// </summary>
        public float FloatValue
        {
            get
            {
                return BitConverter.ToSingle(BitConverter.GetBytes(IntValue), 0);
            }
            set
            {
                IntValue = BitConverter.ToInt32(BitConverter.GetBytes(value), 0);
            }
        }

        /// <summary>
        /// Returns the value of this initial value as a string, and quotes and escapes it if it's a string.
        /// </summary>
        /// <returns>The value of this initial value as a string, quoted and escaped if it's a string</returns>
        public string GetValueQuoted()
        {
            if (this.DataType == DataType.String)
            {
                return "\"" + AssemblerProjectWriter.EscapeString(GetValue() ?? "") + "\"";
            }
            else
            {
                return GetValue() ?? "";
            }
        }
        public string GetValue()
        {
            string myValue;
            if (this.DataType == DataType.String || this.DataType == DataType.RefString)
            {
                myValue = this.StringValue;
            }
            else if (this.DataType == DataType.Float || this.DataType == DataType.RefFloat)
            {
                myValue = this.FloatValue.FloatToString();
            }
            else if (this.DataType == DataType.Bool || this.DataType == DataType.RefBool)
            {
                myValue = (this.IntValue == 0) ? "false" : "true";
            }
            else
            {
                myValue = this.IntValue.ToString(CultureInfo.InvariantCulture);
            }
            return myValue;
        }

        public bool SetValue(DataType dataType, string stringValue)
        {
            if (this.DataType == DataType.String || this.DataType == DataType.RefString)
            {
                SetStringValue(stringValue);
                return true;
            }
            else if (this.DataType == DataType.Float || this.DataType == DataType.RefFloat)
            {
                double doubleValue = 0;
                if (double.TryParse(stringValue, out doubleValue))
                {
                    SetFloatValue((float)doubleValue);
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else if (this.DataType == DataType.Bool || this.DataType == DataType.RefBool)
            {
                if (stringValue == "false")
                {
                    SetBoolValue(false);
                    return true;
                }
                else if (stringValue == "true")
                {
                    SetBoolValue(true);
                    return true;
                }
                return false;
            }
            else
            {
                //int value
                int intValue;
                if (int.TryParse(stringValue, out intValue))
                {
                    SetIntValue(intValue);
                    return true;
                }
                return false;
            }
        }

        public void SetStringValue(string stringValue)
        {
            this.StringValue = stringValue;
            this.IntValue = 0;
            this.DataType = DataType.String;
        }

        public void SetFloatValue(float floatValue)
        {
            this.DataType = DataType.Float;
            this.FloatValue = floatValue;
        }

        public void SetBoolValue(bool boolValue)
        {
            this.IntValue = boolValue ? 1 : 0;
            this.DataType = DataType.Bool;
        }

        public void SetIntValue(int intValue)
        {
            this.IntValue = intValue;
            this.DataType = DataType.Int;
        }

        public override string ToString()
        {
            return GetValue();
        }

        public InitialValue Clone()
        {
            var clone = (InitialValue)this.MemberwiseClone();
            return clone;
        }

        #region IEquatable<InitialValue> Members

        public bool Equals(InitialValue other)
        {
            if (this != null && other == null)
            {
                return false;
            }
            return this.DataType == other.DataType &&
                this.IntValue == other.IntValue &&
                this.StringValue == other.StringValue;
        }

        #endregion
    }

    /// <summary>
    /// The initial value for a global variable
    /// </summary>
    [TypeConverter(typeof(MyTypeConverter))]
    public class GlobalInitialValue : InitialValue, IReadable, IWithRoot<AinFile>
    {
        public GlobalInitialValue()
        {

        }

        public GlobalInitialValue(InitialValue initialValue)
        {
            this.DataType = initialValue.DataType;
            this.IntValue = initialValue.IntValue;
            this.StringValue = initialValue.StringValue;
        }

        /// <summary>
        /// The index of the global variable this corresponds to
        /// </summary>
        public int GlobalIndex = -1;
        /// <summary>
        /// The AIN file
        /// </summary>
        [Browsable(false)]
        public AinFile Root;

        public void Read(BinaryReader br)
        {
            GlobalIndex = br.ReadInt32();
            DataType = (DataType)br.ReadInt32();
            if (DataType == DataType.String)
            {
                StringValue = br.ReadStringNullTerminated();
            }
            else
            {
                IntValue = br.ReadInt32();
            }
        }

        public void Write(BinaryWriter bw)
        {
            bw.Write(GlobalIndex);
            bw.Write((int)DataType);
            if (DataType == DataType.String)
            {
                bw.WriteStringNullTerminated(StringValue);
            }
            else
            {
                bw.Write(IntValue);
            }
        }

        /// <summary>
        /// Gets or Sets the global variable that corresponds to the initial value
        /// </summary>
        [Browsable(false)]
        public Global GlobalVariable
        {
            get
            {
                if (Root != null && GlobalIndex >= 0 && GlobalIndex < Root.Globals.Count)
                {
                    return Root.Globals[GlobalIndex];
                }
                return null;
            }

            set
            {
                if (Root != null)
                {
                    int index = Root.Globals.IndexOf(value);
                    if (index >= 0)
                    {
                        this.GlobalIndex = index;
                    }
                }
            }
        }

        public override string ToString()
        {
            if (this.Root == null)
            {
                return ReflectionUtil.ToString(this);
            }
            string myValue = GetValue();
            return GlobalVariable.ToString() + " = " + myValue;
        }

        public new GlobalInitialValue Clone()
        {
            var clone = (GlobalInitialValue)this.MemberwiseClone();
            return clone;
        }

        #region IWithRoot<AinFile> Members

        AinFile IWithRoot<AinFile>.Root
        {
            get
            {
                return this.Root;
            }
            set
            {
                this.Root = value;
            }
        }

        #endregion
    }
}
