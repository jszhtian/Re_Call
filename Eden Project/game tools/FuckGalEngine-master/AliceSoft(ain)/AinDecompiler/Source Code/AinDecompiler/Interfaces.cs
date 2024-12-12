using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;

namespace AinDecompiler
{
    /// <summary>
    /// An object that has a Root object.
    /// </summary>
    /// <typeparam name="T">The type of the root object</typeparam>
    public interface IWithRoot<T>
    {
        /// <summary>
        /// The Root of an object.
        /// </summary>
        T Root { get; set; }
    }

    /// <summary>
    /// An object that has an array Index property.
    /// </summary>
    public interface IWithIndex
    {
        /// <summary>
        /// The array index of the object.
        /// </summary>
        int Index { get; set; }
    }

    /// <summary>
    /// An object that has a parent
    /// </summary>
    public interface IWithParent
    {
        /// <summary>
        /// The parent of this object.
        /// </summary>
        object Parent { get; set; }
    }

    /// <summary>
    /// An object that has a parent
    /// </summary>
    /// <typeparam name="T">The type of the parent</typeparam>
    public interface IWithParent<T> : IWithParent
    {
        /// <summary>
        /// The parent of this object.
        /// </summary>
        new T Parent { get; set; }
    }

    /// <summary>
    /// An object that has Read() and Write() methods that need the version number.
    /// </summary>
    public interface IReadable2
    {
        /// <summary>
        /// Reads this object from a binary reader, given a version number
        /// </summary>
        /// <param name="br">The binary reader to read from</param>
        /// <param name="version">The version number</param>
        void Read(BinaryReader br, int version);
        /// <summary>
        /// Writes this object to a binary writer, given a version number
        /// </summary>
        /// <param name="bw">The binary writer to write to</param>
        /// <param name="version">The version number</param>
        void Write(BinaryWriter bw, int version);
    }

    /// <summary>
    /// An object that has Read() and Write() methods
    /// </summary>
    public interface IReadable
    {
        /// <summary>
        /// Reads this object from a binary reader
        /// </summary>
        /// <param name="br">The binary reader to read from</param>
        void Read(BinaryReader br);
        /// <summary>
        /// Writes this object to a binary writer
        /// </summary>
        /// <param name="bw">The binary writer to write to</param>
        void Write(BinaryWriter bw);
    }

    /// <summary>
    /// A function object (Function, HllFunction, FunctionType, or Struct)
    /// </summary>
    public interface IFunction : IVariable
    {
        /// <summary>
        /// The name of the function
        /// </summary>
        string Name { get; set; }
        /// <summary>
        /// The nummber of function parameters.
        /// </summary>
        int ParameterCount { get; set; }
        /// <summary>
        /// The list of function parameters.  If it contains more items than ParameterCount, remaining items are local variables.
        /// </summary>
        IList<IVariable> Parameters { get; set; }
    }

    /// <summary>
    /// A variable object (Variable, Global, or FunctionArg) or the return type of a Function object (Function, HllFunction, FunctionType, or Struct)
    /// </summary>
    public interface IVariable : IWithIndex, IWithParent<IFunction>, IWithRoot<AinFile>
    {
        /// <summary>
        /// The name of the variable
        /// </summary>
        string Name { get; set; }
        /// <summary>
        /// The data type of the variable.  For a struct type, use StructType to determine which type it is.
        /// </summary>
        DataType DataType { get; set; }
        /// <summary>
        /// The struct type of the variabile.  -1 for variable types which are not structs.
        /// </summary>
        int StructType { get; set; }
        /// <summary>
        /// The number of dimensions of the array.  0 for variables which are not arrays.
        /// </summary>
        int ArrayDimensions { get; set; }
        /// <summary>
        /// The group of a global variable.  -1 for variables which are not in a global group.
        /// </summary>
        int GroupIndex { get; set; }
    }
}
